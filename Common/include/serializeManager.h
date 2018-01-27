#ifndef _SER_MNGR_H_
#define _SER_MNGR_H_

#include "coreStructs.h"

char * serializeSimpleCommand(simpleCommand * cmd, int * size);
char * serializePlane(Plane * pl, int * size);
char * serializeReservation(Reservation * res, int * size);
char * serializeFlight(Flight * fl, int * size);
char * serializeFlights(Flights * fl, int * size);
char * serializePlanes(Planes * pl, int * size);
char * serializeFlightReservations(flightReservations * fres, int * size);
char * serializeSimpleMessage(simpleMessage * smgs, int * size);
void freeSerialized(char * s);

#endif
 
