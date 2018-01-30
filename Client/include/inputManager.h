#ifndef _INPUT_MNG_H_
#define _INPUT_MNG_H_

#include "coreStructs.h"
#define UNEXPECTED_ERROR -7

char getOption();
int getInt();
int getString(char * str, int max);
int checkReservationInput(Plane * p, char * * res, Reservation * rm);
ReservationMinimal getSeat();
Reservation * getSeatForReservation(const Flight * f);

#endif 