#ifndef _CHANDLER_H_
#define _CHANDLER_H_

#include <unistd.h>    //write

#define HELLO_ERROR -4
#define HELLO_OK 4

#define RECEIVE_DATA_ERROR -1
#define RECEIVE_DATA_OK 1
#define SEND_DATA_ERROR -2
#define SEND_DATA_OK 2
#define CLIENT_DISCONNECT 3

int clientHandler(int socket);

#endif
 
