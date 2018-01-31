#include "menuAdminManager.h"
#include "menuManager.h"
#include "serverManager.h"
#include "expandManager.h"
#include "printManager.h"
#include "constants.h"
#include "utilsCore.h"
#include "inputManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
void displayInsertFlight(int socket);
void displayInsertPlane(int socket);

void displayAdminMenu(int socket){
	int flag,op;
	flag = TRUE;
	do{
		printAdminMenu();
		op = getOption();
		switch(op){
			case GET_FLIGHTS_CMD:
				displayFlightsMenu(socket);
			break;
			case INSERT_FLIGHT_CMD:
				displayInsertFlight(socket);
			break;
			case DELETE_FLIGHT_CMD:
				
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
				displayReservationsMenu(socket);
			break;
			case GET_PLANES_CMD:
				
			break;
			case INSERT_PLANE_CMD:
				displayInsertPlane(socket);
			break;
			case DELETE_PLANE_CMD:
				
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

void displayInsertFlight(int socket){
	Flight * f;

	f = getFlightFromInput();
	
	if(checkFlightInput(f) == TRUE){
		simpleMessage * response;
		printf("Adding Flight\n");
		response = insertFlight_Server(f, socket);
		printf("Status: %s\n", response->msg);
		freeExpandedSimpleMessage(response);
	}

	freeExpandedFlight(f,FALSE);
}

void displayInsertPlane(int socket){
	Plane * p;

	p = getPlaneFromInput();

	if(checkPlaneInput(p) == TRUE){
		simpleMessage * response;
		printf("Adding Plane\n");
		response = insertPlane_Server(p, socket);
		printf("Status: %s\n", response->msg);
		freeExpandedSimpleMessage(response);
	}

	freeExpandedPlane(p,FALSE);
}