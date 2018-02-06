#ifndef _SHANDLER_H_
#define _SHANDLER_H_

#include <unistd.h>    //write
#include "coreStructs.h"

#define HELLO_ERROR -4
#define HELLO_OK 4

#define RESERVE_MEM_ERROR -3
#define RECEIVE_DATA_ERROR -1
#define RECEIVE_DATA_OK 1
#define SEND_DATA_ERROR -2
#define SEND_DATA_OK 2
#define SERVER_DISCONNECTED 3

int sendSimpleCmd(int socket, simpleCommand * simpleCmd);
int sendSimpleMsg(int socket, simpleMessage * simpleMsg);
int sendSimpleMsg_w_bytes(int socket, simpleMessage * simpleMsg, int msg_bytes);
int getDataAndLengthFromServer(int socket, char * * data, int * bytes);

//Test functions
int sendDataToServer(int socket, char * data, int bytes);
int getDataFromServer(int socket, char * buffer, int max_bytes, int * data_size);
int getHelloFromServer(int socket, int isAdmin);

#endif