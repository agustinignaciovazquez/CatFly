#ifndef _EXPAND_MNGR_H_
#define _EXPAND_MNGR_H_

#include "coreStructs.h"
#include "constants.h"

#define EXPAND_ERROR 0
#define EXPAND_OK 1

Flight * expandFlight(Flight * fl);
Plane * expandPlane(Plane * pl);
Flights * expandFlights();
Planes * expandPlanes();
Reservation * expandReservation();
ReservationMinimal * expandReservationMinimal(ReservationMinimal * res);
flightReservations * expandFlightReservations(const char * flightCode, Plane * pl);
int addReservation(flightReservations * expanded, const ReservationMinimal * rs);
int addFlight(Flights * expanded, const Flight * fl);
int addPlane(Planes * expanded, const Plane * pl);
void freeExpandedFlight(Flight * fl);
void freeExpandedPlane(Plane * pl);
void freeExpandedReservation(Reservation * res);
void freeFlights(Flights * fls);
void freePlanes(Planes * pls);
void freeFlightReservations(flightReservations * frs);
#endif
 