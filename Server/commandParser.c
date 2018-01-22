#include "commandParser.h"
#include "constants.h"
#include "serializeManager.h"
#include "deserializeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define DEBUG 1


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
int disconnectClient(char * * response, int * response_bytes);

int verifyResponseFromClient(const char * data, int bytes, int responseID){
	simpleCommand simpleCmd;

	if(deserializeSimpleCommand(data, bytes, &simpleCmd) != DESERIALIZE_OK)
		return RESPONSE_ERROR;

	#ifdef DEBUG
		printf("Received cmd: %c extra: %d\n", simpleCmd.command,simpleCmd.extra);
	#endif

	//VERIFY COMMAND MATCHES RESPONSE_CODE
	if(simpleCmd.command != RESPONSE_CODE_CMD)
		return RESPONSE_ERROR;

	return (simpleCmd.extra == responseID)? RESPONSE_OK : RESPONSE_NO;
}

int veryfyLengthResponse(const char * data, int bytes){
	return verifyResponseFromClient(data,bytes,CLIENT_RESPONSE_LENGTH_OK);
}

int parseRequest(const char * command, int size, char * * response, int * response_bytes){
	cmd_id action;

	action = deserializeCommand(&command, &size);
	switch(action){
		case GET_FLIGHTS_CMD:
			return getAllFlights(command, size, response, response_bytes);
			break;
		case GET_FLIGHT_INFO_CMD:
			return getFlightInfo(command, size, response, response_bytes);
			break;
		case INSERT_FLIGHT_CMD:
			return insertFlight(command, size, response, response_bytes);
			break;
		case DELETE_FLIGHT_CMD:
			return deleteFlight(command, size, response, response_bytes);
			break;
		case GET_PLANES_CMD:
			return getPlanes(command, size, response, response_bytes);
			break;
		case INSERT_PLANE_CMD:
			return insertPlane(command, size, response, response_bytes);
			break;
		case DELETE_PLANE_CMD:
			return deletePlane(command, size, response, response_bytes);
			break;
		case GET_FLIGHT_RESERVATION_CMD:
			return getReservations(command, size, response, response_bytes);
			break;
		case INSERT_FLIGHT_RESERVATION_CMD:
			return insertReservation(command, size, response, response_bytes);
			break;
		case DELETE_FLIGHT_RESERVATION_CMD:
			return deleteReservation(command, size, response, response_bytes);
			break;
		case DISCONNECT_CMD:
			return disconnectClient(response, response_bytes);
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
	*response = DISCONNECT_CODE;
	*response_bytes = DISCONNECT_CODE_LEN;
	return RESPONSE_OK;
}

int disconnectClient(char * * response, int * response_bytes){
	*response = DISCONNECT_CODE;
	*response_bytes = DISCONNECT_CODE_LEN;
	return RESPONSE_OK_AND_DISCONNECT;
}

