#ifndef _DESER_MNGR_H_
#define _DESER_MNGR_H_

#include "coreStructs.h"

#define DESERIALIZE_ERROR -1

cmd_id deserializeCommand(const char * * command, int * size);

int deserializeSimpleCommand(const char * data, int size, simpleCommand * cmd);
int deserializeFlight(const char * data, int size, Flight * fl);
int deserializeFlights(const char * data, int size, Flights * fls);
int deserializePlane(const char * data, int size, Plane * pl);
int deserializePlanes(const char * data, int size, Planes * pls);
int deserializeReservation(const char * data, int size, Reservation * res);
int deserializeReservationMin(const char * data, int size, ReservationMinimal * res);
int deserializeFlightReservations(const char * data, int size, flightReservations * res);
int deserializeSimpleMessage(const char * data, int size, simpleMessage * sm);
#endif
 