/*
 * IBM (C) Copyright 2016, 2016 Eclipse Public License
 * http://www.eclipse.org/org/documents/epl-v10.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netiucv/iucv.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>

/*size define*/
#define SOCKET_TIMEOUT   20
#define BUFFER_SIZE 1024

/*string define*/
#define PATH_FOR_AUTHORIZED_USERID "/etc/iucv_authorized_userid"
//#define PATH_FOR_IUCV_SERVER "/usr/bin/iucvserver"
//#define PATH_FOR_IUCV_SERVER_SERVICE "/etc/init.d/iucvserverd"
#define IUCV_SERVER_VERSION "0.0.0.1"
#define FILE_TRANSPORT "file_transport"
#define PIPE_FIFO_NAME "pipe_fifo"
#define READY_TO_RECEIVE "ready_to_receive"
#define FILE_SENT_OVER "FILE_SENT_OVER"
#define IUCV_SERVER_NEED_UPGRADE "need_upgrade"

/*ERROR defined*/
#define UNAUTHORIZED_ERROR 1
#define CLIENT_USAGE_ERROR 2
#define FILE_TRANSPORT_ERROR 16

/*functions*/
int check_client_authorization(int newsockfd, char *req_userid);
int receive_file_from_client(int newsockfd, char *des_path);
int handle_upgrade();
int server_socket();
int main(int argc,char* argv[]);
