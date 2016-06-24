/*
 * IBM (C) Copyright 2016, 2016 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "../IUCV/iucvclient.h"


/* print log to console and save it to /var/log/messages.
* @param $1: the return value for client.
*        $2: the reason for the details.
*        $3: results.
*
* @return returncode:
*/
int printAndLogIUCVserverReturnCodeReasonCodeoutput(int returncode, int reasoncode, char * message)
{
    char msg[BUFFER_SIZE];
    strcpy(msg,message);
    if(returncode || reasoncode)
    {
        strcat(msg,strerror(reasoncode));
        syslog(LOG_ERR,"%s", msg);
        printf("return code %d, reason code %d, %s\n", returncode, reasoncode, msg);
    }
    eles
    {
        printf("%s", msg);
    }
    return returncode;
}


/* Prepare the commands which will sent to server.
*  the command format should be:
*  client_userid clientside_server_version command [parameters]
*  @param $1: output parm which is used to get the commands.
*         $2: get from main.
*         $3: get from main.
*
*  @return 0: successfully.
           errno:
*/
int prepare_commands(char* buffer, int argc, char *argv[])
{
    /* 1. Prepare client_userid. */
    /* Now ask for a message from the user, this message will be read by server */
    /* Need to add userid to make authorized, if not a opencloud user, request will be refused */
    char user_buf[32];
    int i = 0;

    bzero(buffer,BUFFER_SIZE);
    FILE *fp = popen("vmcp q userid", "r");
    if(fgets(user_buf,sizeof(user_buf),fp) != NULL)
    {
        strcpy(buffer, strtok(user_buf," "));
    }
    else
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(UNAUTHORIZED_ERROR, errno,"ERROR failed to get userid:");
        return errno;
    }
    /* 2. Get the IUCV server's version which exists on zhcp
       This is used for IUCV server's upgrade, when the server's version which installed on VM is lower,
       upgrade is needed.
    */
    char tmp_buf[BUFFER_SIZE];
    sprintf(tmp_buf, "%s --version", FILE_PATH_IUCV_SERVER);
    fp = NULL;
    fp = popen(tmp_buf, "r");
    if(fgets(tmp_buf, sizeof(tmp_buf), fp) != NULL)
    {
        //printf(tmp_buf);
        strcat(buffer, " ");
        strcat(buffer, tmp_buf);
        buffer[strlen(buffer)-1]='\0';//command has an enter;
    }
    fp = NULL;
    close(fp);

    /* 3.commands */
    for (i = 2; i < argc;i++)
    {
        /*if command is transport file, the source path is useless*/
        if(strcmp(argv[2], FILE_TRANSPORT)==0)
        {
            if(i == 3)
            {
                continue;
            }
        }
        strcat(buffer, " ");
        strcat(buffer, argv[i]);
    }
    printf("command=%s\n",buffer);
    return 0;
}

/* When server's version is lower than client side server version,
   upgrade is needed.(to-do)
*  @param
*
*  @return 0: successfully.
*/
int handle_upgrade()
{
    /* 1. Send the upgrade file to server.*/
    /* 2. Send IUCV server update command, it will upgrade server. current socket will be disconnected */
    /* 3. Reconnect with server*/
    /* 4. get the new server's version to make compare(currently not do this step)*/
    return 0;
}


/* Send specified file to IUCV server.
* @param $1: the socket which is used to make IUCV communication to server.
*        $2: the path for the file which need to be sent.
*
* @return 0: if file transport is successful.
*         FILE_TRANSPORT_ERROR: if file transport is failed.
*         errno: if there are any socket connection error.
*/
int send_file_to_server(int sockfd, char *src_path)
{
    char buffer[SMALL_BUFFER_SIZE],md5[SMALL_BUFFER_SIZE];
    char *file_buf;
    FILE *fp = NULL;
    int n_time = 0, n = 0;
    size_t len = n_time = n = 0;

    printf("Will send file %s to IUCV server.\n", src_path);
    file_buf = (char *) malloc(BUFFER_SIZE);
    if((fp = fopen(src_path, "rb"))==NULL)
    {
        syslog(LOG_ERR, "Failed to open file %s for read", src_path);
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"Failed to open the transport file:");
        free(file_buf);
        file_buf = NULL;
        return errno;
    }
    else
    {
        printf("Start to read file\n");
        while (!feof(fp))
        {
            bzero(file_buf, BUFFER_SIZE);
            len = fread(file_buf, sizeof(char), BUFFER_SIZE, fp);
            if ((len = send(sockfd, file_buf, len, 0)) < 0)
            {
                 printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"Failed to send file to serer.");
                 free(file_buf);
                 file_buf = NULL;
                 return errno;
            }
        }
        if (fclose(fp) != 0)
        {
            syslog(LOG_ERR, "ERROR Fail to close sent file after reading: %s\n",strerror(errno));
            return errno;
        }
        fp = NULL;
        free(file_buf);
        file_buf = NULL;

        /* send send_over signal + md5 */
        sprintf(buffer, "md5sum %s",src_path);
        if((fp = popen(buffer, "r"))==NULL)
        {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"Failed to get md5 for file");
            strcpy(buffer, "FILE_SENT_OVER");
            send(sockfd, buffer, strlen(buffer) + 1, 0);
        }
        else
        {
            bzero(buffer,SMALL_BUFFER_SIZE);
            if(fgets(buffer, sizeof(buffer), fp) != NULL)
            {
                buffer[MD5_LENGTH]='\0';
                sprintf(md5,"FILE_SENT_OVER %s",buffer);
            }
            send(sockfd, md5, strlen(md5) + 1, 0);
        }
        /* After finish sending file, wait for the message from server to get the file transport result */   
        printf("Finish sending file, just need to wait for the server's receive respond\n");
        bzero(buffer,SMALL_BUFFER_SIZE);
        recv(sockfd, buffer, SMALL_BUFFER_SIZE, 0);
        if(strcmp(buffer, "FILE_RECEIVED_OK")==0)
        {
            sprintf(buffer,"Transport file %s successfully.\n", src_path);
            printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, buffer);
            return 0;
        }
        else
        {
            sprintf(buffer,"Transport file %s failed.\n", src_path);
            printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, FILE_TRANSPORT_ERROR, buffer);
            return FILE_TRANSPORT_ERROR;
        }
    }
    return 0;
}


/* main function.
* @param $1: the number of param.
*        $2: the params which is input.
*
* @return 0: if file transport is successful.
*         -1: Usage.
*         errno: if there are any socket connection error.
*/
int main(int argc, char *argv[])
{
    int sockfd, portno, n, i;
    struct sockaddr_iucv serv_addr;
    int flags;
    char buffer[BUFFER_SIZE];
    int returncode = 0;

    if (argc < 3)
    {
        if (argc==2 && strcmp(argv[1],"--version")==0)
        {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(0,0,IUCV_CLIENT_VERSION);
            return 0;
        }
        bzero(buffer,BUFFER_SIZE);
        sprintf(buffer, "Usage:\niucvclient [--version]\n\
                              iucvclient server_userid %s src_file_path dst_file_path\n\
                              iucvclient server_userid command [command_parm2, pw:passwd_parm, ...]\n \
                      Pw:passwd_parm is used for password in command, will not show passwd in log file\n",
                      FILE_TRANSPORT);
        printAndLogIUCVserverReturnCodeReasonCodeoutput(USAGE_ERROR,1,buffer);
        return -1;
    }
    /* Create a socket point */
    sockfd = socket(AF_IUCV, SOCK_STREAM, IPPROTO_IP);
    if (sockfd < 0)
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR opening socket:");
        close(sockfd);
        return errno;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.siucv_family = AF_IUCV;
    memcpy(&serv_addr.siucv_user_id, argv[1], 8);
    memcpy(&serv_addr.siucv_name, "OPNCLOUD", 8);

     /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR connecting socket:");
        close(sockfd);
        return errno;
    }
    if((returncode = prepare_commands(buffer, argc, argv)) != 0)
    {
        return returncode;
    }
    /* Send messages to server. */
    n = send(sockfd, buffer, strlen(buffer)+1,0);
    if (n < 0) {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR writing to socket:");
        close(sockfd);
        return errno;
    }
    /* Receive messages from server, to determine what should be done next */
    bzero(buffer,BUFFER_SIZE);
    while (n = recv(sockfd, buffer, BUFFER_SIZE,0) > 0)
    {
        /* if upgrade is needed */
        if(strcmp(buffer, IUCV_SERVER_NEED_UPGRADE) == 0)
        {
            printf("Need to make upgrade!\n");
            if((returncode = handle_upgrade()) != 0)
            {
                return returncode;
            }
            break;
        }
        /* If command is to transport file to VM */
        /* (to-do)later should add a Mutex, when file is transport, command is not be allowed */
        else if(strcmp(argv[2], FILE_TRANSPORT)==0)
        {
            printf("Begin to send file.\n");
            if(n < 0 || strcmp(buffer,READY_TO_RECEIVE) != 0)
            {
                printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"Failed to receive server's response to accept file.");
                close(sockfd);
                return errno;
            }

            if((returncode = send_file_to_server(sockfd, argv[3])) != 0)
            {
                return returncode;
            }
            break;
        }
        /* If command is normal linux command */
        else
        {
            /* Now read server response */
            //printf("result len=%d\n",strlen(buffer));
            printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, buffer);
            if (n < 0)
            {
                printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR reading from socket:");
                close(sockfd);
                return errno;
            }
        }
    }
    close(sockfd);
    return errno;
}
