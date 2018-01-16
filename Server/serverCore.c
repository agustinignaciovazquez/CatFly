/* Server Core */

#include "serverCore.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen

#define _SEMAPHORE_CHILD_NAME_ "CHILD_SEMAPHORE_SRV"

int createServerSocket(const char * address, int port);
void clientHandler(int socket);

int initializeServer(){
	int incomingSocket, acceptSocket, pid;
	struct sockaddr_in clientAddr;
	socklen_t addr_size;
	struct timeval tv; //Timeout
	sem_t * childSemaphore;

	incomingSocket = createServerSocket(SERVER_ADDR,SERVER_PORT);
	if(incomingSocket == SERVER_INIT_ERROR)
		return SERVER_INIT_ERROR;
	 
	addr_size = sizeof(clientAddr);

	//Set the timeout interval
	tv.tv_usec = 0; 
	tv.tv_sec = SERVER_TIMEOUT; 

	//Open the childs semaphore
	childSemaphore = openChildsSemaphore(_SEMAPHORE_CHILD_NAME_);

	while(1){
		acceptSocket = accept(incomingSocket, (struct sockaddr *) &clientAddr,&addr_size);
		//Set the timeout to the socket
		setsockopt(acceptSocket, SOL_SOCKET, SO_RCVTIMEO, (const struct timeval *)&tv,sizeof(struct timeval)); 
		if(acceptSocket < 0){
			printf("Error: Accepting incoming connection \n");
		}else{
			sem_wait(childSemaphore);
			pid = fork();
			if(pid < 0){
				printf("Error: Fork failed \n");
				exit(SERVER_INIT_ERROR);			
			}
			if(pid == 0){
				//Child process
				close(incomingSocket);
				printf("Client connected\n");
				clientHandler(acceptSocket);
				close(acceptSocket);
				printf("Client disconnected\n");
				sem_post(childSemaphore);
				exit(SERVER_EXIT);
			}else{
				close(acceptSocket);
			}
		}
	}
	return SERVER_EXIT;
}

void clientHandler(int socket){
	int n;
	char buffer[256];
	bzero(buffer,256);
	while(1){
		n = read(socket,buffer,255);
		if(n < 0){
			//Error reading from pipe
			return;
		}
		printf("%s",buffer);
	}
	return;
}

int createServerSocket(const char * address, int port){
	int incomingSocket;
	struct sockaddr_in serverAddr;
	int enabled = 1;

	//Create socket w/ Internet Domain, Stream Socket, TCP (Default)
	incomingSocket = socket(PF_INET,SOCK_STREAM,0);

	//Set socket to allow reuse of local addresses
	if (setsockopt(incomingSocket, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) < 0){
    	printf("Error: setsockopt(SO_REUSEADDR) failed\n");
	}

	//Configure the Server settings
	serverAddr.sin_family = AF_INET; //Internet address
	serverAddr.sin_port = htons(port); //Port using htons for proper byte order
	serverAddr.sin_addr.s_addr = inet_addr(address);
	memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero); //? Set all bits of the padding field to 0 ?
	
	if((bind(incomingSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) < 0){
		printf("Error: Binding failed in address %s \n", address);
		return SERVER_INIT_ERROR;
	}

	if((listen(incomingSocket,SERVER_MAX_QUEUE_REQUEST)) < 0){
		printf("Error: Listening failed in port %d \n", port);
		return SERVER_INIT_ERROR;
	}
	printf("Listening on port %d \n",port);
	return incomingSocket;
}

int main(int argc , char *argv[]){
  	initializeServer();
    return 0;
}   
