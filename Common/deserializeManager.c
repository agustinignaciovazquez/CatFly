
#include "deserializeManager.h"
#include "serializeManager.h" //copybytes and copystr
#include "expandManager.h"
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
	if(size != SIMPLE_CMD_SERIALIZE_BYTES)
		return DESERIALIZE_ERROR;
	cmd->command = deserializeCommand(&data, &size); //Never returns PARSE_ERROR because previous check
	data += copyBytes(&(cmd->extra), (void *) data, sizeof(cmd->extra));
	
	return DESERIALIZE_OK;
}

int deserializeFlight(const char * data, int size, Flight * fl){
	const char * aux;
	if(size > FLIGHT_SERIALIZE_BYTES || fl == NULL)
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
	aux += copyBytes(&(fls->qFlights), (void *) aux, sizeof(fls->qFlights));

	fl = expandFlight(NULL);
	if(fl == NULL)
		return DESERIALIZE_ERROR;

	for(i = 0, qFls = fls->qFlights; i < qFls;i++){
		bytesDes = deserializeFlight(aux, auxSize, fl);
		addFlag = addFlight(fls, fl);

		if(bytesDes == DESERIALIZE_ERROR || addFlag != EXPAND_OK){
			freeExpandedFlight(fl);
			return DESERIALIZE_ERROR;
		}

		aux += bytesDes;
		auxSize -= bytesDes;
	}
	freeExpandedFlight(fl);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return DESERIALIZE_OK;
}

int deserializePlane(const char * data, int size, Plane * pl){
	const char * aux;
	if(size > PLANE_SERIALIZE_BYTES || pl == NULL)
		return DESERIALIZE_ERROR;

	//function assumes the plane struct is expanded
	aux = data;
	aux += copyStr(pl->planeModel, aux, MAX_PLANE_MODEL);
	aux += copyBytes(&(pl->rows), (void *) aux, sizeof(pl->rows));
	aux += copyBytes(&(pl->columns), (void *) aux, sizeof(pl->columns));

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return DESERIALIZE_OK;
}

int deserializePlanes(const char * data, int size, Planes * pls){
	const char * aux;
	Plane * pl;
	int i, qPls, bytesDes, addFlag, auxSize;
	
	aux = data;
	auxSize = size;
	aux += copyBytes(&(pls->qPlanes), (void *) aux, sizeof(pls->qPlanes));

	pl = expandPlane(NULL);
	if(pl == NULL)
		return DESERIALIZE_ERROR;

	for(i = 0, qPls = pls->qPlanes; i < qPls;i++){
		bytesDes = deserializePlane(aux, auxSize, pl);
		addFlag = addPlane(pls, pl);

		if(bytesDes == DESERIALIZE_ERROR || addFlag != EXPAND_OK){
			freeExpandedPlane(pl);
			return DESERIALIZE_ERROR;
		}

		aux += bytesDes;
		auxSize -= bytesDes;
	}
	freeExpandedPlane(pl);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return DESERIALIZE_OK;
}

int deserializeReservation(const char * data, int size, Reservation * res){
	const char * aux;
	if(size > RESERVATION_SERIALIZE_BYTES || res == NULL)
		return DESERIALIZE_ERROR;

	//function assumes the reservation struct is expanded
	aux = data;
	aux += copyStr(res->flightCode, aux, MAX_FLIGHTCODE);
	aux += copyBytes(&(res->seatRow), (void *) aux, sizeof(res->seatRow));
	aux += copyBytes(&(res->seatColumn), (void *) aux, sizeof(res->seatColumn));
	aux += copyStr(res->passportID, aux, MAX_PASSPORTID);

	if(size < (aux - data))
		return DESERIALIZE_ERROR;

	return DESERIALIZE_OK;
}