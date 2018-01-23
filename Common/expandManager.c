
#include "expandManager.h"
#include "serializeManager.h" //copybytes and copystr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK 7

int expandStr(char * * p, int max);
void FlightDeepCopy(Flight * dest, const Flight * fl);
void PlaneDeepCopy(Plane * dest, const Plane * pl);
void ResMinDeepCopy(ReservationMinimal * dest, const ReservationMinimal * rs);
void setFlightReserSettings(flightReservations * frs, const char * flightCode, Plane * pl);

Flight * expandFlight(Flight * fl){
	if(fl == NULL)
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

Flights * expandFlights(){
	Flights * fls;

	fls = malloc(sizeof(Flights));
	if(fls == NULL)
		return NULL;

	fls->qFlights = 0;
	fls->flights = NULL;
	return fls;
}

int addFlight(Flights * expanded, const Flight * fl){
	int q = expanded->qFlights;
	Flight *aux = expanded->flights;

	if((q % BLOCK) == 0){
		aux = realloc(expanded->flights, (q+BLOCK)*sizeof(Flight));
		if(aux == NULL){
			freeFlights(expanded);
			return EXPAND_ERROR;
		}
		expanded->flights = aux;
	}
	if(expandFlight((expanded->flights)+q) == NULL)
		return EXPAND_ERROR;

	FlightDeepCopy(((expanded->flights)+q), fl);
	expanded->qFlights += 1;
	return EXPAND_OK;
}

void FlightDeepCopy(Flight * dest, const Flight * fl){
	copyStr(dest->flightCode, fl->flightCode, MAX_FLIGHTCODE);
	copyStr(dest->origin, fl->origin, MAX_ORIGIN);
	copyStr(dest->destination, fl->destination, MAX_DESTINATION);
	copyStr(dest->departureDate, fl->departureDate, MAX_DEP_DATE);
	copyStr(dest->arrivalDate, fl->arrivalDate, MAX_ARR_DATE);
	copyStr(dest->planeModel, fl->planeModel, MAX_PLANE_MODEL);
}

Plane * expandPlane(Plane * pl){
	if(pl == NULL)
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

Planes * expandPlanes(){
	Planes * pls;

	pls = malloc(sizeof(Planes));
	if(pls == NULL)
		return NULL;

	pls->qPlanes = 0;
	pls->planes = NULL;
	return pls;
}

int addPlane(Planes * expanded, const Plane * pl){
	int q = expanded->qPlanes;
	Plane *aux = expanded->planes;

	if((q % BLOCK) == 0){
		aux = realloc(expanded->planes, (q+BLOCK)*sizeof(Plane));
		if(aux == NULL){
			freePlanes(expanded);
			return EXPAND_ERROR;
		}
		expanded->planes = aux;
	}
	if(expandPlane((expanded->planes)+q) == NULL)
		return EXPAND_ERROR;

	PlaneDeepCopy(((expanded->planes)+q), pl);
	expanded->qPlanes += 1;
	return EXPAND_OK;
}

void PlaneDeepCopy(Plane * dest, const Plane * pl){
	dest->rows = pl->rows;
	dest->columns = pl->columns;
	copyStr(dest->planeModel, pl->planeModel, MAX_PLANE_MODEL);
}

Reservation * expandReservation(Reservation * res){
	if(res == NULL)
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

//DEPRECATED
ReservationMinimal * expandReservationMinimal(ReservationMinimal * res){
	if(res == NULL)
		res = calloc(1,sizeof(ReservationMinimal));
	if(res == NULL)
		return NULL;

	return res;
}
//------------

flightReservations * expandFlightReservations(){
	flightReservations * frs;

	frs = calloc(1,sizeof(flightReservations));
	if(frs == NULL)
		return NULL;

	if(expandStr(&(frs->flightCode), MAX_FLIGHTCODE) != EXPAND_OK){
		freeFlightReservations(frs);
		return NULL;
	}

	if((frs->planeSeats = expandPlane(0)) == NULL){
		freeFlightReservations(frs);
		return NULL;
	}

	frs->qReservations = 0;
	frs->reservations = NULL;
	return frs;
}

void setFlightReservationsSettings(flightReservations * frs, const char * flightCode, Plane * pl){
	copyStr(frs->flightCode, flightCode, MAX_FLIGHTCODE);
	PlaneDeepCopy((frs->planeSeats), pl);
}

int addReservation(flightReservations * expanded, const ReservationMinimal * rs){
	int q = expanded->qReservations;
	ReservationMinimal *aux = expanded->reservations;

	if((q % BLOCK) == 0){
		aux = realloc(expanded->reservations, (q+BLOCK)*sizeof(ReservationMinimal));
		if(aux == NULL){
			freeFlightReservations(expanded);
			return EXPAND_ERROR;
		}
		expanded->reservations = aux;
	}
	/*if(expandReservationMinimal((expanded->reservations)+q) == NULL)
		return EXPAND_ERROR;*/

	ResMinDeepCopy(((expanded->reservations)+q), rs);
	expanded->qReservations += 1;
	return EXPAND_OK;
}

void ResMinDeepCopy(ReservationMinimal * dest, const ReservationMinimal * rs){
	dest->seatRow = rs->seatRow;
	dest->seatColumn = rs->seatColumn;
}

void freeFlightReservations(flightReservations * frs){
	free((frs->planeSeats).planeModel);
	free(frs->flightCode);
	free(frs);
}

void freeFlights(Flights * fls){
	int i;
	Flight * f;
	for(i = 0; i < fls->qFlights;i++){
		f = fls->flights+i;
		freeExpandedFlight(f);
	}
	free(fls->flights);
	free(fls);
}

void freePlanes(Planes * pls){
	int i;
	Plane * p;
	for(i = 0; i < pls->qPlanes;i++){
		p = pls->planes+i;
		freeExpandedPlane(p);
	}
	free(pls->planes);
	free(pls);
}

void freeExpandedReservation(Reservation * res){
	free(res->flightCode);
	free(res->passportID);
	free(res);
}

void freeExpandedReservationMin(Reservation * res){
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

int expandStr(char * * p, int max){
	char * s;

	s = calloc(max, sizeof(char));
	if(s == NULL)
		return EXPAND_ERROR;

	*p = s;
	return EXPAND_OK;
}