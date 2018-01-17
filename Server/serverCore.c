/* Server Core */

#include "serverCore.h"
#include "commandParser.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _SEMAPHORE_CHILD_NAME_ "CHILD_SEMAPHORE_SRV"

int createServerSocket(const char * address, int port);
void clientHandler(int socket);
void configureServerSettings(const char * address, int port, struct sockaddr_in * serverAddr);

int initializeServer(const char * address, int port){
	int incomingSocket, acceptSocket, pid;
	struct sockaddr_in clientAddr;
	socklen_t addr_size;
	struct timeval tv; //Timeout
	sem_t * childSemaphore;

	incomingSocket = createServerSocket(address, port);
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
				exit(SERVER_CHILD_ERROR);			
			}
			if(pid == 0){
				//Child process
				printf("Client connected\n");
				close(incomingSocket);
				
				clientHandler(acceptSocket);

				printf("Client disconnected\n");
				close(acceptSocket);
				sem_post(childSemaphore);
				exit(SERVER_EXIT); //end proccess and send ok signal to parent ps
			}else{
				close(acceptSocket);
			}
		}
	}
	return SERVER_EXIT;
}

void clientHandler(int socket){
	int read_size, write_size, response_size;
	char read_buffer[SERVER_MAX_INPUT_LENGTH];
	char * response_buffer;

	while(1){
		//Fill buffer with zeros
		bzero(read_buffer,SERVER_MAX_INPUT_LENGTH); // not neccessary?

		//Receive data from socket
		read_size = recv(socket,read_buffer,SERVER_MAX_INPUT_LENGTH-1,0);
		if(read_size == 0)
			return; //client disconnected
		if(read_size < 0){
			printf("Error: Reading from Socket\n");
			return;
		}

		response_buffer = getResponse(read_buffer, read_size, &response_size);
		if(response_buffer == NULL){
			printf("Error: Parsing request ... forcing client to disconnect \n");
			return;
		}

		write_size = write(socket, response_buffer, response_size);
		if(write_size < 0){
			printf("Error: Writing to Socket\n");
			return;
		}
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
	configureServerSettings(address, port, &serverAddr);
	
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
  
void configureServerSettings(const char * address, int port, struct sockaddr_in * serverAddr){
	serverAddr->sin_addr.s_addr = inet_addr(address);
	serverAddr->sin_family = AF_INET; //Internet address
	serverAddr->sin_port = htons(port); //Port using htons for proper byte order
	memset(serverAddr->sin_zero, 0, sizeof serverAddr->sin_zero); //? Set all bits of the padding field to 0 ?
}