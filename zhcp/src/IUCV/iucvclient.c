/*
 * IBM (C) Copyright 2016, 2016 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */
#include "../IUCV/iucvclient.h"


/* print log to console and save it to /var/log/messages.
* @param $1: the return value for client.
*        $2: the reason for the details.
*        $3: results.
*        $4: whether to print error reason which is get from errno. 0 not print info, 1 print the error info.
*
* @return returncode:
*/
int printAndLogIUCVserverReturnCodeReasonCodeoutput(int returncode, int reasoncode, char * message, int with_strerr)
{
    if(returncode || reasoncode)
    {
        syslog(LOG_ERR,"%s  Return code %d, Reason code %d", message, returncode, reasoncode);
        if(reasoncode && with_strerr)
        {
            printf("%s\n%s\nReturn code %d, Reason code %d.\n", message, strerror(reasoncode), returncode, reasoncode);
            return returncode;
        }
        printf("%s\nReturn code %d, Reason code %d.\n", message, returncode, reasoncode);
    }
    else
    {
        syslog(LOG_INFO,"%s", message);
        printf("%s", message);
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
    char user_buf[32], err_buf[256];
    int i = 0;

    bzero(buffer,BUFFER_SIZE);
    FILE *fp = popen("vmcp q userid", "r");
    if(fgets(user_buf,sizeof(user_buf),fp) != NULL)
    {
        strcpy(buffer, strtok(user_buf," "));
        pclose(fp);
        fp = NULL;
    }
    else
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(UNAUTHORIZED_ERROR, errno,"ERROR failed to get userid:", 0);
        pclose(fp);
        fp = NULL;
        return UNAUTHORIZED_ERROR;
    }
    /* 2. Get the IUCV server's version which exists on zhcp
       This is used for IUCV server's upgrade, when the server's version which installed on VM is lower,
       upgrade is needed.
    */
    /* Check whether the IUCV server exist */
    if(fopen(FILE_PATH_IUCV_SERVER,"r") == NULL)
    {
        sprintf(err_buf, "ERROR: can't find IUCV server in path %s, please copy file to the path and try again.", FILE_PATH_IUCV_SERVER);
        printAndLogIUCVserverReturnCodeReasonCodeoutput(IUCV_FILE_NOT_EXIST, errno, err_buf, 1);
        return IUCV_FILE_NOT_EXIST;
    }
    char tmp_buf[BUFFER_SIZE];
    sprintf(tmp_buf, "%s --version", FILE_PATH_IUCV_SERVER);
    fp = popen(tmp_buf, "r");
    if(fgets(tmp_buf, sizeof(tmp_buf), fp) != NULL)
    {
        //printf(tmp_buf);
        strcat(buffer, " ");
        strcat(buffer, tmp_buf);
        buffer[strlen(buffer)-1]='\0';//command has an enter;
    }
    pclose(fp);
    fp = NULL;

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
    //printf("command=%s\n",buffer);
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


/* Send specified file (NOT support to transport folder) to IUCV server.
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
        sprintf(buffer, "Failed to open file %s for read.", src_path);
        printAndLogIUCVserverReturnCodeReasonCodeoutput(FILE_TRANSPORT_ERROR, errno, buffer, 1);
        free(file_buf);
        file_buf = NULL;
        return FILE_TRANSPORT_ERROR;
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
                 printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno, "Failed to send file to serer.", 1);
                 free(file_buf);
                 file_buf = NULL;
                 fclose(fp);
                 fp = NULL;
                 return SOCKET_ERROR;
            }
        }
        if (fclose(fp) != 0)
        {
            syslog(LOG_ERR, "ERROR Fail to close sent file after reading: %s.\n",strerror(errno));
        }
        fp = NULL;
        free(file_buf);
        file_buf = NULL;

        /* send send_over signal + md5 */
        sprintf(buffer, "md5sum %s",src_path);
        if((fp = popen(buffer, "r"))==NULL)
        {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(FILE_TRANSPORT_ERROR, errno,"Failed to get md5 for file.", 1);
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
        pclose(fp);
        fp = NULL;
        /* After finish sending file, wait for the message from server to get the file transport result */
        printf("Finish sending file, just need to wait for the server's receive respond\n");
        bzero(buffer,SMALL_BUFFER_SIZE);
        recv(sockfd, buffer, SMALL_BUFFER_SIZE, 0);
        if(strcmp(buffer, "FILE_RECEIVED_OK")==0)
        {
            sprintf(buffer,"Transport file %s successfully.\n", src_path);
            printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, buffer, 0);
            return 0;
        }
        else
        {
            sprintf(buffer,"Transport file %s failed. %s.", src_path, buffer);
            printAndLogIUCVserverReturnCodeReasonCodeoutput(FILE_TRANSPORT_ERROR, FILE_TRANSPORT_ERROR, buffer, 0);
            return FILE_TRANSPORT_ERROR;
        }
    }
    return 0;
}


/* main function.
* @param $1: the number of param.
*        $2: the params which is input.
*
* @return 0: command are executed successfully.
*         1: UNAUTHORIZED_ERROR
*         2: USAGE_ERROR
*         4: SOCKET_ERROR
*         8: CMD_EXEC_ERROR
*        16: FILE_TRANSPORT_ERROR
*        32: IUCV_FILE_NOT_EXIST
*        -1: Usage:
*/
int main(int argc, char *argv[])
{
    int sockfd, portno, n, i;
    struct sockaddr_iucv serv_addr;
    int flags;
    char buffer[BUFFER_SIZE],result[BUFFER_SIZE],cmd_rc[8];
    int returncode = 0;
    char* pos = NULL;

    if (argc < 3)
    {
        if (argc==2 && strcmp(argv[1],"--version")==0)
        {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, IUCV_CLIENT_VERSION, 0);
            return 0;
        }
        bzero(buffer,BUFFER_SIZE);
        sprintf(buffer, "Usage:\niucvclnt [--version]\n\
                              iucvclnt server_userid %s src_file_path/filename dst_file_path/filename\n\
                              iucvclnt server_userid command [command_parm2, ...]",
                      FILE_TRANSPORT);
        /*(to-do) add pars password to not show it in log file.
        sprintf(buffer, "Usage:\niucvclnt [--version]\n\
                              iucvclnt server_userid %s src_file_path dst_file_path\n\
                              iucvclnt server_userid command [command_parm2, pw:passwd_parm, ...]\n \
                      Pw:passwd_parm is used for password in command, will not show passwd in log file\n",
                      FILE_TRANSPORT);
        */
        printAndLogIUCVserverReturnCodeReasonCodeoutput(USAGE_ERROR, 1, buffer, 0);
        return -1;
    }
    /* Create a socket point */
    sockfd = socket(AF_IUCV, SOCK_STREAM, IPPROTO_IP);
    if (sockfd < 0)
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno, "ERROR opening socket:", 1);
        close(sockfd);
        return SOCKET_ERROR;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.siucv_family = AF_IUCV;
    memcpy(&serv_addr.siucv_user_id, argv[1], 8);
    memcpy(&serv_addr.siucv_name, "OPNCLOUD", 8);

     /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno, "ERROR connecting socket:", 1);
        close(sockfd);
        return SOCKET_ERROR;
    }
    if((returncode = prepare_commands(buffer, argc, argv)) != 0)
    {
        close(sockfd);
        return returncode;
    }
    /* Send messages to server. */
    n = send(sockfd, buffer, strlen(buffer)+1,0);
    if (n < 0) {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno, "ERROR writing to socket:", 1);
        close(sockfd);
        return SOCKET_ERROR;
    }
    /* Receive messages from server, to determine what should be done next */
    bzero(buffer,BUFFER_SIZE);
    while (n = recv(sockfd, buffer, BUFFER_SIZE,0) > 0)
    {
        //printf("result =%s\n",buffer);
        /* Check the result is NOT_AUTHORIZED_USERID */
        if(strncmp(buffer, "UNAUTHORIZED_ERROR", strlen("UNAUTHORIZED_ERROR")) == 0)
        {
            bzero(result, 8);
            strcpy(result, strtok(buffer, "#"));
            bzero(cmd_rc, 8);
            strcpy(cmd_rc, strtok(NULL,"#"));
            returncode = (atoi(cmd_rc) == UNAUTHORIZED_ERROR) ? 0 : atoi(cmd_rc);
            printAndLogIUCVserverReturnCodeReasonCodeoutput(UNAUTHORIZED_ERROR, returncode, buffer, 0);
            close(sockfd);
            return UNAUTHORIZED_ERROR;
        }
        /* if upgrade is needed */
        if(strcmp(buffer, IUCV_SERVER_NEED_UPGRADE) == 0)
        {
            printf("Need to make upgrade!\n");
            if((returncode = handle_upgrade()) != 0)
            {
                close(sockfd);
                return returncode;
            }
            break;
        }
        /* If command is to transport file to VM */
        /* (to-do)later should add a Mutex, when file is transport, command is not be allowed */
        if(strcmp(argv[2], FILE_TRANSPORT)==0)
        {
            /* If the source path is a folder, return error */
            struct stat st;
            stat(argv[3], &st);
            if((st.st_mode & S_IFDIR) == S_IFDIR)
            {
                sprintf(buffer, "The source path %s should include the file name.", argv[3]);
                printAndLogIUCVserverReturnCodeReasonCodeoutput(FILE_TRANSPORT_ERROR, 1, buffer, 0);
                close(sockfd);
                return FILE_TRANSPORT_ERROR;
            }
            printf("Begin to send file.\n");
            if(n < 0 || strcmp(buffer,READY_TO_RECEIVE) != 0)
            {
                sprintf(result, "Server can't receive file. Reason: %s.", buffer);
                printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno, result, 0);
                close(sockfd);
                return SOCKET_ERROR;
            }
            if((returncode = send_file_to_server(sockfd, argv[3])) != 0)
            {
                close(sockfd);
                return returncode;
            }
            break;
        }
        /* If command is normal linux command */
        else
        {
            /* Now read server response */
            //printf("result =%s\n",buffer);
            /* check the command return code */
            pos = strstr(buffer, "iucvcmdrc=");
            if(pos == NULL)
            {
                strcpy(cmd_rc, "0");
                bzero(result,BUFFER_SIZE);
                strcpy(result, buffer);
            }
            else
            {
                bzero(cmd_rc, 8);
                strncpy(cmd_rc, pos+strlen("iucvcmdrc="), 8);
                bzero(result,BUFFER_SIZE);
                strncpy(result, buffer, strlen(buffer)-strlen(pos));
            }
            //printf("rc=%d,result=%s,buffer=%s",atoi(cmd_rc),result,buffer);
            if(atoi(cmd_rc) == 0)
            {
                printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, result, 0);
                /* If message has sent over, close the socket immediately to enhance performance */
                if(pos != NULL)
                {
                    close(sockfd);
                    return 0;
                }
            }
            else
            {
                printAndLogIUCVserverReturnCodeReasonCodeoutput(CMD_EXEC_ERROR, atoi(cmd_rc), result, 0);
                close(sockfd);
                return CMD_EXEC_ERROR;
            }
            if (n < 0)
            {
                printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno, "ERROR reading from socket:", 1);
                close(sockfd);
                return SOCKET_ERROR;
            }
        }
    }
    close(sockfd);
    return 0;
}
