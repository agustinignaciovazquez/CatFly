#include "serverSecurityTests.h"
#include "serverHandlerCore.h" //Send and recive functions
#include "clientCore.h" //createSocket
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <time.h>

#define BUF_SIZE 7777
/*
int sendDataToServer(int socket, char * data, int bytes);
int getDataFromServer(int socket, char * buffer, int max_bytes, int * data_size);
int getHelloFromServer(int socket, int isAdmin);*/

void fillRandomData(char * buff, int bytes);
int sendFuzzData(int socket);

void fuzzTest(){
	int con_status, socket;
	printf("\n\nServer Fuzz test\n");

    socket = createSocket(SERVER_ADDR, SERVER_PORT);
    if(socket == SERVER_CONNECTION_ERROR){
    	printf("Error server unreacheable\n");
    }

    con_status = getHelloFromServer(socket, FALSE);
    if(con_status != HELLO_OK)
		printf("Error server unreacheable\n");

	con_status = sendFuzzData(socket);
	if(con_status != RECEIVE_DATA_OK){
		printf("Test OK, Server disconect us for security reasons\n");
	}else{
		printf("Test NOT OK, Server didnt disconect us\n");
	}
}

void fuzzTest_in_HELLO_REQ(){
	int con_status, socket;
	printf("\n\nServer Fuzz in Hello Request test\n");

    socket = createSocket(SERVER_ADDR, SERVER_PORT);
    if(socket == SERVER_CONNECTION_ERROR){
    	printf("Error server unreacheable\n");
    }

	con_status = sendFuzzData(socket);
	if(con_status != RECEIVE_DATA_OK){
		printf("Test OK, Server disconect us for security reasons\n");
	}else{
		printf("Test NOT OK, Server didnt disconect us\n");
	}
}

int sendFuzzData(int socket){
	char buffer[BUF_SIZE];
	int s,aux;
	fillRandomData(buffer, BUF_SIZE);

	if((s = sendDataToServer(socket, buffer, BUF_SIZE)) != SEND_DATA_OK){
		printf("Error sending data\n");
		return s;
	}

	return getDataFromServer(socket, buffer, BUF_SIZE, &aux);
}

void fillRandomData(char * buff, int bytes){
	char r;
	srand(time(NULL));
	for(int i = 0;  i < bytes;i++){
		r = (char) rand();
		buff[i] = r;
	}
}