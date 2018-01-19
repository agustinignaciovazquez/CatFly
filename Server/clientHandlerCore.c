#include "clientHandlerCore.h"
#include "commandParser.h"
#include "semaphores.h"
#include "constants.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getDataFromClient(int socket, char * buffer, int max_bytes, int * data_size);
int sendDataToClient(int socket, char * data, int bytes);
int sendDataAndLengthToClient(int socket, char * data, int bytes);
int verifyResponseFromClient(const char * data, int bytes, int responseID);
int veryfyLengthResponse(const char * data, int bytes);

int clientHandler(int socket){
	int con_status;
	int read_size, write_size, response_size, response_status;
	char read_buffer[SERVER_MAX_INPUT_LENGTH];
	char * response_buffer;

	while(TRUE){
		//Receive data from socket
		con_status = getDataFromClient(socket,read_buffer,SERVER_MAX_INPUT_LENGTH,&read_size);
		if(con_status != RECEIVE_DATA_OK){
			return con_status;
		}
		
		response_status = parseRequest(read_buffer, read_size, &response_buffer, &response_size);
		if(response_status == PARSE_ERROR){
			con_status = CLIENT_DISCONNECT;
			fprintf(stderr,"Error: Parsing request ... possible hack attempt ... forcing client to disconnect \n");
			return con_status;
		}

		//Send data to client
		con_status = sendDataAndLengthToClient(socket,response_buffer,response_size);
		fprintf(stderr,"statyus %d \n", con_status);
		if(con_status != SEND_DATA_OK && con_status != CLIENT_ERROR_LENGTH){ //keep alive if client refuses to receive the length 
			return con_status;
		}
	}

	return con_status;
} 

int getDataFromClient(int socket, char * buffer, int max_bytes, int * data_size){
	int read_size;

	read_size = recv(socket,buffer,max_bytes,0);
	if(read_size == 0)
		return CLIENT_DISCONNECT; //client disconnected
	if(read_size < 0){
		fprintf(stderr,"Error: Reading from Socket ... Possible Timeout\n");
		return RECEIVE_DATA_ERROR;
	}

	//Data received correctly
	*data_size = read_size;
	return RECEIVE_DATA_OK;
}

int sendDataToClient(int socket, char * data, int bytes){
	int write_size = 0;

	do{
		write_size = write(socket, data, bytes);
		if(write_size <= 0){
			fprintf(stderr,"Error: Writing to Socket\n");
			return SEND_DATA_ERROR;
		}
	}while(write_size < bytes); //write return value may at times be less than the specified nbytes (returns the number of bytes successfully)

	return SEND_DATA_OK;
}

int sendDataAndLengthToClient(int socket, char * data, int bytes){
	//do we need mutex here?
	int status, read_size;
	char buffer[SERVER_MAX_INPUT_LENGTH];

	//Convert int to char *
	char bytes_aux[sizeof(bytes)];
	memcpy(bytes_aux, &bytes, sizeof(bytes));
	
	status = sendDataToClient(socket, bytes_aux, sizeof(bytes));
	if(status != SEND_DATA_OK)
		return status;
	
	status = getDataFromClient(socket,buffer,SERVER_MAX_INPUT_LENGTH,&read_size);
	if(status != RECEIVE_DATA_OK)
		return status;

	status = veryfyLengthResponse(buffer,read_size);
	if(status != CLIENT_OK_LENGTH)
		return status;

	status = sendDataToClient(socket,data,bytes);
	return status;
}

int verifyResponseFromClient(const char * data, int bytes, int responseID){
	char r;

	if(bytes < sizeof(char))
		return CLIENT_ERROR_LENGTH;
	r = (char)*data;

	return (r == responseID)? CLIENT_OK_LENGTH : CLIENT_ERROR_LENGTH;
}

int veryfyLengthResponse(const char * data, int bytes){
	return verifyResponseFromClient(data,bytes,CLIENT_OK_LENGTH);
}