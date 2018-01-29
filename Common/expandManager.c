
#include "expandManager.h"
#include "serializeManager.h" 
#include "utilsCore.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK 7

int expandStr(char * * p, int max);
void FlightDeepCopy(Flight * dest, const Flight * fl);
void PlaneDeepCopy(Plane * dest, const Plane * pl);
void ResMinDeepCopy(ReservationMinimal * dest, const ReservationMinimal * rs);
void setFlightReserSettings(flightReservations * frs, const char * flightCode, Plane * pl);

simpleMessage * expandSimpleMessage(){
	simpleMessage * sMgs;

	sMgs = calloc(1, sizeof(simpleMessage));
	if(sMgs == NULL)
		return NULL;

	if(expandStr(&(sMgs->msg), MAX_MESSAGE_LENGTH) != EXPAND_OK){
		freeExpandedSimpleMessage(sMgs);
		return NULL;
	}

	return sMgs;
}

void setSimpleMessageSettings(simpleMessage * sMsgs, cmd_id cmd,const char * msg){
	copyStr(sMsgs->msg, msg, MAX_MESSAGE_LENGTH);
	sMsgs->command = cmd;
}

void setSimpleMessageSettings_w_bytes(simpleMessage * sMsgs, cmd_id cmd, const char * msg, int msg_bytes){
	char * messageStr = sMsgs->msg;
	copyBytes(messageStr, msg, msg_bytes);
	messageStr[msg_bytes] = '\0';
	sMsgs->command = cmd;
}

Flight * expandFlight(Flight * fl){
	int isArray = (fl == NULL)? FALSE : TRUE;

	if(isArray == FALSE)
		fl = malloc(sizeof(Flight));
	if(fl == NULL)
		return NULL;

	//Expand all strings
	if(expandStr(&(fl->flightCode), MAX_FLIGHTCODE) != EXPAND_OK){
		freeExpandedFlight(fl, isArray);
		return NULL;
	}
	if(expandStr(&(fl->origin), MAX_ORIGIN) != EXPAND_OK){
		freeExpandedFlight(fl, isArray);
		return NULL;
	}
	if(expandStr(&(fl->destination), MAX_DESTINATION) != EXPAND_OK){
		freeExpandedFlight(fl, isArray);
		return NULL;
	}
	if(expandStr(&(fl->departureDate), MAX_DEP_DATE) != EXPAND_OK){
		freeExpandedFlight(fl, isArray);
		return NULL;
	}
	if(expandStr(&(fl->arrivalDate), MAX_ARR_DATE) != EXPAND_OK){
		freeExpandedFlight(fl, isArray);
		return NULL;
	}
	if(expandStr(&(fl->planeModel), MAX_PLANE_MODEL) != EXPAND_OK){
		freeExpandedFlight(fl, isArray);
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
	Flight *f,*aux = expanded->flights;
	
	if((q % BLOCK) == 0){

		aux = realloc(expanded->flights, (q+BLOCK)*sizeof(Flight));
		if(aux == NULL){
			freeFlights(expanded);
			return EXPAND_ERROR;
		}
		expanded->flights = aux;

	}
	f = (expanded->flights)+q;
	if(expandFlight(f) == NULL)
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
	int isArray = (pl == NULL)? FALSE : TRUE;

	if(isArray == FALSE)
		pl = calloc(1,sizeof(Plane));
	if(pl == NULL)
		return NULL;

	//Expand everything
	if(expandStr(&(pl->planeModel), MAX_PLANE_MODEL) != EXPAND_OK){
		freeExpandedPlane(pl, isArray);
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
	int isArray = (res == NULL)? FALSE:TRUE;

	if(isArray == FALSE)
		res = calloc(1,sizeof(Reservation));
	if(res == NULL)
		return NULL;

	//Expand everything
	if(expandStr(&(res->flightCode), MAX_FLIGHTCODE) != EXPAND_OK){
		freeExpandedReservation(res, isArray);
		return NULL;
	}
	if(expandStr(&(res->passportID), MAX_PASSPORTID) != EXPAND_OK){
		freeExpandedReservation(res, isArray);
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

void freeExpandedReservationMin(Reservation * res){
	free(res);
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

void setFlightReservationsSettings(flightReservations * frs, const char * flightCode, const Plane * pl){
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
		return EXPAND_ERROR;*/ //no se necesita

	ResMinDeepCopy(((expanded->reservations)+q), rs);
	expanded->qReservations += 1;
	return EXPAND_OK;
}

void ResMinDeepCopy(ReservationMinimal * dest, const ReservationMinimal * rs){
	dest->seatRow = rs->seatRow;
	dest->seatColumn = rs->seatColumn;
}

void freeExpandedSimpleMessage(simpleMessage * sMgs){
	if(sMgs == NULL)
		return;
	#ifdef DEBUG
		fprintf(stdout, "Free msg STR in (%p)\n", sMgs->msg);
		fprintf(stdout, "Free struct in (%p)\n", sMgs);
	#endif
	free(sMgs->msg);
	free(sMgs);
}

void freeFlightReservations(flightReservations * frs){
	if(frs == NULL)
		return;
	#ifdef DEBUG
		fprintf(stdout, "Free FLIGHTCODE STR in (%p)\n", frs->flightCode);
		fprintf(stdout, "Free array reservations in (%p)\n", frs->reservations);
		fprintf(stdout, "Free struct in (%p)\n", frs);
	#endif
	freeExpandedPlane(frs->planeSeats, FALSE);
	free(frs->flightCode);
	free(frs->reservations);
	free(frs);
}

void freeFlights(Flights * fls){
	int i,q;
	Flight * f;
	if(fls == NULL)
		return;
	q = fls->qFlights;
	#ifdef DEBUG
		fprintf(stdout, "Free array flights in (%p)\n", fls->flights);
		fprintf(stdout, "Free struct in (%p)\n", fls);
	#endif
	for(i = 0; i < q;i++){
		f = (fls->flights)+i;
		freeExpandedFlight(f, TRUE);
	}
	free(fls->flights);
	free(fls);
}

void freePlanes(Planes * pls){
	int i,q;
	Plane * p;
	if(pls == NULL)
		return;
	q = pls->qPlanes;

	#ifdef DEBUG
		fprintf(stdout, "Free array planes in (%p)\n", pls->planes);
		fprintf(stdout, "Free struct in (%p)\n", pls);
	#endif
	for(i = 0; i < q;i++){
		p = (pls->planes)+i;
		freeExpandedPlane(p, TRUE);
	}
	free(pls->planes);
	free(pls);
}

void freeExpandedReservation(Reservation * res, int isArray){
	if(res == NULL)
		return;
	#ifdef DEBUG
		fprintf(stdout, "Free FLIGHTCODE STR in (%p)\n", res->flightCode);
		fprintf(stdout, "Free PASSPORTID STR in (%p)\n", res->passportID);
		if(isArray == FALSE)
			fprintf(stdout, "Free struct in (%p)\n", res);
	#endif
	free(res->flightCode);
	free(res->passportID);
	if(isArray == FALSE)
		free(res);
}

void freeExpandedPlane(Plane * pl, int isArray){
	if(pl == NULL)
		return;
	#ifdef DEBUG
	fprintf(stdout, "Free PLANE_MODEL STR in (%p)\n", pl->planeModel);
	#endif
	free(pl->planeModel);
	if(isArray == FALSE)
		free(pl);
}

void freeExpandedFlight(Flight * fl, int isArray){
	if(fl == NULL)
		return;
	#ifdef DEBUG
		fprintf(stdout, "Free FLCODE STR in (%p)\n", fl->flightCode);
		fprintf(stdout, "Free ORIGIN STR in (%p)\n", fl->origin);
		fprintf(stdout, "Free DEST STR in (%p)\n", fl->destination);
		fprintf(stdout, "Free DEPDATE STR in (%p)\n", fl->departureDate);
		fprintf(stdout, "Free ARRDATE STR in (%p)\n", fl->arrivalDate);
		fprintf(stdout, "Free PLANE_MODEL STR in (%p)\n", fl->planeModel);
		if(isArray == FALSE)
			fprintf(stdout, "Free struct in (%p)\n", fl);
	#endif
	free(fl->flightCode);
	free(fl->origin);
	free(fl->destination);
	free(fl->departureDate);
	free(fl->arrivalDate);
	free(fl->planeModel);
	if(isArray == FALSE)
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