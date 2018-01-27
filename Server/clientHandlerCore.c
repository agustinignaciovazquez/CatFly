#include "clientHandlerCore.h"
#include "commandParser.h"
#include "semaphores.h"
#include "serializeManager.h"
#include "coreStructs.h"
#include "constants.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getDataFromClient(int socket, char * buffer, int max_bytes, int * data_size);
int sendDataToClient(int socket, char * data, int bytes);
int sendDataAndLengthToClient(int socket, char * data, int bytes);
int getHelloFromClient(int socket, int * isAdmin);

int clientHandler(int socket){
	int con_status;
	int read_size, response_status, response_size = 0;
	int isAdmin;
	char read_buffer[SERVER_MAX_INPUT_LENGTH];
	char * response_buffer = NULL;

	con_status = getHelloFromClient(socket, &isAdmin);
	if(con_status != HELLO_OK)
		return con_status;
	while(TRUE){
		//Receive data from socket
		con_status = getDataFromClient(socket,read_buffer,SERVER_MAX_INPUT_LENGTH,&read_size);
		if(con_status != RECEIVE_DATA_OK){
			return con_status;
		}
		
		response_status = parseRequest(read_buffer, read_size, &response_buffer, &response_size, isAdmin);
		//disconnect only if request it or parse error (TCP protocol ensures data is received correctly)
		if(response_status == PARSE_ERROR || response_status == RESPONSE_OK_AND_DISCONNECT){ 
			con_status = CLIENT_DISCONNECT;
			if(response_status == PARSE_ERROR)
				fprintf(stderr,"Error: Parsing request ... possible hack attempt ... forcing client to disconnect \n");
			return con_status;
		}
		if(response_status == RESPONSE_OK){
			//Send data to client
			con_status = sendDataAndLengthToClient(socket,response_buffer,response_size);
			freeSerialized(response_buffer);
			if(con_status != SEND_DATA_OK && con_status != RESPONSE_NO){ //keep alive if client refuses to receive the length 
				return con_status;
			}
		}
	}

	return con_status;
} 

int getHelloFromClient(int socket, int * isAdmin){
	int status, bytes;
	char * bytes_aux, data[SERVER_MAX_INPUT_LENGTH];
	simpleCommand simpleCmd = {.command = HELLO_CODE_CMD, .extra = SERVER_HELLO_OK};

	//Get hello from client
	status = getDataFromClient(socket,data,SERVER_MAX_INPUT_LENGTH,&bytes);
	if(status != RECEIVE_DATA_OK)
		return status;
	
	status = verifyHelloResponse(data, bytes);
	if(status != RESPONSE_OK){
		//Verify admin secret code
		status = verifyHelloAdminResponse(data, bytes);
		*isAdmin = (status == RESPONSE_OK)? TRUE:FALSE;
		if(isAdmin == FALSE)
			return HELLO_ERROR;
	}
	#ifdef DEBUG
		printf("Received hello OK from client\n");
	#endif
	//Say hello to client
	bytes_aux = serializeSimpleCommand(&simpleCmd, &bytes);
	if(bytes_aux == NULL)
		return SEND_DATA_ERROR;

	status = sendDataToClient(socket, bytes_aux, bytes);
	freeSerialized(bytes_aux);
	if(status != SEND_DATA_OK)
		return status;

	return HELLO_OK;
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
	int writed, write_size = 0;

	do{
		writed = write(socket, data + write_size, bytes);
		if(writed <= 0){
			fprintf(stderr,"Error: Writing to Socket\n");
			return SEND_DATA_ERROR;
		}
		write_size += writed;
	}while(write_size < bytes); //write return value may at times be less than the specified nbytes (returns the number of bytes successfully)

	return SEND_DATA_OK;
}

int sendDataAndLengthToClient(int socket, char * data, int bytes){
	//do we need mutex here?
	int status, read_size, cmd_size;
	char buffer[SERVER_MAX_INPUT_LENGTH];
	char * bytes_aux;

	simpleCommand simpleCmd = {.command = LENGTH_CODE_CMD, .extra = bytes};
	
	//Create the request
	
	bytes_aux = serializeSimpleCommand(&simpleCmd, &cmd_size);
	if(bytes_aux == NULL)
		return SEND_DATA_ERROR;

	status = sendDataToClient(socket, bytes_aux, cmd_size);
	freeSerialized(bytes_aux);
	if(status != SEND_DATA_OK)
		return status;
	
	status = getDataFromClient(socket, buffer, SERVER_MAX_INPUT_LENGTH, &read_size);
	if(status != RECEIVE_DATA_OK)
		return status;

	status = veryfyLengthResponse(buffer,read_size);
	if(status != RESPONSE_OK)//Client refuses to receive or parse error
		return status;

	status = sendDataToClient(socket,data,bytes);


	return status;
}

