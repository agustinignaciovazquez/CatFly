#include "serverSecurityTests.h"
#include "serverManager.h"
#include "serverHandlerCore.h" //Send and recive functions
#include "expandManager.h"
#include "clientCore.h" //createSocket
#include "constants.h"
#include "utilsCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <time.h>

#define BUF_SIZE 7777
#define TEST_PASSPORTID "39765402"

void fillRandomData(char * buff, int bytes);
ReservationMinimal getRandomFreeSeat(Flight * f, int socket);
ReservationMinimal getFreeSeatFromMatrix(char * * matrix, int rows, int columns);
Reservation getReservation(Flight * f, int socket);

void cancellationTest(){
	int con_status, socket;
	simpleMessage * firstMsg,* secndMsg, * thrdmsg;
	Reservation res;
	Flights * fls;
	Flight * f;
	int i = 0;
	
	printf("\n\nServer Reservation test\n");

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

	fls = getFlights_Server(socket);
	if(fls == NULL || fls->qFlights <= 0){
		printf("No flights to reserve\n");
		return;
	}

	do{
	f = (fls->flights) + i; //Select the first flight
	res = getReservation(f,socket);
	i++;
	}while(i < fls->qFlights && res.seatRow < 0);

	if(res.seatRow < 0){
		printf("All seats in all flights occupied :(\n");
		return;
	}

	firstMsg = insertReservation_Server(&res,socket);
	secndMsg = insertCancellation_Server(&res,socket);
	thrdmsg = insertReservation_Server(&res,socket);
	if(firstMsg != NULL && secndMsg != NULL && thrdmsg != NULL){
		//Check manually (second attempt must return error)
		printf("First attempt to reserve seat(%d,%d) in flight (%s): %s\n",res.seatRow+1,res.seatColumn+1,res.flightCode,firstMsg->msg);
		printf("Cancelling seat(%d,%d) in flight (%s): %s\n",res.seatRow+1,res.seatColumn+1,res.flightCode,secndMsg->msg);
		printf("Second attempt to reserve seat(%d,%d) in flight (%s): %s\n",res.seatRow+1,res.seatColumn+1,res.flightCode,thrdmsg->msg);
	}

	freeExpandedSimpleMessage(firstMsg);
	freeExpandedSimpleMessage(secndMsg);
	freeFlights(fls);
}

void reservationTest(){
	int con_status, socket;
	simpleMessage * firstMsg,* secndMsg;
	Reservation res;
	Flights * fls;
	Flight * f;
	int i = 0;
	
	printf("\n\nServer Reservation test\n");

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

	fls = getFlights_Server(socket);
	if(fls == NULL || fls->qFlights <= 0){
		printf("No flights to reserve\n");
		return;
	}

	do{
	f = (fls->flights) + i; //Select the first flight
	res = getReservation(f,socket);
	i++;
	}while(i < fls->qFlights && res.seatRow < 0);

	if(res.seatRow < 0){
		printf("All seats in all flights occupied :(\n");
		return;
	}

	firstMsg = insertReservation_Server(&res,socket);
	secndMsg = insertReservation_Server(&res,socket);
	if(firstMsg != NULL && secndMsg != NULL){
		//Check manually (second attempt must return error)
		printf("First attempt to reserve seat(%d,%d) in flight (%s): %s\n",res.seatRow+1,res.seatColumn+1,res.flightCode,firstMsg->msg);
		printf("Second attempt to reserve seat(%d,%d) in flight (%s): %s\n",res.seatRow+1,res.seatColumn+1,res.flightCode,secndMsg->msg);
	}

	freeExpandedSimpleMessage(firstMsg);
	freeExpandedSimpleMessage(secndMsg);
	freeFlights(fls);
}

Reservation getReservation(Flight * f, int socket){
	Reservation res = {.seatRow = -1, .seatColumn =-1, .flightCode = f->flightCode, .passportID = TEST_PASSPORTID};
	ReservationMinimal rm = getRandomFreeSeat(f,socket);

	res.seatRow = rm.seatRow;
	res.seatColumn = rm.seatColumn;
	return res;
}

ReservationMinimal getRandomFreeSeat(Flight * f, int socket){
	char * * matrix;
	flightReservations * frs;
	ReservationMinimal rm = {.seatRow = -1, .seatColumn =-1};

	frs = getFlightReservations_Server(f,socket);
	if(frs == NULL){
		printf("unexpected error\n");
		return rm;
	}

	matrix = createReservationsMatrix(frs);
	if(matrix == NULL){
		printf("unexpected error\n");
		return rm;
	}

	rm = getFreeSeatFromMatrix(matrix,frs->planeSeats->rows,frs->planeSeats->columns);

	freeReservationsMatrix(frs,matrix);
	freeFlightReservations(frs);
	return rm;
}

ReservationMinimal getFreeSeatFromMatrix(char * * matrix, int rows, int columns){
	int i,j;
	ReservationMinimal rm = {.seatRow = -1, .seatColumn =-1};
	for(i = 0; i < rows; i++){
		for(j = 0; j < columns; j++){
			if(matrix[i][j] != OCCUPIED_SEAT){
				rm.seatRow = i;
				rm.seatColumn = j;
			}
		}
	}
	return rm;
}

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