/* Serialize Manager */

#include "serializeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copyStr(char * ser, const char * string, int max);
int copyBytes(char * ser, const void * data, int bytes);

char * serializeSimpleCommand(simpleCommand * cmd, int * size){
	char * s, *aux;
	s = malloc(SIMPLE_CMD_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	aux = s;
	aux += copyBytes(aux, (void *) &(cmd->command), sizeof(cmd->command));
	aux += copyBytes(aux, (void *) &(cmd->extra), sizeof(cmd->extra));

	*size = SIMPLE_CMD_SERIALIZE_BYTES;
	return s;
}

char * serializeFlight(Flight * fl, int * size){
	char * s, *aux;
	s = malloc(FLIGHT_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	aux = s;
	aux += copyStr(aux, fl->flightCode, MAX_FLIGHTCODE);
	aux += copyStr(aux, fl->origin, MAX_ORIGIN);
	aux += copyStr(aux, fl->destination, MAX_DESTINATION);
	aux += copyStr(aux, fl->departureDate, MAX_DEP_DATE);
	aux += copyStr(aux, fl->arrivalDate, MAX_ARR_DATE);
	aux += copyStr(aux, fl->planeModel, MAX_PLANE_MODEL);

	*size = (aux - s);
	return s;
}

char * serializeReservation(Reservation * res, int * size){
	char * s, *aux;
	s = malloc(RESERVATION_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	aux = s;
	aux += copyStr(aux, res->flightCode, MAX_FLIGHTCODE);
	aux += copyBytes(aux, (void *) &(res->seatRow), sizeof(res->seatRow));
	aux += copyBytes(aux, (void *) &(res->seatColumn), sizeof(res->seatColumn));
	aux += copyStr(aux, res->passportID, MAX_PASSPORTID);

	*size = (aux - s);
	return s;
}

char * serializePlane(Plane * pl, int * size){
	char * s, *aux;
	s = malloc(PLANE_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	aux = s;
	aux += copyStr(aux, pl->planeModel, MAX_PLANE_MODEL);
	aux += copyBytes(aux, (void *) &(pl->rows), sizeof(pl->rows));
	aux += copyBytes(aux, (void *) &(pl->columns), sizeof(pl->columns));

	*size = (aux - s);
	return s;
}

int copyStr(char * ser, const char * string,  int max){
	int i;

	for(i = 0; i < max && string[i] != '\0';i++){
		ser[i] = string[i];
	}
	ser[i] = '\0';
	i++;

	return i;
}

int copyBytes(char * ser, const void * data, int bytes){
	memcpy(ser, data, bytes);
	return bytes;
}

void freeSerialized(char * s){
	free(s);
}