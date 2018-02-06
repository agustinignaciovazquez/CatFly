#ifndef _CLIENT_CORE_H_
#define _CLIENT_CORE_H_

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr

#define CLIENT_TIMEOUT 7

#define SERVER_CONNECTION_ERROR -7

/* using http://www.binarytides.com/server-client-example-c-sockets-linux/ part of code */
int connectToServer(const char * address, int port);

//Tests usage
int createSocket(const char * address, int port);
#endif 
