#include "serverHandlerCore.h" 
#include "menuManager.h"
#include "constants.h"
#include "serializeManager.h"
#include "deserializeManager.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sendDataToServer(int socket, char * data, int bytes);
int getDataFromServer(int socket, char * buffer, int max_bytes, int * data_size);
int getHelloFromServer(int socket, int isAdmin);

int serverHandler(int socket){
	int con_status;
	int isAdmin;
	isAdmin = TRUE; //TODO CHANGE LATER
	con_status = getHelloFromServer(socket, isAdmin);

	//Verify we are connected to the server w/ unique Hello command
	if(con_status != HELLO_OK)
		return HELLO_ERROR;
	
	displayAdminMenu(socket);

	return con_status;
}

//Returns SEND_DATA_OK if sends simple command successfully
int sendSimpleCmd(int socket, simpleCommand * simpleCmd){
	int status, bytes;
	char * bytes_aux;

	bytes_aux = serializeSimpleCommand(simpleCmd, &bytes);
	if(bytes_aux == NULL)
		return SEND_DATA_ERROR;
	
	status = sendDataToServer(socket, bytes_aux, bytes);
	freeSerialized(bytes_aux);

	return status;
}

//Returns SEND_DATA_OK if sends simple message successfully
int sendSimpleMsg(int socket, simpleMessage * simpleMsg){
	return sendSimpleMsg_w_bytes(socket, simpleMsg, 0);
}

int sendSimpleMsg_w_bytes(int socket, simpleMessage * simpleMsg, int msg_bytes){
	int status, bytes;
	char * bytes_aux;

	bytes_aux = serializeSimpleMessage(simpleMsg, &bytes, msg_bytes);
	if(bytes_aux == NULL)
		return SEND_DATA_ERROR;
	
	status = sendDataToServer(socket, bytes_aux, bytes);
	freeSerialized(bytes_aux);

	return status;
}

int getHelloFromServer(int socket, int isAdmin){
	int status, bytes;
	char data[SERVER_MAX_INPUT_LENGTH];
	simpleCommand simpleCmd = {.command = HELLO_CODE_CMD, .extra = CLIENT_HELLO_OK};

	if(isAdmin == TRUE){
		simpleCmd.command = HELLO_ADMIN_CODE_CMD;
		simpleCmd.extra = CLIENT_ADMIN_HELLO_OK;
	}

	//Say hello to server
	status = sendSimpleCmd(socket, &simpleCmd);
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
	int status, read_size, aux_size;
	simpleCommand simpleCmd;
	simpleCommand rCmd = {.command = RESPONSE_CODE_CMD, .extra = CLIENT_RESPONSE_LENGTH_OK};
	char buffer[SERVER_MAX_INPUT_LENGTH];
	char * bytes_aux;

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
	if(sendSimpleCmd(socket, &rCmd) != SEND_DATA_OK)
		return SEND_DATA_ERROR;

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
