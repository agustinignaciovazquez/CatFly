#include "serverHandlerCore.h" 
#include "coreStructs.h"
#include "constants.h"
#include "serializeManager.h"
#include "deserializeManager.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getDataFromServer(int socket, char * buffer, int max_bytes, int * data_size);
int sendDataToServer(int socket, char * data, int bytes);
int getDataAndLengthFromServer(int socket, char * * data, int * bytes);
int getHelloFromServer(int socket, int isAdmin);

int serverHandler(int socket){
	int con_status;
	int isAdmin;
	int read_size, send_size;
	char * send_buffer;
	char * read_buffer;
	isAdmin = TRUE; //TODO CHANGE LATER
	con_status = getHelloFromServer(socket, isAdmin);
	if(con_status != HELLO_OK)
		return HELLO_ERROR;
	while(TRUE){
		send_buffer = "A";
		send_size = strlen(send_buffer);

		//Send req to server
		con_status = sendDataToServer(socket, send_buffer, send_size);
		if(con_status != SEND_DATA_OK){
			return con_status;
		}
		//free(read_buffer);
		//Try to get length and response of prev req
		con_status = getDataAndLengthFromServer(socket,&read_buffer,&read_size);
		if(con_status != RECEIVE_DATA_OK && con_status != RESERVE_MEM_ERROR){ //keep alive if reserve mem fails
			return con_status;
		}
		if(con_status == RECEIVE_DATA_OK){
			printf("%s \n", read_buffer);
			free(read_buffer);
		}
	}

	return con_status;
}

int getHelloFromServer(int socket, int isAdmin){
	int status, bytes;
	char * bytes_aux, data[SERVER_MAX_INPUT_LENGTH];
	simpleCommand simpleCmd = {.command = HELLO_CODE_CMD, .extra = CLIENT_HELLO_OK};

	if(isAdmin == TRUE){
		simpleCmd.command = HELLO_ADMIN_CODE_CMD;
		simpleCmd.extra = CLIENT_ADMIN_HELLO_OK;
	}

	//Say hello to server
	bytes_aux = serializeSimpleCommand(&simpleCmd, &bytes);
	if(bytes_aux == NULL)
		return SEND_DATA_ERROR;

	status = sendDataToServer(socket, bytes_aux, bytes);
	freeSerialized(bytes_aux);
	if(status != SEND_DATA_OK)
		return status;

	//Get hello from server
	status = getDataFromServer(socket,data,SERVER_MAX_INPUT_LENGTH,&bytes);
	if(status != RECEIVE_DATA_OK)
		return status;
	
	if(deserializeSimpleCommand(data, bytes, &simpleCmd) == DESERIALIZE_ERROR)
		return RECEIVE_DATA_ERROR;
	
	//VERIFY RESPONSE MATCHES HELLO COMMAND
	if(!(simpleCmd.command == HELLO_CODE_CMD && simpleCmd.extra == SERVER_HELLO_OK))
		return HELLO_ERROR;

	#ifdef DEBUG
		printf("Received hello OK from server\n");
	#endif
	
	return HELLO_OK;
}

int getDataFromServer(int socket, char * buffer, int max_bytes, int * data_size){
	int read_size;

	read_size = recv(socket,buffer,max_bytes,0);
	if(read_size == 0)
		return SERVER_DISCONNECTED; //server disconnected
	if(read_size < 0){
		fprintf(stderr,"Error: Reading from Socket ... Possible Timeout\n");
		return RECEIVE_DATA_ERROR;
	}

	//Data received correctly
	*data_size = read_size;
	return RECEIVE_DATA_OK;
}

int sendDataToServer(int socket, char * data, int bytes){
	int writed, write_size = 0;

	do{
		writed = write(socket, (data + write_size), bytes);
		if(writed <= 0){
			fprintf(stderr,"Error: Writing to Socket\n");
			return SEND_DATA_ERROR;
		}
		write_size += writed;
	}while(write_size < bytes); //write return value may at times be less than the specified nbytes (returns the number of bytes successfully)

	return SEND_DATA_OK;
}

int getDataAndLengthFromServer(int socket, char * * data, int * bytes){
	//do we need mutex here?
	int status, read_size, send_size, aux_size;
	simpleCommand simpleCmd;
	simpleCommand rCmd = {.command = RESPONSE_CODE_CMD, .extra = CLIENT_RESPONSE_LENGTH_OK};
	char buffer[SERVER_MAX_INPUT_LENGTH];
	char * bytes_aux;
	char * send_buffer;

	//GET THE SIMPLECMD THAT CONTAINS THE LENGTH SIZE
	status = getDataFromServer(socket, buffer, SERVER_MAX_INPUT_LENGTH, &read_size);
	if(status != RECEIVE_DATA_OK)
		return status;

	if(deserializeSimpleCommand(buffer, read_size, &simpleCmd) == DESERIALIZE_ERROR)
		return RECEIVE_DATA_ERROR;
	
	//VERIFY RESPONSE MATCHES LENGTH COMMAND
	if(simpleCmd.command != LENGTH_CODE_CMD)
		return RECEIVE_DATA_ERROR;

	#ifdef DEBUG
		printf("Received cmd: %c extra: %d\n", simpleCmd.command,simpleCmd.extra);
	#endif

	//Reserve memory for server response 
	read_size = simpleCmd.extra;
	bytes_aux = malloc(read_size);
	
	if(bytes_aux == NULL){
		rCmd.extra = CLIENT_RESPONSE_LENGTH_ERROR;
		status = RESERVE_MEM_ERROR;
	}

	//Send response (200 if memz allocate OK )
	send_buffer = serializeSimpleCommand(&rCmd, &send_size);
	if(sendDataToServer(socket, send_buffer, send_size) != SEND_DATA_OK)
	{
		freeSerialized(send_buffer);
		return SEND_DATA_ERROR;
	}
	freeSerialized(send_buffer);

	//receive data if memory allocation success
	if(status == RESERVE_MEM_ERROR)
		return RESERVE_MEM_ERROR;

	status = getDataFromServer(socket, bytes_aux, read_size, &aux_size);
	if(aux_size != read_size)
		return RECEIVE_DATA_ERROR;

	*data = bytes_aux;
	*bytes = aux_size;
	
	return status;
}
