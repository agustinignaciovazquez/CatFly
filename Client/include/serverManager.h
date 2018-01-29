#ifndef _SRV_MNG_H_
#define _SRV_MNG_H_

#include "coreStructs.h"

Flights * getFlights_Server(int socket);
simpleMessage * insertFlight_Server(Flight * f, int socket);
simpleMessage * deleteFlight_Server(const char * flightCode, int socket);
Planes * getPlanes_Server(int socket);
simpleMessage * insertPlane_Server(Plane * p, int socket);
simpleMessage * deletePlane_Server(const char * planeModel, int socket);
flightReservations * getReservations_Server(const char * flightCode, int socket);
simpleMessage * insertReservation_Server(Reservation * r, int socket);
simpleMessage * insertCancellation_Server(Reservation * r, int socket);

#endif 