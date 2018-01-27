#ifndef _SHANDLER_H_
#define _SHANDLER_H_

#include <unistd.h>    //write

#define HELLO_ERROR -4
#define HELLO_OK 4

#define RESERVE_MEM_ERROR -3
#define RECEIVE_DATA_ERROR -1
#define RECEIVE_DATA_OK 1
#define SEND_DATA_ERROR -2
#define SEND_DATA_OK 2
#define SERVER_DISCONNECTED 3

int serverHandler(int socket);

#endif