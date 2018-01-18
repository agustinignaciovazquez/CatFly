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
void sigchld_handler(int signal);
void sigterm_handler(int signal);

int incomingSocket, acceptSocket; //Global variables so it can be used in signalHandler for signal SIGTERM

int initializeServerForeground(const char * address, int port){
	int p;

	p = fork();
	if(pid < 0){
		fprintf(stderr,"Error: Fork failed \n");
		return SERVER_INIT_ERROR;			
	}
	
	if(pid == 0){
		//Daemon child
		initializeServer(address,port);
	}

	return SERVER_EXIT;
}

int initializeServer(const char * address, int port){
	int pid;
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

	//Prevent ended children from becoming zombies 
	signal(SIGCHLD, sigchld_handler); 
	//handle KILL signal
    signal(SIGTERM, sigterm_handler); 

	//Open the childs semaphore
	childSemaphore = openChildsSemaphore(_SEMAPHORE_CHILD_NAME_);

	while(1){
		acceptSocket = accept(incomingSocket, (struct sockaddr *) &clientAddr,&addr_size);
		//Set the timeout to the socket
		setsockopt(acceptSocket, SOL_SOCKET, SO_RCVTIMEO, (const struct timeval *)&tv,sizeof(struct timeval)); 
		if(acceptSocket < 0){
			fprintf(stderr,"Error: Accepting incoming connection \n");
		}else{
			sem_wait(childSemaphore);
			pid = fork();
			if(pid < 0){
				fprintf(stderr,"Error: Fork failed \n");
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

/* Handle finished child process (Source: https://github.com/kklis/proxy/blob/master/proxy.c;)*/
void sigchld_handler(int signal) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

/* Handle term signal */
void sigterm_handler(int signal) {
    close(acceptSocket);
    close(incomingSocket);
    exit(SERVER_EXIT);
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
			fprintf(stderr,"Error: Reading from Socket\n");
			return;
		}

		response_buffer = getResponse(read_buffer, read_size, &response_size);
		if(response_buffer == NULL){
			fprintf(stderr,"Error: Parsing request ... forcing client to disconnect \n");
			return;
		}

		write_size = write(socket, response_buffer, response_size);
		if(write_size < 0){
			fprintf(stderr,"Error: Writing to Socket\n");
			return;
		}
	}

	return;
}

int createServerSocket(const char * address, int port){
	int serverSocket;
	struct sockaddr_in serverAddr;
	int enabled = 1;

	//Create socket w/ Internet Domain, Stream Socket, TCP (Default)
	serverSocket = socket(PF_INET,SOCK_STREAM,0);

	//Set socket to allow reuse of local addresses
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) < 0){
    	fprintf(stderr,"Error: setsockopt(SO_REUSEADDR) failed\n");
	}

	//Configure the Server settings
	configureServerSettings(address, port, &serverAddr);
	
	if((bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) < 0){
		fprintf(stderr,"Error: Binding failed in address %s \n", address);
		return SERVER_INIT_ERROR;
	}

	if((listen(serverSocket,SERVER_MAX_QUEUE_REQUEST)) < 0){
		fprintf(stderr,"Error: Listening failed in port %d \n", port);
		return SERVER_INIT_ERROR;
	}

	printf("Listening on port %d \n",port);
	return serverSocket;
}
  
void configureServerSettings(const char * address, int port, struct sockaddr_in * serverAddr){
	serverAddr->sin_addr.s_addr = inet_addr(address);
	serverAddr->sin_family = AF_INET; //Internet address
	serverAddr->sin_port = htons(port); //Port using htons for proper byte order
	memset(serverAddr->sin_zero, 0, sizeof serverAddr->sin_zero); //? Set all bits of the padding field to 0 ?
}