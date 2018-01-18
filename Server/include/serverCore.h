#ifndef _SERVER_CORE_H_
#define _SERVER_CORE_H_

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include "constants.h"
#define SERVER_MAX_QUEUE_REQUEST 7
#define SERVER_TIMEOUT 77
#define SERVER_MAX_INPUT_LENGTH 777

#define SERVER_INIT_ERROR -1
#define SERVER_CHILD_ERROR -2
#define SERVER_DB_ERROR -3

#define SERVER_FOREGROUND 1
#define SERVER_EXIT 0

int initializeServer(const char * address, int port);
int initializeServerForeground(const char * address, int port);

#endif

