#include "commandParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define GET_FLIGHTS 'A'
#define GET_FLIGHT_INFO 'B'
#define INSERT_FLIGHT 'C'
#define DELETE_FLIGHT 'D'
#define GET_PLANES 'E'
#define INSERT_PLANE 'F'
#define DELETE_PLANE 'G'
#define GET_FLIGHT_RESERVATIONS 'H'
#define INSERT_FLIGHT_RESERVATION 'I'
#define DELETE_FLIGHT_RESERVATION 'J'
#define DISCONNECT 'Q'

int getAllFlights(const char * command, int size, char * * response, int * response_bytes);
int getFlightInfo(const char * command, int size, char * * response, int * response_bytes);
int insertFlight(const char * command, int size, char * * response, int * response_bytes);
int deleteFlight(const char * command, int size, char * * response, int * response_bytes);
int getPlanes(const char * command, int size, char * * response, int * response_bytes);
int insertPlane(const char * command, int size, char * * response, int * response_bytes);
int deletePlane(const char * command, int size, char * * response, int * response_bytes);
int getReservations(const char * command, int size, char * * response, int * response_bytes);
int insertReservation(const char * command, int size, char * * response, int * response_bytes);
int deleteReservation(const char * command, int size, char * * response, int * response_bytes);

int parseRequest(const char * command, int size, char * * response, int * response_bytes){
	char action;

	if(size == 0)
		return PARSE_ERROR; //should never happend ... just to be safe anyway
	action = *command;
	command += sizeof(char); 
	size -= sizeof(char);

	switch(action){
		case GET_FLIGHTS:
			return getAllFlights(command, size, response, response_bytes);
			break;
		case GET_FLIGHT_INFO:
			return getFlightInfo(command, size, response, response_bytes);
			break;
		case INSERT_FLIGHT:
			return insertFlight(command, size, response, response_bytes);
			break;
		case DELETE_FLIGHT:
			return deleteFlight(command, size, response, response_bytes);
			break;
		case GET_PLANES:
			return getPlanes(command, size, response, response_bytes);
			break;
		case INSERT_PLANE:
			return insertPlane(command, size, response, response_bytes);
			break;
		case DELETE_PLANE:
			return deletePlane(command, size, response, response_bytes);
			break;
		case GET_FLIGHT_RESERVATIONS:
			return getReservations(command, size, response, response_bytes);
			break;
		case INSERT_FLIGHT_RESERVATION:
			return insertReservation(command, size, response, response_bytes);
			break;
		case DELETE_FLIGHT_RESERVATION:
			return deleteReservation(command, size, response, response_bytes);
			break;
		case DISCONNECT:
			return RESPONSE_OK_AND_DISCONNECT;
			break;

	}

	return PARSE_ERROR;
}

int getAllFlights(const char * command, int size, char * * response, int * response_bytes){
	*response = "flights";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int getFlightInfo(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "flightinfo";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int insertFlight(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "inseflight";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int deleteFlight(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "delflight";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int getPlanes(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "planes";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int insertPlane(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "instplane";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int deletePlane(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "delplane";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int getReservations(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "rese";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int insertReservation(const char * command, int size, char * * response, int * response_bytes){
	*response_bytes = 20;
	*response = "insrese";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int deleteReservation(const char * command, int size, char * * response, int * response_bytes){
	*response = SQL_DISCONNECT_CODE;
	*response_bytes = SQL_DISCONNECT_CODE_LEN;
	return RESPONSE_OK;
}

