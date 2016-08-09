/*
 * IBM (C) Copyright 2016, 2016 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "../IUCV/iucvserver.h"


/*Check authorization for client.
* For Security reason, IUCV server only allow OPNCLOUD user to make IUCV communication.
*
* @param $1: the socket which is used to make IUCV communication to client.
*        $2: the userid which is sent from IUCV command.
*
* @return 0: successfully to make authorized.
*     errno: other reason which lead to authorized failed
*     UNAUTHORIZED_ERROR: userid is not authorized.
*/
int check_client_authorization(int newsockfd, char *req_userid)
{
    /* authoroized_useid is saved in PATH_FOR_AUTHORIZED_USERID */
    FILE *fp = NULL;
    int len = 0;
    char client_userid[16], err_msg[BUFFER_SIZE];
    /* authorized file is copied for opencloud when IUCV initialized*/
    fp = fopen(PATH_FOR_AUTHORIZED_USERID,"r+");
    if( NULL == fp)
    {
        /* all the message sent to client, should start with UNAUTHORIZED_ERROR: reason.#errno*/
        sprintf(err_msg, "UNAUTHORIZED_ERROR: Authorized path %s doesn't exist on iucvserver.#%d", PATH_FOR_AUTHORIZED_USERID, errno);
        syslog(LOG_ERR, err_msg);
        send(newsockfd, err_msg, strlen(err_msg)+1, 0);
        return errno;
    }
    else
    {
        syslog(LOG_INFO,"%s exists, check authorization.\n", PATH_FOR_AUTHORIZED_USERID);
        fseek(fp , 0 , SEEK_END);
        len = ftell(fp);
        if(len > 16)
        {
            sprintf(err_msg, "UNAUTHORIZED_ERROR: Userid in authorized file is not correct.#0",PATH_FOR_AUTHORIZED_USERID);
            syslog(LOG_ERR, err_msg);
            send(newsockfd, err_msg, strlen(err_msg)+1, 0);
            fclose(fp);
            fp = NULL;
            return UNAUTHORIZED_ERROR;
        }
        fseek(fp, 0, SEEK_SET);
        bzero(client_userid, 16);
        if(fread(client_userid, 1, len, fp)!=len)
        {
            sprintf(err_msg, "UNAUTHORIZED_ERROR: Failed to read userid from %s.#%d",PATH_FOR_AUTHORIZED_USERID, errno);
            syslog(LOG_ERR, err_msg);
            send(newsockfd, err_msg, strlen(err_msg)+1, 0);
            fclose(fp);
            fp = NULL;
            return errno;
        }
        syslog(LOG_INFO, "senduserid=%s, authuserid=%s, len=%d",req_userid, client_userid,len);
        if (fclose(fp) != 0)
        {
            syslog(LOG_ERR, "ERROR Fail to close authorized file after reading: %s\n",strerror(errno));
        }
        fp = NULL;
        /* if the userid is not authorized, send error message back*/
        if(strcasecmp(req_userid,client_userid))
        {
            sprintf(err_msg, "UNAUTHORIZED_ERROR: Userid %s is not authorized, IUCV agent only can communicate with specified open cloud user!#0", req_userid);
            syslog(LOG_ERR, err_msg);
            send(newsockfd, err_msg, strlen(err_msg)+1, 0);
            return UNAUTHORIZED_ERROR;
        }
    } /* End of check authorization */
    return 0;
}


/* receicve file (NOT support to tranport folder) from client for upgrade or transport
*
* @param $1: the socket which is used to make IUCV communication to client.
*        $2: the path for the file which need to be saved
*
* @return 0: if file transport is successful.
*         FILE_TRANSPORT_ERROR: if file transport is failed.
*         errno: if there are any socket connection error.
*/
int receive_file_from_client(int newsockfd, char *des_path)
{
    char buffer[BUFFER_SIZE], md5[256];
    int n = 0;
    FILE * fp = NULL;
    syslog(LOG_INFO,"Will receive and save file to %s which is sent from IUCV client.\n", des_path);
    /* If the destination path is a folder, return error */
    struct stat st;
    stat(des_path, &st);
    if((st.st_mode & S_IFDIR) == S_IFDIR)
    {
        sprintf(buffer, "The destination path %s should include the file name.", des_path);
        syslog(LOG_ERR, buffer);
        send(newsockfd, buffer, strlen(buffer)+1, 0);
        return errno;
    }
    /* If file exists, rename it firstly */
    if(!access(des_path, 0))
    {
        syslog(LOG_INFO,"Transport file has existed on system, rename it to xxx.iucvold.");
        sprintf(buffer,"mv %s %s.iucvold", des_path, des_path);
        system(buffer);
    }
    fp = fopen(des_path, "ab");
    if(NULL == fp)
    {
        sprintf(buffer, "Failed to open file %s for write", des_path);
        syslog(LOG_ERR, buffer);
        send(newsockfd, buffer,strlen(buffer)+1, 0);
        return errno;
    }
    send(newsockfd, READY_TO_RECEIVE,strlen(READY_TO_RECEIVE)+1, 0);
    bzero(buffer,BUFFER_SIZE);
    while((n=recv(newsockfd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        buffer[n] = 0;
        syslog(LOG_DEBUG,"write n=%d,data=%s,len=%u\n",n, buffer, strlen(buffer));
        if(strncmp(buffer, FILE_SENT_OVER, strlen(FILE_SENT_OVER) )==0)
        {
            syslog(LOG_INFO, "FILE_SENT_OVER");
            /* Get md5 code from client */
            strcpy(md5, buffer + strlen(FILE_SENT_OVER) + 1);
            syslog(LOG_INFO,"file md5=%s",md5);
            break;
        }
        syslog(LOG_INFO,"start to write file\n");
        if(fwrite(buffer, n, 1, fp)!=1)
        {
            syslog(LOG_ERR,"Failed to write file to %s", des_path);
            strcpy(buffer,"DATA_SENT_ERROR");
            send(newsockfd, buffer, strlen(buffer)+1, 0);
            if (fclose(fp) != 0)
            {
                syslog(LOG_ERR, "ERROR Fail to close received file after writing failed: %s\n",strerror(errno));
            }
            fp = NULL;
            return errno;
        }
        syslog(LOG_INFO, "write to file a time");
        bzero(buffer,BUFFER_SIZE);
    }
    if (fclose(fp) != 0)
    {
        syslog(LOG_ERR, "ERROR Fail to close received file after writing: %s\n",strerror(errno));
    }
    fp = NULL;
    if (n < 0)
    {
        syslog(LOG_ERR, "ERROR reading from socket to get the tranport file: %s\n",strerror(errno));
        return errno;
    }

    /* After finish sending file, send message to client */
    sprintf(buffer, "md5sum %s",des_path);
    if((fp = popen(buffer, "r"))==NULL)
    {
        syslog(LOG_ERR,"Failed to get md5 for file %s.",buffer);
        strcpy(buffer,"FILE_RECEIVED_FAILED");
        send(newsockfd,buffer,strlen(buffer)+1,0);
        syslog(LOG_INFO, "send FILE_RECEIVED_FAILED to client",buffer);
        return FILE_TRANSPORT_ERROR;
    }
    else
    {
        bzero(buffer,BUFFER_SIZE);
        if(fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if(strncmp(md5, buffer, 32)==0) //md5 is 32 bytes
            {
                strcpy(buffer,"FILE_RECEIVED_OK");
                send(newsockfd,buffer,strlen(buffer)+1,0);
                syslog(LOG_INFO, "send FILE_RECEIVED_OK to client",buffer);
                pclose(fp);
                fp = NULL;
                return 0;
            }
        }
    }
    pclose(fp);
    fp = NULL;
    strcpy(buffer,"FILE_RECEIVED_FAILED");
    send(newsockfd,buffer,strlen(buffer)+1,0);
    syslog(LOG_INFO, "send FILE_RECEIVED_FAILED to client",buffer);
    return errno;
}


/* When server's version is lower than client side server version,
*  upgrade is needed.
* @param
*
* @return 0:
*/
int handle_upgrade()
{
    /* 1. Send upgrade needed signal to client.*/
    /* 2. Receive new version file from client.*/
    /* 3. Execute restart command which is got from client.*/
    return 0;
}


/* Main logic, use to make socket connection.
*
* @param
*
* @return 0: successfully.
*         errno: any socket error.
*/
int server_socket()
{
    int sockfd, newsockfd, portno, clilen;
    char send_buf[BUFFER_SIZE], buffer[BUFFER_SIZE];
    struct sockaddr_iucv cli_addr;
    struct sockaddr_iucv serv_addr;
    size_t len, on=1;
    int n, returncode = 0;
    FILE *fp = NULL;
    char tmp[16];

    /* First call to socket() function */
    sockfd = socket(AF_IUCV, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        syslog(LOG_ERR, "ERROR opening socket: %s\n",strerror(errno));
        close(sockfd);
        return errno;
    }

    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
    {
        syslog(LOG_ERR, "ERROR setsockopt: %s\n",strerror(errno));
        close(sockfd);
        return errno;
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.siucv_family = AF_IUCV;
    memcpy(serv_addr.siucv_name, "OPNCLOUD", 8);
    memcpy(serv_addr.siucv_user_id, "", 8);
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        syslog(LOG_ERR, "ERROR on binding: %s\n",strerror(errno));
        close(sockfd);
        return errno;
    }
    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    if (listen(sockfd,SOCKET_TIMEOUT) < 0)
    {
        syslog(LOG_ERR, "ERROR on liston: %s\n",strerror(errno));
        close(sockfd);
        return errno;
    }
    clilen = sizeof(cli_addr);
    while(1)
    {
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            syslog(LOG_ERR, "ERROR on accepting: %s\n",strerror(errno));
            close(newsockfd);
            close(sockfd);
            return errno;
        }
        /* If connection is established then start communicating */
        bzero(buffer,BUFFER_SIZE);
        n =  recv(newsockfd,buffer, BUFFER_SIZE, 0);
        if (n < 0)
        {
            syslog(LOG_ERR, "ERROR reading from socket: %s\n",strerror(errno));
            close(newsockfd);
            close(sockfd);
            return errno;
        }
        syslog(LOG_INFO,"Receive %s sent from IUCV client.\n", buffer);

        bzero(tmp, 16);
        len = strcspn(buffer, " ");
        strncpy(tmp, buffer, len);
        strcpy(buffer, buffer+len+1);
        /*check the client userid's authorized*/
        if((returncode = check_client_authorization(newsockfd, tmp)) != 0)
        {
            close(newsockfd);
            close(sockfd);
            return returncode;
        }
        /* Check upgrade.
           This is used for IUCV server's upgrade, when the server's version which installed on VM is lower,
           upgrade is needed.
        */
        len = strcspn(buffer, " ");
        bzero(tmp, 16);
        strncpy(tmp, buffer, len);
        syslog(LOG_DEBUG, "Current version is %s, upgraded version is %s", IUCV_SERVER_VERSION, tmp);
        if(strcmp(tmp, IUCV_SERVER_VERSION) > 0)
        {
            syslog(LOG_ERR, "Upgrade for IUCV is needed.Current version is %s, upgraded version is %s", IUCV_SERVER_VERSION, tmp);
            if((returncode = handle_upgrade()) != 0)
            {
                close(newsockfd);
                close(sockfd);
                return returncode;
            }
        }
        /* Handle the commands sent from client. */
        if(strlen(buffer) > len)
        {
            strcpy(buffer, buffer + strlen(tmp) + 1);
            /* for file_transport command, received string should contain the path.
               if user is authorized, just accept the file, or else will pop up the error.
               (to-do) add a Mutex, when file is transport, command is not be allowed.
            */
            if(strncmp(buffer, FILE_TRANSPORT, strlen(FILE_TRANSPORT))==0)
            {
                strtok(buffer," ");
                char path[BUFFER_SIZE], tmp_path[BUFFER_SIZE + 10];
                strcpy(path, strtok(NULL," "));
                if((returncode = receive_file_from_client(newsockfd, path)) != 0)
                {
                    //rollback
                    sprintf(tmp_path,"%s.iucvold", path);
                    if(!access(tmp_path, 0))
                    {
                        syslog(LOG_INFO,"rollback file from xxx.iucvold to xxx.");
                        sprintf(buffer,"mv %s %s", tmp_path, path);
                        system(buffer);
                    }
                    close(newsockfd);
                    close(sockfd);
                    return returncode;
                }
                syslog(LOG_INFO,"Finish %s file transport.",path);
            }
            else
            {
                //(to-do) tranport passwd.
                /* to collect the system error info*/
                strcat(buffer, "  2>&1; echo iucvcmdrc=$?");
                syslog(LOG_INFO,"Will execute the linux command %s sent from IUCV client.\n", buffer);

                if(NULL == (fp=popen(buffer, "r")))
                {
                    strcpy(buffer, "Failed to execute command with popen.");
                    syslog(LOG_ERR, buffer);
                    send(newsockfd, buffer,strlen(buffer)+1, 0);
                    close(newsockfd);
                    close(sockfd);
                    return errno;
                }

                /* Write a response to the client */
                len = 0;
                bzero(send_buf, BUFFER_SIZE);
                while (fgets(buffer, sizeof(buffer), fp) != NULL)
                {
                    /* get all the results and send it,
                    if result > 1024, will send it per 1024 byte
                    */
                    len += strlen(buffer);
                    if(len <= BUFFER_SIZE)
                    {
                        strcat(send_buf, buffer);
                    }
                    if(len >= BUFFER_SIZE)
                    {
                        syslog(LOG_INFO,"result length=%u, send message length=%u,\n %s", len, strlen(send_buf), send_buf);
                        send_buf[strlen(send_buf)] = 0;
                        send(newsockfd, send_buf,strlen(send_buf)+1, 0);
                        len = strlen(buffer);
                        /* put the last string to the buffer to send it the next time */
                        strcpy(send_buf,buffer);
                     }
                }
                syslog(LOG_INFO,"result length=%u, send message length=%u,\n %s", len, strlen(send_buf), send_buf);
                send_buf[strlen(send_buf)] = 0;
                send(newsockfd, send_buf, strlen(send_buf)+1, 0);
                /* close */
                pclose(fp);
                fp = NULL;
            }
        }
        close(newsockfd);
    }
    close(sockfd);
    syslog(LOG_INFO, "Successfully to stop to IUCV server");
    return 0;
}


/* Main function.
*
* @param $1: the number of param.
*        $2: the params which is input.
*
* @return 0: if file transport is successful.
*         -1: Usage:
*/
int main(int argc,char* argv[])
{
    /*(to-do) change to use "getopt"*/
    if(argc==2 && strcmp(argv[1],"--version")==0)
    {
        printf("%s\n", IUCV_SERVER_VERSION);
        return 0;
    }
    /* This is used by RHEL6 SLES11 */
    else if(argc==2 && strcmp(argv[1],"start-as-daemon")==0)
    {
        syslog(LOG_INFO,"IUCV server start to work as daemon!\n");
        daemon(0,0);
        while(1)
        {
            syslog(LOG_INFO,"Begin a new socket.\n");
            server_socket();
        }
        return 0;
    }
    /* This is used by RHEL7 SLES12 */
    else if(argc==2 && strcmp(argv[1],"start")==0)
    {
        syslog(LOG_INFO,"IUCV server start to work!\n");
        while(1)
        {
            syslog(LOG_INFO,"Begin a new socket.\n");
            server_socket();
        }
        return 0;
    }
    else
    {
        printf("Usage: iucvserver [--version] [start] [start-as-daemon]\n");
        return -1;
    }
}