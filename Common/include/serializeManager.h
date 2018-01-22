#ifndef _SER_MNGR_H_
#define _SER_MNGR_H_

#include "coreStructs.h"

char * serializeSimpleCommand(simpleCommand * cmd, int * size);
char * serializePlane(Plane * pl, int * size);
char * serializeReservation(Reservation * res, int * size);
void freeSerialized(char * s);

#endif
 
