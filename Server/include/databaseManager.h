#ifndef _DB_MNGR_H_
#define _DB_MNGR_H_

#include <sqlite3.h>
#include "coreStructs.h"

#define SUCCESS_INSERT_FLIGHT_STR "Flight added successfully"

#define _SEMAPHORE_FLIGHTS_NAME_ "__FLIGHTS__SEM__DB__"

Flights * getFlights_DB(sqlite3 * db);
simpleMessage * insertFlight_DB(Flight * f, sqlite3 * db);
simpleMessage * deleteFlight_DB(const char * flightCode, sqlite3 * db);
Planes * getPlanes(sqlite3 * db);
simpleMessage * insertPlane_DB(Plane * p, sqlite3 * db);
simpleMessage * deletePlane_DB(const char * planeModel, sqlite3 * db);
#endif
 
