#ifndef _EXPAND_MNGR_H_
#define _EXPAND_MNGR_H_

#include "coreStructs.h"
#include "constants.h"

#define EXPAND_ERROR 0
#define EXPAND_OK 1

Flight * expandFlight();
Plane * expandPlane();
Reservation * expandReservation();
void freeExpandedFlight(Flight * fl);
void freeExpandedPlane(Plane * pl);
void freeExpandedReservation(Reservation * res);
#endif
 