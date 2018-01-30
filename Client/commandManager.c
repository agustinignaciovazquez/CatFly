#include "commandManager.h"
#include "serializeManager.h"
#include "serverManager.h"
#include "expandManager.h"
#include "printManager.h"
#include "constants.h"
#include "inputManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

int displayFlightsMenu(int socket);
int displayFlightMenu(Flight * fl, int socket);
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
	int op;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return 0;

	do{
		printFlightsMenu(fls);
		op = getInt();
		if(op == 0)
			return op;
		if(op > 0 && op <= fls->qFlights){
			f = fls->flights + (op - 1);
			return displayFlightMenu(f, socket);
		}
		printf("Error: Invalid option\n");
	}while(TRUE);

	return 0;
}

int displayFlightMenu(Flight * fl, int socket){
	int flag,op;
	flag = TRUE;
	do{
		printFlightMenu(fl);
		op = getOption();
		switch(op){
			case INSERT_FLIGHT_RESERVATION_CMD:
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