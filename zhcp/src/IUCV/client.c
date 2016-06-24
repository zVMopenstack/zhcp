#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netiucv/iucv.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define UNAUTHORIZED_ERROR 1
#define USAGE_ERROR 2
#define SOCKET_ERROR 4
#define CMD_EXEC_ERROR 8
#define FILE_TRANSPORT_ERROR 16


#define IUCV_CLIENT_VERSION "0.0.0.1-20160623"
#define FILE_TRANSPORT "file_transport"

int printAndLogIUCVserverReturnCodeReasonCodeoutput(int returncode, int reasoncode, char * message)
{
    char msg[256];
    strcpy(msg,message);
    if(returncode || reasoncode)
    {
        strcat(msg,strerror(reasoncode));
        syslog(LOG_ERR,"%s", msg);
    }
    printf("%s\n", (strlen(message)==0) ? "...Done" : msg);
    return returncode;
}   
int main(int argc, char *argv[]) {
    int sockfd, portno, n, i;
    struct sockaddr_iucv serv_addr;
    struct hostent *server;
    int flags;
    char buffer[BUFFER_SIZE],result[BUFFER_SIZE*10];
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
        return 1;
    }
    /* Create a socket point */
    sockfd = socket(AF_IUCV, SOCK_STREAM, IPPROTO_IP);
    if (sockfd < 0) {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR opening socket:");
        close(sockfd);
        return errno;
    }
   
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.siucv_family = AF_IUCV;
    memcpy(&serv_addr.siucv_user_id, argv[1], 8);
    memcpy(&serv_addr.siucv_name, "AGENT   ", 8);      
    
     /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR connecting socket:");
        close(sockfd);
        return errno;
    }
    /* Now ask for a message from the user, this message
    * will be read by server
    */
    /* Need to add userid to make authorized, if not a opencloud user, request will be refused */
    bzero(buffer,BUFFER_SIZE);
    FILE *fp = popen("vmcp q userid", "r");
    if(fgets(buffer,sizeof(buffer),fp)!=NULL)
    {
        strcpy(buffer, strtok(buffer," "));
    }
    else
    {
        printAndLogIUCVserverReturnCodeReasonCodeoutput(UNAUTHORIZED_ERROR, errno,"ERROR failed to get userid:");
        return errno;    
    }
    /* Send message to server */
    for (i = 2; i < argc;i++)
    {

        strcat(buffer, " ");
        strcat(buffer, argv[i]);

    }
    printf("command=%s\n",buffer);
    /* If command is to tranport file to VM */
    /* (to-do)later should add a Mutex, when file is transport, command is not be allowed */
    if(strcmp(argv[2], FILE_TRANSPORT)==0)
    {
        printf("Will send file %s to IUCV server.\n", argv[3]);
        syslog(LOG_INFO,"Will send file %s to IUCV server.\n", argv[3]);
        fp = fopen(argv[3], "r");
        if(NULL == fp)
        {
            syslog(LOG_ERR, "Failed to open file %s for read", argv[3]);
        }
        else
        {
            printf("Start to read file\n");
            while((n = fread(buffer, strlen(buffer), 1, fp)) > 0)
            {
                printf("n=%s\n");
                n = send(sockfd, buffer, strlen(buffer),0);
                if (n < 0) 
                {
                    printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR writing the transport file to socket:");
                    close(sockfd);
                    return errno;
                }
            }
            if(n < 0)
            {
                syslog(LOG_ERR,"Failed to read file to %s", argv[2]);
                fclose(fp);
                fp = NULL;
                close(sockfd);
                return errno;
            }       
            fclose(fp);
            fp = NULL;
            /* After finish sending file, wait for the message from server to get the file transport result */
            printf("Finish sending file, just need to wait for the server's receive respond\n");
            bzero(buffer,BUFFER_SIZE);
            recv(sockfd, buffer, strlen(buffer)+1, 0);
            if(strcmp(buffer, "FILE_SEND_OK")==0)
            {
                sprintf(buffer,"Transport file %s successfully", argv[3]);
                printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, buffer);
            }
            else
            {
                sprintf(buffer,"Transport file %s failed", argv[3]);
                printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, FILE_TRANSPORT_ERROR, buffer);
            }
        }
          
    }
    /* If command is normal linux command */
    else
    {
        n = send(sockfd, buffer, strlen(buffer),0);
        if (n < 0) {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR writing to socket:");
            close(sockfd);
            return errno;
        }
       
        /* Now read server response */
        bzero(buffer,BUFFER_SIZE);
        bzero(result,BUFFER_SIZE*10);
        /* the firstline is to get return value for command */
        n = recv(sockfd, buffer, 255,0);
        if (n < 0) 
        {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR reading from socket:");
            close(sockfd);
            return errno;
        }
        else
        {
            strcpy(result, buffer);
        }        
        while (n = recv(sockfd, buffer, 255,0) > 0)
        {  
            strcat(result,buffer);
        }    
        printf("result len=%d\n",strlen(result));
        printAndLogIUCVserverReturnCodeReasonCodeoutput(0, 0, result);
       
        if (n < 0)
        {
            printAndLogIUCVserverReturnCodeReasonCodeoutput(SOCKET_ERROR, errno,"ERROR reading from socket:");
            close(sockfd);
            return errno;
        }
    }
    close(sockfd);	
    return errno;
}
