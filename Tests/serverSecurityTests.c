#include "serverSecurityTests.h"
#include "serverManager.h"
#include "serverHandlerCore.h" //Send and recive functions
#include "clientCore.h" //createSocket
#include "constants.h"
#include "expandManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <time.h>

#define BUF_SIZE 7777

void fillRandomData(char * buff, int bytes);
int sendFuzzData(int socket);

void notAdminTest(){
	int con_status, socket;
	Planes * pls;
	printf("\n\nServer Admin test\n");

    socket = createSocket(SERVER_ADDR, SERVER_PORT);
    if(socket == SERVER_CONNECTION_ERROR){
    	printf("Error server unreacheable\n");
    	return;
    }

    con_status = getHelloFromServer(socket, FALSE);//Hello as normal user
    if(con_status != HELLO_OK){
		printf("Error server unreacheable\n");
		return;
    }

    //Try to execute one of the admin commands (should return NULL and disconnect)
	pls = getPlanes_Server(socket);
	if(pls == NULL){
		printf("Test OK, Server didnt response us\n");
	}else{
		printf("Test NOT OK, Server return us a response for an admin command\n");
	}

	freePlanes(pls);
}

void fuzzTest(){
	int con_status, socket;
	printf("\n\nServer Fuzz test\n");

    socket = createSocket(SERVER_ADDR, SERVER_PORT);
    if(socket == SERVER_CONNECTION_ERROR){
    	printf("Error server unreacheable\n");
    	return;
    }

    con_status = getHelloFromServer(socket, FALSE);
    if(con_status != HELLO_OK){
		printf("Error server unreacheable\n");
		return;
    }

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
    	return;
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