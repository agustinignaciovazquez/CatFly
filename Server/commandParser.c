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

int getAllFlights(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
int getFlightCancelations(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db);
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
		case GET_FLIGHT_CANCELATIONS_CMD:
			r = getFlightCancelations(command, size, response, response_bytes, db);
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

int getAllFlights(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Flights * fls;
	
	if((fls = getFlights_DB(db)) == NULL)
		return SQL_ERROR;

	*response = serializeFlights(fls, response_bytes);
	freeFlights(fls);

	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

 
int getFlightCancelations(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response_bytes = 20;
	*response = "flightinfo";
	*response_bytes = strlen(*response);
	return RESPONSE_OK;
}

int insertFlight(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Flight * f;
	simpleMessage * msg;

	f = expandFlight(NULL);
	if(f == NULL)
		return EXPAND_ERROR;
	
	if(deserializeFlight(command, size, f) == DESERIALIZE_ERROR){
		freeExpandedFlight(f, FALSE);
		return PARSE_ERROR;
	}

	msg = insertFlight_DB(f,db);
	freeExpandedFlight(f, FALSE);
	if(msg == NULL)
		return EXPAND_ERROR;

	*response = serializeSimpleMessage(msg, response_bytes);
	freeExpandedSimpleMessage(msg);

	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int deleteFlight(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Flight * f;
	simpleMessage * msg;

	f = expandFlight(NULL);
	if(f == NULL)
		return EXPAND_ERROR;
	

	if(deserializeFlight(command, size, f) == DESERIALIZE_ERROR){
		freeExpandedFlight(f, FALSE);
		return PARSE_ERROR;
	}

	msg = deleteFlight_DB(f->flightCode,db);
	freeExpandedFlight(f, FALSE);
	if(msg == NULL)
		return EXPAND_ERROR;

	*response = serializeSimpleMessage(msg, response_bytes);
	freeExpandedSimpleMessage(msg);
	
	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
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
	Plane * p;
	simpleMessage * msg;

	p = expandPlane(NULL);
	if(p == NULL)
		return EXPAND_ERROR;
	

	if(deserializePlane(command, size,p) == DESERIALIZE_ERROR){
		freeExpandedPlane(p, FALSE);
		return PARSE_ERROR;
	}

	msg = insertPlane_DB(p,db);
	freeExpandedPlane(p, FALSE);
	if(msg == NULL)
		return EXPAND_ERROR;

	*response = serializeSimpleMessage(msg, response_bytes);
	freeExpandedSimpleMessage(msg);
	
	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int deletePlane(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Plane * p;
	simpleMessage * msg;

	p = expandPlane(NULL);
	if(p == NULL)
		return EXPAND_ERROR;
	

	if(deserializePlane(command, size,p) == DESERIALIZE_ERROR){
		freeExpandedPlane(p, FALSE);
		return PARSE_ERROR;
	}

	msg = deletePlane_DB(p->planeModel,db);
	freeExpandedPlane(p, FALSE);
	if(msg == NULL)
		return EXPAND_ERROR;

	*response = serializeSimpleMessage(msg, response_bytes);
	freeExpandedSimpleMessage(msg);
	
	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int getReservations(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	flightReservations * frs;
	Flight * f;

	f = expandFlight(NULL);
	if(f == NULL)
		return EXPAND_ERROR;
	

	if(deserializeFlight(command, size, f) == DESERIALIZE_ERROR){
		freeExpandedFlight(f, FALSE);
		return PARSE_ERROR;
	}

	frs = getReservations_DB(f->flightCode,db);
	freeExpandedFlight(f, FALSE);
	if(frs == NULL)
		return SQL_ERROR;
	
	*response = serializeFlightReservations(frs, response_bytes);
	freeFlightReservations(frs);

	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int insertReservation(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	Reservation * res;
	simpleMessage * msg;

	res = expandReservation(NULL);
	if(res == NULL)
		return EXPAND_ERROR;
	
	if(deserializeReservation(command, size, res) == DESERIALIZE_ERROR){
		freeExpandedReservation(res, FALSE);
		return PARSE_ERROR;
	}

	msg = insertReservation_DB_wMessage(res,db);
	freeExpandedReservation(res, FALSE);
	if(msg == NULL)
		return EXPAND_ERROR;

	*response = serializeSimpleMessage(msg, response_bytes);
	freeExpandedSimpleMessage(msg);
	
	return (*response != NULL) ? RESPONSE_OK : EXPAND_ERROR;
}

int deleteReservation(const char * command, int size, char * * response, int * response_bytes, sqlite3 * db){
	*response = DISCONNECT_CODE;
	*response_bytes = DISCONNECT_CODE_LEN;
	return RESPONSE_OK;
}

int disconnectClient(char * * response, int * response_bytes, sqlite3 * db){
	return RESPONSE_OK_AND_DISCONNECT;
}

