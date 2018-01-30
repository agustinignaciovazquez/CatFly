#ifndef _INPUT_MNG_H_
#define _INPUT_MNG_H_

#include "coreStructs.h"
#define INPUT_ERROR -7
#define INPUT_OK 7

char getOption();
int getInt(const char * msg);
int getString(const char * msg, char * str, int max);

Plane * getPlaneFromInput();
int checkPlaneInput(Plane * p);
Reservation * getReservationFromInput(const Flight * f);
int checkReservationInput(Reservation * rm);
Flight * getFlightFromInput();
int checkFlightInput(Flight * f);
#endif 