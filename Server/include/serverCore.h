#ifndef _SERVER_CORE_H_
#define _SERVER_CORE_H_

#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT  7777
#define SERVER_TIMEOUT 30
#define SERVER_MAX_QUEUE_REQUEST 7

#define SERVER_INIT_ERROR -1
#define SERVER_EXIT 0
int initializeServer();
#endif

