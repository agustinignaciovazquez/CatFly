
#include "expandManager.h"
#include "serializeManager.h" //copybytes and copystr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int expandStr(char * * p, int max);

Flight * expandFlight(){
	Flight * fl;

	fl = malloc(sizeof(Flight));
	if(fl == NULL)
		return NULL;

	//Expand all strings
	if(expandStr(&(fl->flightCode), MAX_FLIGHTCODE) != EXPAND_OK){
		freeExpandedFlight(fl);
		return NULL;
	}
	if(expandStr(&(fl->origin), MAX_ORIGIN) != EXPAND_OK){
		freeExpandedFlight(fl);
		return NULL;
	}
	if(expandStr(&(fl->destination), MAX_DESTINATION) != EXPAND_OK){
		freeExpandedFlight(fl);
		return NULL;
	}
	if(expandStr(&(fl->departureDate), MAX_DEP_DATE) != EXPAND_OK){
		freeExpandedFlight(fl);
		return NULL;
	}
	if(expandStr(&(fl->arrivalDate), MAX_ARR_DATE) != EXPAND_OK){
		freeExpandedFlight(fl);
		return NULL;
	}
	if(expandStr(&(fl->planeModel), MAX_PLANE_MODEL) != EXPAND_OK){
		freeExpandedFlight(fl);
		return NULL;
	}
		
	return fl;
}

Plane * expandPlane(){
	Plane * pl;

	pl = calloc(1,sizeof(Plane));
	if(pl == NULL)
		return NULL;

	//Expand everything
	if(expandStr(&(pl->planeModel), MAX_PLANE_MODEL) != EXPAND_OK){
		freeExpandedPlane(pl);
		return NULL;
	}

	return pl;
}

Reservation * expandReservation(){
	Reservation * res;

	res = calloc(1,sizeof(Reservation));
	if(res == NULL)
		return NULL;

	//Expand everything
	if(expandStr(&(res->flightCode), MAX_FLIGHTCODE) != EXPAND_OK){
		freeExpandedReservation(res);
		return NULL;
	}
	if(expandStr(&(res->passportID), MAX_PASSPORTID) != EXPAND_OK){
		freeExpandedReservation(res);
		return NULL;
	}

	return res;
}
int expandStr(char * * p, int max){
	char * s;

	s = calloc(max, sizeof(char));
	if(s == NULL)
		return EXPAND_ERROR;

	*p = s;
	return EXPAND_OK;
}

void freeExpandedReservation(Reservation * res){
	free(res->flightCode);
	free(res->passportID);
	free(res);
}

void freeExpandedPlane(Plane * pl){
	free(pl->planeModel);
	free(pl);
}

void freeExpandedFlight(Flight * fl){
	free(fl->flightCode);
	free(fl->origin);
	free(fl->destination);
	free(fl->departureDate);
	free(fl->arrivalDate);
	free(fl->planeModel);
	free(fl);
}