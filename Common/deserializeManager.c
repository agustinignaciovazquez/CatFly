
#include "deserializeManager.h"
#include "serializeManager.h" //copybytes and copystr
#include "expandManager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cmd_id deserializeCommand(const char * * command, int * size){
	cmd_id action;
	if(*size < sizeof(cmd_id))
		return DESERIALIZE_ERROR; 

	memcpy(&action, *command, sizeof(cmd_id));
	*command += sizeof(cmd_id); 
	*size -= sizeof(cmd_id);
	return action;
}

int deserializeSimpleCommand(const char * data, int size, simpleCommand * cmd){
	const char * aux;
	if(size != SIMPLE_CMD_SERIALIZE_BYTES)
		return DESERIALIZE_ERROR;
	aux = data;
	cmd->command = deserializeCommand(&aux, &size); //Never returns PARSE_ERROR because previous check
	aux += copyBytes(&(cmd->extra), (void *) aux, sizeof(cmd->extra));
	
	return (aux - data);
}

int deserializeSimpleMessage(const char * data, int size, simpleMessage * sm){
	const char * aux;
	if(sm == NULL)
		return DESERIALIZE_ERROR;

	aux = data;
	//function assumes the sm struct is expanded
	aux += copyBytes(&(sm->command), (void *) aux, sizeof(sm->command));
	aux += copyStr(sm->msg, aux, MAX_MESSAGE_LENGTH);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;
	return (aux - data);
}

int deserializeFlight(const char * data, int size, Flight * fl){
	const char * aux;
	if(fl == NULL)
		return DESERIALIZE_ERROR;

	//function assumes the flight struct is expanded
	aux = data;
	
	aux += copyStr(fl->flightCode, aux, MAX_FLIGHTCODE);
	aux += copyStr(fl->origin, aux, MAX_ORIGIN);
	aux += copyStr(fl->destination, aux, MAX_DESTINATION);
	aux += copyStr(fl->departureDate, aux, MAX_DEP_DATE);
	aux += copyStr(fl->arrivalDate, aux, MAX_ARR_DATE);
	aux += copyStr(fl->planeModel, aux, MAX_PLANE_MODEL);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}

int deserializeFlights(const char * data, int size, Flights * fls){
	const char * aux;
	Flight * fl;
	int i, qFls, bytesDes, addFlag, auxSize;
	
	aux = data;
	auxSize = size;
	aux += copyBytes(&(qFls), (void *) aux, sizeof(fls->qFlights));

	fl = expandFlight(NULL);
	if(fl == NULL)
		return DESERIALIZE_ERROR;

	for(i = 0; i < qFls;i++){
		bytesDes = deserializeFlight(aux, auxSize, fl);
		if(bytesDes == DESERIALIZE_ERROR){
			freeExpandedFlight(fl, FALSE);
			return DESERIALIZE_ERROR;
		}
		
		addFlag = addFlight(fls, fl);
		if(addFlag != EXPAND_OK){
			freeExpandedFlight(fl, FALSE);
			return DESERIALIZE_ERROR;
		}
		
		aux += bytesDes;
		auxSize -= bytesDes;
	}
	freeExpandedFlight(fl, FALSE);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}

int deserializePlane(const char * data, int size, Plane * pl){
	const char * aux;
	if(pl == NULL)
		return DESERIALIZE_ERROR;

	//function assumes the plane struct is expanded
	aux = data;
	aux += copyStr(pl->planeModel, aux, MAX_PLANE_MODEL);
	aux += copyBytes(&(pl->rows), (void *) aux, sizeof(pl->rows));
	aux += copyBytes(&(pl->columns), (void *) aux, sizeof(pl->columns));

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}

int deserializePlanes(const char * data, int size, Planes * pls){
	const char * aux;
	Plane * pl;
	int i, qPls, bytesDes, addFlag, auxSize;
	
	aux = data;
	auxSize = size;
	aux += copyBytes(&qPls, (void *) aux, sizeof(pls->qPlanes));

	pl = expandPlane(NULL);
	if(pl == NULL)
		return DESERIALIZE_ERROR;

	for(i = 0; i < qPls;i++){
		bytesDes = deserializePlane(aux, auxSize, pl);
		if(bytesDes == DESERIALIZE_ERROR){
			freeExpandedPlane(pl, FALSE);
			return DESERIALIZE_ERROR;
		}
		
		addFlag = addPlane(pls, pl);
		if(addFlag != EXPAND_OK){
			freeExpandedPlane(pl, FALSE);
			return DESERIALIZE_ERROR;
		}

		aux += bytesDes;
		auxSize -= bytesDes;
		
	}

	freeExpandedPlane(pl, FALSE);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}

int deserializeReservation(const char * data, int size, Reservation * res){
	const char * aux;
	if(res == NULL)
		return DESERIALIZE_ERROR;

	//function assumes the reservation struct is expanded
	aux = data;
	aux += copyStr(res->flightCode, aux, MAX_FLIGHTCODE);
	aux += copyBytes(&(res->seatRow), (void *) aux, sizeof(res->seatRow));
	aux += copyBytes(&(res->seatColumn), (void *) aux, sizeof(res->seatColumn));
	aux += copyStr(res->passportID, aux, MAX_PASSPORTID);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}

int deserializeReservationMin(const char * data, int size, ReservationMinimal * res){
	const char * aux;
	if(res == NULL)
		return DESERIALIZE_ERROR;

	//function assumes the reservation struct is expanded
	aux = data;
	aux += copyBytes(&(res->seatRow), (void *) aux, sizeof(res->seatRow));
	aux += copyBytes(&(res->seatColumn), (void *) aux, sizeof(res->seatColumn));

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}

int deserializeFlightReservations(const char * data, int size, flightReservations * res){
	const char * aux;
	ReservationMinimal resMin;
	int i, qRes, bytesDes, addFlag, auxSize;
	
	aux = data;
	auxSize = size;

	aux += copyStr(res->flightCode, aux, MAX_FLIGHTCODE);
	aux += deserializePlane(aux, auxSize, res->planeSeats);
	aux += copyBytes(&(qRes), (void *) aux, sizeof(res->qReservations));

	for(i = 0; i < qRes;i++){
		bytesDes = deserializeReservationMin(aux, auxSize, &resMin);
		if(bytesDes == DESERIALIZE_ERROR)
			return DESERIALIZE_ERROR;
				
		addFlag = addReservation(res, &resMin);
		if(addFlag != EXPAND_OK)
			return DESERIALIZE_ERROR;
		
		aux += bytesDes;
		auxSize -= bytesDes;
		
	}
	

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return (aux - data);
}