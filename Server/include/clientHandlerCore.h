#ifndef _CHANDLER_H_
#define _CHANDLER_H_

#include <unistd.h>    //write

#define SERVER_MAX_INPUT_LENGTH 777


#define RECEIVE_DATA_ERROR -1
#define RECEIVE_DATA_OK 1
#define SEND_DATA_ERROR -2
#define SEND_DATA_OK 2
#define CLIENT_DISCONNECT 3

#define CLIENT_ERROR_LENGTH 202
#define CLIENT_OK_LENGTH 200

int clientHandler(int socket);

#endif
 
