#include "commandManager.h"
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
int displayFlightReservations(Flight * fl, int socket);
void displaySimpleMenu(int socket){
	int flag,op;
	flag = TRUE;
	do{
		printSimpleMenu();
		op = getOption();
		switch(op){
			case GET_FLIGHTS_CMD:
				op = displayFlightsMenu(socket);
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
			break;
			case GET_USER_RESERVATIONS_CMD:
			break;
			case DISCONNECT_CMD:
				flag = FALSE;
				//TODO ADD FUNCTION TO SEND SERVER DISCONNECTION
			break;
			default:
				printf("Error: Invalid option\n");
			break;
		}
	}while(flag == TRUE && op != EOF);
}

int displayFlightsMenu(int socket){
	int flag,op;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return 0;

	flag = TRUE;
	do{
		printFlightsMenu(fls);
		op = getInt();
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
			case INSERT_FLIGHT_RESERVATION_CMD:
				displayFlightReservations(fl, socket);
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

int displayFlightReservations(Flight * fl, int socket){
	Reservation * rm;
	flightReservations * frs;
	char * * resMatrix;
	
	frs = getFlightReservations_Server(fl, socket);
	if(frs == NULL)
		return 0;
	
	if((resMatrix = createReservationsMatrix(frs)) == NULL){
		freeFlightReservations(frs);
		return 0;
	}

	printFlight(fl);
	printReservations(resMatrix, frs->planeSeats);
	rm = getReservationFromInput(fl);

	if(checkReservationInput(rm) == TRUE){ 
		simpleMessage * response;
		printf("Adding reservation:\n");
		response = insertReservation_Server(rm, socket);
		printf("Status: %s\n", response->msg);
		freeExpandedSimpleMessage(response);
	}

	freeExpandedReservation(rm, FALSE);
	freeReservationsMatrix(frs, resMatrix);
	freeFlightReservations(frs);
	return 0;
}