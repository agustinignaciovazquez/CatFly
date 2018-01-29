#ifndef _DB_MNGR_H_
#define _DB_MNGR_H_

#include <sqlite3.h>
#include "coreStructs.h"

#define INVALID_SEAT_OR_PLANE 9

#define RESPONSE_INSERT_FLIGHT_OK "Flight added successfully"
#define RESPONSE_INSERT_PLANE_OK "Plane added successfully"
#define RESPONSE_DELETE_FLIGHT_OK "Flight removed successfully"
#define RESPONSE_DELETE_PLANE_OK "Plane removed successfully"

#define _SEMAPHORE_FLIGHTS_NAME_ "__FLIGHTS__SEM__DB__"
#define _SEMAPHORE_PLANES_NAME_ "__PLANES__SEM__DB__"
#define _SEMAPHORE_RESERVATION_NAME_ "__RESERVATION__SEM__DB__"

Flights * getFlights_DB(sqlite3 * db);
simpleMessage * insertFlight_DB(Flight * f, sqlite3 * db);
simpleMessage * deleteFlight_DB(const char * flightCode, sqlite3 * db);
Planes * getPlanes_DB(sqlite3 * db);
simpleMessage * insertPlane_DB(Plane * p, sqlite3 * db);
simpleMessage * deletePlane_DB(const char * planeModel, sqlite3 * db);
flightReservations * getFlightReservations_DB(const char * flightCode, sqlite3 * db);
Reservations * getUserReservations_DB(const char * passportID, sqlite3 * db);
simpleMessage * insertReservation_DB(Reservation * r, sqlite3 * db);
simpleMessage * insertCancellation_DB(Reservation * r, sqlite3 * db);
#endif
 
