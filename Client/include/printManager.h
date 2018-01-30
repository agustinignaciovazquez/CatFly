#ifndef _PRINT_MNG_H_
#define _PRINT_MNG_H_

#include "coreStructs.h"

#define SEAT_AVAILABLE_DISPLAY '_'
#define SEAT_OCCUPIED_DISPLAY 'X'

void printSimpleMenu();
void printAdminMenu();
void printFlightMenu(Flight * fl);
void printFlights(Flights * fls);
void printFlight(Flight * fl);
void printFlightsMenu(Flights * fls);
void printFlightMin(Flight * fl, int i);
void printPlanes(Planes * pls);
void printPlane(Plane * pl, int i);
void printReservations(char * * reservations, Plane * p);

#endif 
