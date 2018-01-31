#include "menuManager.h"
#include "serializeManager.h"
#include "serverManager.h"
#include "expandManager.h"
#include "printManager.h"
#include "constants.h"
#include "utilsCore.h"
#include "inputManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

int displayFlightsMenu(int socket);
int displayFlightMenu(Flight * fl, int socket);
int displayReservationsMenu(int socket);
int displayReservationsMenu(int socket);
int displayFlightReservations(Flight * fl, int socket);
int displayInsertFlightReservation(Flight * fl, int socket);
int displayUserReservationsMenu(int socket);
int displayDeleteUserReservationsMenu(int socket);
Reservations * getUserReservationsById(int socket);
void displayCancellation(Reservation * r, int socket);
void displaySimpleMenu(int socket){
	int flag,op;
	flag = TRUE;
	do{
		printSimpleMenu();
		op = getOption();
		switch(op){
			case GET_FLIGHTS_CMD:
				displayFlightsMenu(socket);
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
				displayReservationsMenu(socket);
			break;
			case GET_USER_RESERVATIONS_CMD:
				displayUserReservationsMenu(socket);
			break;
			case DELETE_FLIGHT_RESERVATION_CMD:
				displayDeleteUserReservationsMenu(socket);
			break;
			case DISCONNECT_CMD:
				notifyDisconnection(socket);
				flag = FALSE;
			break;
			default:
				printf("Error: Invalid option\n");
			break;
		}
	}while(flag == TRUE && op != EOF);
}

int displayFlightsMenu(int socket){
	int flag,op = 0;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return 0;

	flag = TRUE;
	do{
		printFlightsMenu(fls);

		if(fls->qFlights <= 0)
			break;
		
		op = getInt("Please select one flight Or 0 to go back");
		if(op == 0)
			flag = FALSE;//GO BACK

		if(op > 0 && op <= fls->qFlights){//Check if number is in array bounds
			flag = FALSE;
			f = fls->flights + (op - 1);
			op = displayFlightMenu(f, socket);
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freeFlights(fls);
	return op;
}

int displayFlightMenu(Flight * fl, int socket){
	int flag,op;
	flag = TRUE;
	do{
		printFlightMenu(fl);
		op = getOption();
		switch(op){
			case GET_FLIGHT_RESERVATIONS_CMD:
				displayFlightReservations(fl, socket);
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
				displayInsertFlightReservation(fl, socket);
			break;
			case BACK_CMD:
				flag = FALSE;
			break;
			default:
				printf("Error: Invalid option\n");
			break;
		}
	}while(flag == TRUE && op != EOF);
	return op;
}

int displayReservationsMenu(int socket){
	int flag, op = 0;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return 0;

	flag = TRUE;
	do{
		printFlightsMenu(fls);
		if(fls->qFlights <= 0){
			break;
		}
			
		op = getInt("Please select one flight to reserve Or 0 to go back");	
		if(op == 0)
			flag = FALSE;//GO BACK

		if(op > 0 && op <= fls->qFlights){//Check if number is in array bounds
			flag = FALSE;
			f = fls->flights + (op - 1);
			displayInsertFlightReservation(f, socket);
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freeFlights(fls);
	return op;
}

int displayUserReservationsMenu(int socket){
	Reservations * res = getUserReservationsById(socket);
	printReservationsMenu(res);
	freeUserReservations(res);
	return 0;
}

int displayDeleteUserReservationsMenu(int socket){
	int flag,op = 0;
	Reservations * res;
	Reservation * r;
	res = getUserReservationsById(socket);
	if(res == NULL)
		return 0;

	flag = TRUE;
	do{
		printReservationsMenu(res);

		if(res->qReservations < 1)
			break;
		
		op = getInt("Please select one reservation to cancel Or 0 to go back");
		if(op == 0)
			flag = FALSE;//GO BACK

		if(op > 0 && op <= res->qReservations){//Check if number is in array bounds
			flag = FALSE;
			r = res->reservations + (op - 1);
			displayCancellation(r, socket);
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freeUserReservations(res);
	return op;
}

void displayCancellation(Reservation * r, int socket){
	simpleMessage * response;
	printf("Removing reservation\n");
	response = insertCancellation_Server(r, socket);
	printf("Status: %s\n", response->msg);
	freeExpandedSimpleMessage(response);
}

Reservations * getUserReservationsById(int socket){
	Reservations * res;
	Reservation * aux = expandReservation(NULL);

	getString("Enter your passport ID", aux->passportID, MAX_PASSPORTID);

	res = getUserReservations_Server(aux, socket);
	freeExpandedReservation(aux, FALSE);

	if(res == NULL){
		printf("Unexpected error .. Please try again later\n");
		return 0;
	}
	
	return res;
}

int displayFlightReservations(Flight * fl, int socket){
	flightReservations * frs;
	char * * resMatrix;
	
	frs = getFlightReservations_Server(fl, socket);
	if(frs == NULL)
		return 0;
	
	if((resMatrix = createReservationsMatrix(frs)) == NULL){
		freeFlightReservations(frs);
		return 0;
	}
	printf("Flight Reservations: \n");
	printFlightMin(fl,-1);
	printReservations(resMatrix, frs->planeSeats);
	
	freeReservationsMatrix(frs, resMatrix);
	freeFlightReservations(frs);
	return 0;
}

int displayInsertFlightReservation(Flight * fl, int socket){
	int r;
	Reservation * rm;

	displayFlightReservations(fl,socket);

	rm = getReservationFromInput(fl);
	if((r = checkReservationInput(rm)) == TRUE){ 
		simpleMessage * response;
		printf("Adding reservation\n");
		response = insertReservation_Server(rm, socket);
		printf("Status: %s\n", response->msg);
		freeExpandedSimpleMessage(response);
	}
	displayFlightReservations(fl,socket);
	freeExpandedReservation(rm, FALSE);
	return r;
}