#ifndef _DB_MNGR_H_
#define _DB_MNGR_H_

#include <sqlite3.h>
#include "coreStructs.h"

#define SUCCESS_INSERT_FLIGHT_STR "Flight added successfully"

Flights * getFlights_DB(sqlite3 * db);
simpleMessage * insertFlight_DB(Flight * f, sqlite3 * db);

#endif
 
