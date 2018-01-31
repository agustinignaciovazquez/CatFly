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
void displayPlanesMenu(int socket);
void displayDeleteFlightsMenu(int socket);
void displayDeletePlanesMenu(int socket);
void displayDeleteFlight(Flight * fl, int socket);
void displayDeletePlane(Plane * pl, int socket);

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
				displayDeleteFlightsMenu(socket);
			break;
			case INSERT_FLIGHT_RESERVATION_CMD:
				displayReservationsMenu(socket);
			break;
			case GET_PLANES_CMD:
				displayPlanesMenu(socket);
			break;
			case INSERT_PLANE_CMD:
				displayInsertPlane(socket);
			break;
			case DELETE_PLANE_CMD:
				displayDeletePlanesMenu(socket);
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

void displayDeleteFlightsMenu(int socket){
	int flag,op;
	Flights * fls;
	Flight * f;
	fls = getFlights_Server(socket);
	if(fls == NULL)
		return;

	flag = TRUE;
	do{
		printFlightsMenu(fls);

		if(fls->qFlights <= 0)
			break;
		
		op = getInt("Please select one flight to delete Or 0 to go back");
		if(op == 0)
			flag = FALSE;//GO BACK

		if(op > 0 && op <= fls->qFlights){//Check if number is in array bounds
			flag = FALSE;
			f = fls->flights + (op - 1);
			displayDeleteFlight(f, socket);
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freeFlights(fls);
}

void displayInsertFlight(int socket){
	Flight * f;

	f = getFlightFromInput();

	if(checkFlightInput(f) == TRUE){
		simpleMessage * response;
		printf("Adding Flight\n");
		response = insertFlight_Server(f, socket);
		if(response != NULL){
			printf("Status: %s\n", response->msg);
			freeExpandedSimpleMessage(response);
		}
	}

	freeExpandedFlight(f,FALSE);
}

void displayDeleteFlight(Flight * fl, int socket){
	simpleMessage * response;

	printf("Deleting flight: ");
	printFlightMin(fl,-1);
	response = deleteFlight_Server(fl, socket);
	if(response != NULL){
		printf("Status: %s\n", response->msg);
		freeExpandedSimpleMessage(response);
	}
}

void displayPlanesMenu(int socket){
	Planes * pls;
	pls = getPlanes_Server(socket);
	if(pls == NULL)
		return;
	printPlanesMenu(pls);
	freePlanes(pls);
}

void displayInsertPlane(int socket){
	Plane * p;

	p = getPlaneFromInput();

	if(checkPlaneInput(p) == TRUE){
		simpleMessage * response;
		printf("Adding Plane\n");
		response = insertPlane_Server(p, socket);
		if(response != NULL){
			printf("Status: %s\n", response->msg);
			freeExpandedSimpleMessage(response);
		}
	}

	freeExpandedPlane(p,FALSE);
}

void displayDeletePlanesMenu(int socket){
	int flag,op;
	Planes * pls;
	Plane * p;

	pls = getPlanes_Server(socket);
	if(pls == NULL)
		return;
	
	flag = TRUE;
	do{
		printPlanesMenu(pls);

		if(pls->qPlanes <= 0)
			break;
		
		op = getInt("Please select one plane to delete Or 0 to go back");
		if(op == 0)
			flag = FALSE;//GO BACK

		if(op > 0 && op <= pls->qPlanes){//Check if number is in array bounds
			flag = FALSE;
			p = pls->planes + (op - 1);
			displayDeletePlane(p, socket);
		}else{
			printf("Error: Invalid option\n");
		}
		
	}while(flag == TRUE);

	freePlanes(pls);
}

void displayDeletePlane(Plane * pl, int socket){
	simpleMessage * response;

	printf("Deleting plane: ");
	printPlaneMin(pl);
	response = deletePlane_Server(pl, socket);
	if(response != NULL){
		printf("Status: %s\n", response->msg);
		freeExpandedSimpleMessage(response);
	}
}