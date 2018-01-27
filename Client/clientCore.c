#include "clientCore.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int createSocket(const char * address, int port);
void configureServerSettings(const char * address, int port, struct sockaddr_in * serverAddr);

int connectToServer(const char * address, int port){
	int socket;

    socket = createSocket(address, port);
    if(socket == SERVER_CONNECTION_ERROR)
    	return SERVER_CONNECTION_ERROR;
    
    return serverHandler(socket);
} 

int createSocket(const char * address, int port){
	int sock;
    struct sockaddr_in serverAddr;
    struct timeval tv; //Timeout
	//Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
        return SERVER_CONNECTION_ERROR;

    //Configure the address and port of server
    configureServerSettings(address,port,&serverAddr);

    //Set the timeout interval
    tv.tv_usec = 0; 
    tv.tv_sec = CLIENT_TIMEOUT; 

    //Set the timeout to the socket
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const struct timeval *)&tv,sizeof(struct timeval)); 
    
    //Connect to server
    if (connect(sock , (struct sockaddr *)&serverAddr , sizeof(serverAddr)) < 0)
        return SERVER_CONNECTION_ERROR;
  	
    return sock;
}

void configureServerSettings(const char * address, int port, struct sockaddr_in * serverAddr){
	//Set the address to any if no address is given
	serverAddr->sin_addr.s_addr = INADDR_ANY;
	if(address != NULL)
		serverAddr->sin_addr.s_addr = inet_addr(address);
	
	serverAddr->sin_family = AF_INET; //Internet address
	serverAddr->sin_port = htons(port); //Port using htons for proper byte order
	memset(serverAddr->sin_zero, 0, sizeof serverAddr->sin_zero); //? Set all bits of the padding field to 0 ?
}