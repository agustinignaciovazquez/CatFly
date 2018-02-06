/* Serialize Manager */

#include "serializeManager.h"
#include "utilsCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int copyReservationsArr(char * ser, const Reservation * rs, int size);
int copyReservationsMinArr(char * ser, const ReservationMinimal * res, int size);
int copyFlightsArr(char * ser, const Flight * fl, int size);
int copyPlanesArr(char * ser, const Plane * pl, int size);
int copyPlane(char * ser, const Plane * pl);
int copyFlight(char * ser, const Flight * fl);
int copyReservation(char * ser, const Reservation * res);
int copySimpleCmd(char * ser, const simpleCommand * cmd);
int copyFlightReservations(char * ser, const flightReservations * fres);
int copySimpleMsg(char * ser, const simpleMessage * mgs);
int copySimpleMsg_bytes(char * ser, const simpleMessage * sm, int msg_bytes);

char * serializeSimpleCommand(simpleCommand * cmd, int * size){
	char * s;
	s = malloc(SIMPLE_CMD_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	*size = copySimpleCmd(s,cmd);

	return s;
}

/* If msg_bytes equals or less than 0, copies bytes until '\0'*/
char * serializeSimpleMessage(simpleMessage * smgs, int * size, int msg_bytes){
	char * s;
	s = malloc(SIMPLE_MSG_SERIALIZE_BYTES);
	if(s == NULL)
		return s;	

	//Copy the struct
	if(msg_bytes <= 0){
		*size = copySimpleMsg(s,smgs);
	}else{
		*size = copySimpleMsg_bytes(s,smgs,msg_bytes);
	}
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
	s = malloc(PLANE_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	//Copy the struct
	*size = copyPlane(s,pl);

	return s;
}

char * serializeUserReservations(Reservations * res, int * size){
	char *s,*aux;
	int totalBytes;

	totalBytes = RESERVATIONS_SERIALIZE_BYTES + (res->qReservations)*RESERVATION_SERIALIZE_BYTES;
	s = malloc(totalBytes);
	if(s == NULL)
		return s;

	//copythestruct
	aux = s;
	aux += copyBytes(aux, (void *)&(res->qReservations), sizeof(res->qReservations));
	aux += copyReservationsArr(aux, res->reservations, res->qReservations);

	*size = (aux -s);
	return s;
}

char * serializeFlightReservations(flightReservations * fres, int * size){
	char * s;
	int totalBytes;
	
	totalBytes = FLIGHT_RESERVATION_SERIALIZE_BYTES + (fres->qReservations)*RESERVATION_MINIMAL_SERIALIZE_BYTES;
	s = malloc(totalBytes);
	if(s == NULL)
		return s;
	//Copy the struct
	*size = copyFlightReservations(s, fres);

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

int copySimpleMsg(char * ser, const simpleMessage * sm){
	char * aux = ser;

	aux += copyBytes(aux, (void *) &(sm->command), sizeof(sm->command));
	aux += copyStr(aux, sm->msg, MAX_MESSAGE_LENGTH);

	return (aux - ser);
}

int copySimpleMsg_bytes(char * ser, const simpleMessage * sm, int msg_bytes){
	char * aux = ser;
	aux += copyBytes(aux, (void *) &(sm->command), sizeof(sm->command));
	if(msg_bytes > MAX_MESSAGE_LENGTH)
		return 0;
	aux += copyBytes(aux, sm->msg, msg_bytes);

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

int copyReservationsArr(char * ser, const Reservation * rs, int size){
	int i;
	char * aux = ser;

	for(i = 0; i < size;i++){
		aux += copyReservation(aux, &(rs[i]));
	}

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

int copyFlightReservations(char * ser, const flightReservations * fres){
	char * aux = ser;
	aux += copyStr(aux, fres->flightCode, MAX_FLIGHTCODE);
	aux += copyPlane(aux, (fres->planeSeats));
	aux += copyBytes(aux, (void *)&(fres->qReservations), sizeof(fres->qReservations));
	aux += copyReservationsMinArr(aux, fres->reservations, fres->qReservations);
	return (aux - ser);
}

void freeSerialized(char * s){
	#ifdef DEBUG
		fprintf(stdout, "Free serialized data in (%p)\n", s);
	#endif
	free(s);
}