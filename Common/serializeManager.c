/* Serialize Manager */

#include "serializeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copyReservationsMinArr(char * ser, const ReservationMinimal * res, int size);
int copyFlightsArr(char * ser, const Flight * fl, int size);
int copyPlanesArr(char * ser, const Plane * pl, int size);
int copyPlane(char * ser, const Plane * pl);
int copyFlight(char * ser, const Flight * fl);
int copyReservation(char * ser, const Reservation * res);
int copySimpleCmd(char * ser, const simpleCommand * cmd);


char * serializeSimpleCommand(simpleCommand * cmd, int * size){
	char * s;
	s = malloc(SIMPLE_CMD_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	*size = copySimpleCmd(s,cmd);

	return s;
}

char * serializeFlight(Flight * fl, int * size){
	char * s;

	s = malloc(FLIGHT_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	*size = copyFlight(s, fl);

	return s;
}

char * serializeReservation(Reservation * res, int * size){
	char * s;
	s = malloc(RESERVATION_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	*size = copyReservation(s,res);

	return s;
}


char * serializePlane(Plane * pl, int * size){
	char * s;
	int tot;
	s = malloc(PLANE_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	tot = copyPlane(s,pl);
	*size = tot;

	return s;
}

char * serializeFlightReservations(flightReservations * fres, int * size){
	char * s, *aux;
	int totalBytes;
	
	totalBytes = FLIGHT_RESERVATION_SERIALIZE_BYTES + (fres->qReservations)*RESERVATION_MINIMAL_SERIALIZE_BYTES;
	s = malloc(totalBytes);
	if(s == NULL)
		return s;
	//Copy the struct
	aux = s;
	aux += copyStr(aux, fres->flightCode, MAX_FLIGHTCODE);
	aux += copyPlane(aux, (fres->planeSeats));
	aux += copyBytes(aux, (void *)&(fres->qReservations), sizeof(fres->qReservations));
	aux += copyReservationsMinArr(aux, fres->reservations, fres->qReservations);

	*size = (aux - s);
	return s;
}

char * serializeFlights(Flights * fl, int * size){
	char *s,*aux;
	int totalBytes;

	totalBytes = FLIGHTS_SERIALIZE_BYTES + (fl->qFlights)*FLIGHT_SERIALIZE_BYTES;
	s = malloc(totalBytes);
	if(s == NULL)
		return s;

	//copythestruct
	aux = s;
	aux += copyBytes(aux, (void *)&(fl->qFlights), sizeof(fl->qFlights));
	aux += copyFlightsArr(aux, fl->flights, fl->qFlights);

	*size = (aux -s);
	return s;
}

char * serializePlanes(Planes * pl, int * size){
	char *s,*aux;
	int totalBytes;

	totalBytes = PLANES_SERIALIZE_BYTES + (pl->qPlanes)*PLANE_SERIALIZE_BYTES;
	s = malloc(totalBytes);
	if(s == NULL)
		return s;

	//copythestruct
	aux = s;
	aux += copyBytes(aux, (void *)&(pl->qPlanes), sizeof(pl->qPlanes));
	aux += copyPlanesArr(aux, pl->planes, pl->qPlanes);

	*size = (aux -s);
	return s;
}

int copySimpleCmd(char * ser, const simpleCommand * cmd){
	char * aux = ser;

	aux += copyBytes(aux, (void *) &(cmd->command), sizeof(cmd->command));
	aux += copyBytes(aux, (void *) &(cmd->extra), sizeof(cmd->extra));

	return (aux - ser);
}

int copyPlane(char * ser, const Plane * pl){
	char * aux = ser;

	aux += copyStr(aux, pl->planeModel, MAX_PLANE_MODEL);
	aux += copyBytes(aux, (void *) &(pl->rows), sizeof(pl->rows));
	aux += copyBytes(aux, (void *) &(pl->columns), sizeof(pl->columns));

	return (aux - ser);
}

int copyReservation(char * ser, const Reservation * res){
	char * aux = ser;

	aux += copyStr(aux, res->flightCode, MAX_FLIGHTCODE);
	aux += copyBytes(aux, (void *) &(res->seatRow), sizeof(res->seatRow));
	aux += copyBytes(aux, (void *) &(res->seatColumn), sizeof(res->seatColumn));
	aux += copyStr(aux, res->passportID, MAX_PASSPORTID);

	return (aux - ser);
}

int copyFlight(char * ser, const Flight * fl){
	char * aux;
	aux = ser;

	aux += copyStr(aux, fl->flightCode, MAX_FLIGHTCODE);
	aux += copyStr(aux, fl->origin, MAX_ORIGIN);
	aux += copyStr(aux, fl->destination, MAX_DESTINATION);
	aux += copyStr(aux, fl->departureDate, MAX_DEP_DATE);
	aux += copyStr(aux, fl->arrivalDate, MAX_ARR_DATE);
	aux += copyStr(aux, fl->planeModel, MAX_PLANE_MODEL);

	return (aux - ser);
}

int copyReservationsMinArr(char * ser, const ReservationMinimal * res, int size){
	return copyBytes(ser, res, sizeof(ReservationMinimal)*size); 
}

int copyFlightsArr(char * ser, const Flight * fl, int size){
	int i;
	char * aux = ser;

	for(i = 0; i < size;i++){
		aux += copyFlight(aux, &(fl[i]));
	}

	return (aux - ser); 
}

int copyPlanesArr(char * ser, const Plane * pl, int size){
	int i;
	char * aux = ser;

	for(i = 0; i < size;i++){
		aux += copyPlane(aux, &(pl[i]));
	}

	return (aux - ser); 
}

int copyStr(char * str, const char * data,  int max){
	int i;

	for(i = 0; i < max-1 && data[i] != '\0';i++){
		str[i] = data[i];
	}
	str[i] = '\0';
	i++;

	return i;
}

int copyBytes(void * dest, const void * data, int bytes){
	memcpy(dest, data, bytes);
	return bytes;
}

void freeSerialized(char * s){
	free(s);
}