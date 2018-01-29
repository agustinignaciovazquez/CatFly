#ifndef _EXPAND_MNGR_H_
#define _EXPAND_MNGR_H_

#include "coreStructs.h"

#define EXPAND_ERROR -77
#define EXPAND_OK 1

Flight * expandFlight(Flight * fl);
Plane * expandPlane(Plane * pl);
Reservation * expandReservation(Reservation * res);
ReservationMinimal * expandReservationMinimal(ReservationMinimal * res);
Reservations * expandReservations();
Flights * expandFlights();
Planes * expandPlanes();
flightReservations * expandFlightReservations();
simpleMessage * expandSimpleMessage();

int addUserReservation(Reservations * expanded, const Reservation * r);
int addFlightReservation(flightReservations * expanded, const ReservationMinimal * rs);
int addFlight(Flights * expanded, const Flight * fl);
int addPlane(Planes * expanded, const Plane * pl);
void freeExpandedFlight(Flight * fl, int isArray);
void freeExpandedPlane(Plane * pl, int isArray);
void freeExpandedReservation(Reservation * res, int isArray);
void freeFlights(Flights * fls);
void freePlanes(Planes * pls);
void freeUserReservations(Reservations * res);
void freeFlightReservations(flightReservations * frs);
void freeExpandedSimpleMessage(simpleMessage * sMgs);
void setFlightReservationsSettings(flightReservations * frs, const char * flightCode, const Plane * pl);
void setSimpleMessageSettings(simpleMessage * sMsgs, cmd_id cmd,const char * msg);
void setSimpleMessageSettings_w_bytes(simpleMessage * sMsgs, cmd_id cmd, const char * msg, int msg_bytes);
#endif
 