#include "commandParser.h"
#include "constants.h"
#include "serializeManager.h"
#include "deserializeManager.h"
#include "expandManager.h"
#include "databaseManager.h"
#include "databaseCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

void printFlights(Flights * fls);
void printFlight(Flight * fl);

int getAllFlights(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int getFlightInfo(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int insertFlight(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int deleteFlight(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int getPlanes(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int insertPlane(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int deletePlane(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int getReservations(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int insertReservation(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int deleteReservation(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int disconnectClient(char * * response, int * response_bytes, sqlite3 * db);

int verifyResponseFromClient(const char * data, int bytes, int responseID){
	simpleCommand simpleCmd;

	if(deserializeSimpleCommand(data, bytes, &simpleCmd) == DESERIALIZE_ERROR)
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
	int r;
	sqlite3 * db;
	action = deserializeCommand(&command, &size);

	if(openDatabase(&db) != SQLITE_OK)
		return SQL_ERROR;

	switch(action){
		case GET_FLIGHTS_CMD:
			r = getAllFlights(command, size, response, response_bytes, db);
			break;
		case GET_FLIGHT_CANCELATIONS_CMD:
			r = getFlightInfo(command, size, response, response_bytes, db);
			break;
		case INSERT_FLIGHT_CMD:
			r = insertFlight(command, size, response, response_bytes, db);
			break;
		case DELETE_FLIGHT_CMD:
			r = deleteFlight(command, size, response, response_bytes, db);
			break;
		case GET_PLANES_CMD:
			r = getPlanes(command, size, response, response_bytes, db);
			break;
		case INSERT_PLANE_CMD:
			r = insertPlane(command, size, response, response_bytes, db);
			break;
		case DELETE_PLANE_CMD:
			r = deletePlane(command, size, response, response_bytes, db);
			break;
		case GET_FLIGHT_RESERVATION_CMD:
			r = getReservations(command, size, response, response_bytes, db);
			break;
		case INSERT_FLIGHT_RESERVATION_CMD:
			r = insertReservation(command, size, response, response_bytes, db);
			break;
		case DELETE_FLIGHT_RESERVATION_CMD:
			r = deleteReservation(command, size, response, response_bytes, db);
			break;
		case DISCONNECT_CMD:
			r = disconnectClient(response, response_bytes, db);
			break;
		default:
			r = PARSE_ERROR;
			break;
	}
	closeDatabase(db);
	return r;
}

void printFlight(Flight * fl){
  fprintf(stdout,"struct in : %p\nFlight Code: %s (at %p)\n", fl, fl->flightCode, (fl->flightCode));
  fprintf(stdout,"Origin: %s (at %p)\n", fl->origin, (fl->origin));
  fprintf(stdout,"Dest: %s (at %p)\n", fl->destination, (fl->destination));
  fprintf(stdout,"dep date: %s (at %p)\n", fl->departureDate, (fl->departureDate));
  fprintf(stdout,"arr date: %s (at %p)\n", fl->arrivalDate, (fl->arrivalDate));
  fprintf(stdout,"plane model: %s (at %p)\n", fl->planeModel, (fl->planeModel));
}
void printFlights(Flights * fls){
  int q,i;

  for(i = 0, q = fls->qFlights; i<q;i++ ){
    Flight * f = fls->flights + i;
    printFlight(f);
  }
}

int getAllFlights(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Flights * fls;
	
	if((fls = getFlights_DB(db)) == NULL)
		return SQL_ERROR;

	*response = serializeFlights(fls, response_bytes);
	freeFlights(fls);

	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

 
int getFlightInfo(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "flightinfo";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int insertFlight(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "inseflight";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int deleteFlight(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "delflight";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int getPlanes(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Planes * pls;
	
	if((pls = getPlanes_DB(db)) == NULL)
		return SQL_ERROR;
	
	*response = serializePlanes(pls, response_bytes);
	freePlanes(pls);

	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int insertPlane(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "instplane";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int deletePlane(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "delplane";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int getReservations(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	flightReservations * frs;
	char * flightCode = "AAA1";
	if(FALSE) //TODO DESERIALIZE FLIGHT CODE
		return PARSE_ERROR;

	if((frs = getReservations_DB(flightCode,db)) == NULL)
		return SQL_ERROR;
	
	*response = serializeFlightReservations(frs, response_bytes);
	freeFlightReservations(frs);

	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int insertReservation(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "insrese";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int deleteReservation(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response = DISCONNECT_CODE;
	*response_bytes = DISCONNECT_CODE_LEN;
	return RESPONSE_OK;
}

int disconnectClient(char * * response, int * response_bytes, sqlite3 * db){
	*response = DISCONNECT_CODE;
	*response_bytes = DISCONNECT_CODE_LEN;
	return RESPONSE_OK_AND_DISCONNECT;
}

