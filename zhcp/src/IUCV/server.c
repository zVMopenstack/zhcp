#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netiucv/iucv.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>

#define CLIENT_USERID "SONGCONT"
#define SOCKET_TIMEOUT   5
#define BUFFER_SIZE 1024
#define PATH_FOR_AUTHORIZED_USERID "/tmp/authoried_userid"
//#define PATH_FOR_IUCV_SERVER "/usr/bin/iucvserver"
//#define PATH_FOR_IUCV_SERVER_SERVICE "/
#define IUCV_SERVER_VERSION "0.0.0.1-20160623"
#define FILE_TRANSPORT "file_transport"
#define PIPE_FIFO_NAME "pipe_fifo"

/*ERROR defined*/
#define NOT_AUTHORIZED_USERID 1
#define CLIENT_USAGE_ERROR 2

int server_socket() 
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_iucv cli_addr;
    struct sockaddr_iucv serv_addr;
    size_t n, len, on=1;
    char* client_userid=CLIENT_USERID;

    /* First call to socket() function */
    sockfd = socket(AF_IUCV, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        close(sockfd);
        syslog(LOG_ERR, "ERROR opening socket: %s\n",strerror(errno));
        return errno;
    }

    if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        close(sockfd);
   	    syslog(LOG_ERR, "ERROR setsockopt: %s\n",strerror(errno));
        return errno;
    }  
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.siucv_family = AF_IUCV;
    memcpy(serv_addr.siucv_name, "AGENT   ", 8);
    memcpy(serv_addr.siucv_user_id, "", 8);
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        close(sockfd);
        syslog(LOG_ERR, "ERROR on binding: %s\n",strerror(errno));
        return errno;
    }
    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,SOCKET_TIMEOUT);
    clilen = sizeof(cli_addr);
    while(1)
    {
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
	        close(newsockfd);
            close(sockfd);
            syslog(LOG_ERR, "ERROR on accepting: %s\n",strerror(errno));
            return errno;
        }
        /* If connection is established then start communicating */
        bzero(buffer,BUFFER_SIZE);
        n =  recv(newsockfd,buffer, BUFFER_SIZE, 0);
        if (n < 0)
        {
            close(newsockfd);
            close(sockfd);
            syslog(LOG_ERR, "ERROR reading from socket: %s\n",strerror(errno));
            return errno;
        }
        /*check the client useid's authorized*/
        len = strcspn(buffer, " ");
        char tmp[16];
        strncpy(tmp, buffer, len);
        /* authoroized_useid is saved in PATH_FOR_AUTHORIZED_USERID */
        FILE *fp = NULL;
        fp = fopen(PATH_FOR_AUTHORIZED_USERID,"r");
        /* if the file doesn't exist, will create it and save the userid in it 
        * if the file exists, read the value in it and make comparation.*/
        if( NULL == fp)
        {
            fp=fopen(PATH_FOR_AUTHORIZED_USERID,"w");
            if(NULL == fp)
            {
                syslog(LOG_ERR, "ERROR to open %s to write userid.", PATH_FOR_AUTHORIZED_USERID);
            }
            else
            {
                if(fwrite(tmp,strlen(tmp)+1,1,fp)!=1)
                {
                    syslog(LOG_ERR,"ERROR to write userid to %s.",PATH_FOR_AUTHORIZED_USERID);
                 }
                fclose(fp);
                fp = NULL;
            }
        }
        else
        {
            fread(tmp,1,strlen(tmp),fp);
            fclose(fp);
            fp = NULL;
            /* if the userid is not authorized, send error message back*/
            if(strlen(tmp) > 0 && !strncasecmp(tmp,client_userid,len))
            {
                strcpy(buffer,"NOT_AUTHORIZED_USERID");
                send(newsockfd, buffer,strlen(buffer)+1, 0);
                syslog(LOG_ERR, "ERROR %d:IUCV agent only can communicate with specified open cloud user!\n", NOT_AUTHORIZED_USERID);
                close(newsockfd);
                close(sockfd);
                return NOT_AUTHORIZED_USERID;
            }
        }
        if(strlen(buffer) > len)
        {
            strcpy(buffer, buffer+len+1);
            /* for file_transport command, received string should contain the path.
               if user is authrized, just accept the file, or else will pop up the error.
               later should add a Mutex, when file is transport, command is not be allowed.
            */  
            if(strncmp(buffer, FILE_TRANSPORT, strlen(FILE_TRANSPORT))==0)
            {
                strtok(buffer," ");
                char *path = strtok(NULL," ");
                syslog(LOG_INFO,"Will receive and save file %s sending from IUCV client.\n", path);
                while((n = recv(newsockfd,buffer, BUFFER_SIZE, 0)) > 0)
                {
                    fp = fopen(path,"w");
                    if(NULL == fp)
                    {
                        syslog(LOG_ERR, "Failed to open file %s for write", path);
                    }
                    else
                    {
                        syslog(LOG_INFO,"start to write file\n");
                        if((n = fwrite(buffer, strlen(buffer), 1, fp)) < 0)
                        {
                            syslog(LOG_ERR,"Failed to write file to %s", path);
                            strcpy(buffer,"FILE_SEND_ERROR");
                            send(newsockfd,buffer,strlen(buffer)+1,0);
                            fclose(fp);
                            fp = NULL;
                            close(newsockfd);
                            close(sockfd);
                            return errno; 
                        }
                        syslog(LOG_ERR, "write %s to file",buffer);
                        fclose(fp);
                        fp = NULL;
                        /* After finish sending file, send message to client */
                        strcpy(buffer,"FILE_SEND_OK");
                        send(newsockfd,buffer,strlen(buffer)+1,0);
                    }
                }
                if (n < 0)
                {
                    close(newsockfd);
                    close(sockfd);
                    syslog(LOG_ERR, "ERROR reading from socket to get the tranport file: %s\n",strerror(errno));
                    return errno;
                }
                
            }
            else
            {    
                syslog(LOG_INFO,"Will execute the command %s sent from IUCV client.\n", buffer);
                fp = popen(buffer, "r");
                /* Write a response to the client */
                while (fgets(buffer, sizeof(buffer), fp) != NULL)
                {
                    syslog(LOG_ERR,"write %s length %d", buffer, strlen(buffer));
                    buffer[strlen(buffer)] = 0;
                    send(newsockfd, buffer,strlen(buffer)+1, 0);
                }
            }
        }
        /* close */
        pclose(fp);
        close(newsockfd);
    }    
    close(sockfd);
    syslog(LOG_INFO, "Successfully to stop to IUCV server");   
    return 0;
}

int main(int argc,char* argv[])
{
    if(argc==2 && strcmp(argv[1],"--version")==0)
    {  
        printf("%s\n", IUCV_SERVER_VERSION);
        return 0;
    }
    else if(argc==2 && strcmp(argv[1],"start")==0)
    {
        syslog(LOG_ERR,"IUCV server start to work!\n");
        daemon(0,0);
        while(1)
        {
            syslog(LOG_ERR,"Begin a new socket.\n");
            server_socket();
        }
    }
    else if(argc==2 && strcmp(argv[1],"stop")==0)
    {
        
        syslog(LOG_INFO, "Send command stop IUCV server!\n");
        return 0;
    }
    else{
        printf("Usage: iucvserver --version\n");
        return -1;
    }

    return 0;
}
