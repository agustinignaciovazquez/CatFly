#include "menuManager.h"
#include "menuAdminManager.h"
#include "serverManager.h"
#include "expandManager.h"
#include "printManager.h"
#include "constants.h"
#include "utilsCore.h"
#include "inputManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

void displaySimpleMenu(int socket);
void displayFlightMenu(Flight * fl, int socket);
int displayDeleteUserReservationsMenu(int socket);
int displayCancellation(Reservation * r, int socket);
int displayDisconnection(int socket);
int displayFlightReservations(Flight * fl, int socket);
int displayUserReservationsMenu(int socket);
int displayInsertFlightReservation(Flight * fl, int socket);
Reservations * getUserReservationsById(int socket);
int flightReservationsMatchesFlight(Flight * fl, flightReservations * frs);

/* Returns TRUE if user selects to be Admin FALSE otherwise*/
int displaySelection(){
	int op;
	do{	
		printSelectionMenu();
		op = getInt("Please select one option");
		switch(op){
			case ENTER_AS_USER:
				return FALSE;
			break;
			case ENTER_AS_ADMIN:
				return TRUE;
			break;
			default:
				printf("Invalid number\n");
			break;
		}
		
	}while(TRUE);
	return FALSE;
}

void displayMenu(int isAdmin, int socket){
	if(isAdmin == TRUE){
		displayAdminMenu(socket);
		return;
	}
	displaySimpleMenu(socket);
}

void displaySimpleMenu(int socket){
	int flag,op;
	flag = TRUE;
	do{
		printSimpleMenu();
		op = getOption();
		switch(op){
			case GET_FLIGHTS_CMD:
				flag = displayFlightsMenu(socket);
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
				flag = displayReservationsMenu(socket);
			break;
			case GET_USER_RESERVATIONS_CMD:
				flag = displayUserReservationsMenu(socket);
			break;
			case DELETE_FLIGHT_RESERVATION_CMD:
				flag = displayDeleteUserReservationsMenu(socket);
			break;
			case DISCONNECT_CMD:
				flag = displayDisconnection(socket);
			break;
			default:
				printf("Error: Invalid option\n");
			break;
		}
	}while(flag == TRUE && op != EOF);
}

int displayDisconnection(int socket){
	printf("Disconecting...\n");
	notifyDisconnection(socket);
	return FALSE;
}

/* Returns FALSE if Server is not reachable */
int displayFlightsMenu(int socket){
	int flag,op = 0;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return FALSE;

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
			displayFlightMenu(f, socket);
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freeFlights(fls);
	return TRUE;
}

void displayFlightMenu(Flight * fl, int socket){
	int flag,op;
	flag = TRUE;
	do{
		printFlightMenu(fl);
		op = getOption();
		switch(op){
			case GET_FLIGHT_RESERVATIONS_CMD:
				flag = (displayFlightReservations(fl, socket)  == STATUS_OK) ? TRUE : FALSE;
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
				flag = (displayInsertFlightReservation(fl, socket) == STATUS_OK) ? TRUE : FALSE;
			break;
			case BACK_CMD:
				flag = FALSE;
			break;
			default:
				printf("Error: Invalid option\n");
			break;
		}
	}while(flag == TRUE && op != EOF);
}

/* Returns FALSE if Server is not reachable */
int displayReservationsMenu(int socket){
	int flag, op = 0;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return FALSE;

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
	return TRUE;
}

/* Returns FALSE if Server is not reachable */
int displayUserReservationsMenu(int socket){
	Reservations * res = getUserReservationsById(socket);
	if(res == NULL)
		return FALSE;

	printReservationsMenu(res);
	freeUserReservations(res);
	return TRUE;
}

int displayDeleteUserReservationsMenu(int socket){
	int flag,op = 0;
	Reservations * res;
	Reservation * r;
	res = getUserReservationsById(socket);
	if(res == NULL)
		return FALSE;

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
			if(displayCancellation(r, socket) == FALSE){
				freeUserReservations(res);
				return FALSE;
			}
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freeUserReservations(res);
	return TRUE;
}

int displayCancellation(Reservation * r, int socket){
	simpleMessage * response;
	printf("Removing reservation\n");
	response = insertCancellation_Server(r, socket);
	if(response == NULL)
		return FALSE;
	
	printf("Status: %s\n", response->msg);
	freeExpandedSimpleMessage(response);
	return TRUE;
}

Reservations * getUserReservationsById(int socket){
	Reservations * res;
	Reservation * aux;

	aux = getReservationPassportFromInput();
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
	if(frs == NULL || flightReservationsMatchesFlight(fl, frs) == FALSE){
		printf("Flight does not exists anymore\n");
		freeFlightReservations(frs);
		return STATUS_ERROR_NOT_EXISTS;
	}
	
	if((resMatrix = createReservationsMatrix(frs)) == NULL){
		freeFlightReservations(frs);
		return STATUS_ERROR_MATRIX_EXPAND;
	}

	printf("Flight Reservations: \n");
	printFlightMin(fl,-1);
	printReservations(resMatrix, frs->planeSeats);
	
	freeReservationsMatrix(frs, resMatrix);
	freeFlightReservations(frs);
	return STATUS_OK;
}

int flightReservationsMatchesFlight(Flight * fl, flightReservations * frs){
	return (strcmp(frs->flightCode,fl->flightCode) == 0)? TRUE:FALSE;
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
		if(response != NULL){
			printf("Status: %s\n", response->msg);
			freeExpandedSimpleMessage(response);
		}
	}

	r = displayFlightReservations(fl,socket);
	freeExpandedReservation(rm, FALSE);
	return r;
}
