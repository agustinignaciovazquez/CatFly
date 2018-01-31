#ifndef _PRINT_MNG_H_
#define _PRINT_MNG_H_

#include "coreStructs.h"

#define SEAT_AVAILABLE_DISPLAY '_'
#define SEAT_OCCUPIED_DISPLAY 'X'

#define ENTER_AS_USER 1
#define ENTER_AS_ADMIN 2

void printSelectionMenu();
void printSimpleMenu();
void printAdminMenu();
void printFlightMenu(Flight * fl);
void printFlights(Flights * fls);
void printFlight(Flight * fl);
void printFlightsMenu(Flights * fls);
void printFlightMin(Flight * fl, int i);
void printPlanesMenu(Planes * pls);
void printPlanes(Planes * pls);
void printPlane(Plane * pl, int i);
void printPlaneMin(Plane * pl);
void printReservationsMenu(Reservations * res);
void printUserReservation(Reservation * res);
void printUserReservations(Reservations * res);
void printReservations(char * * reservations, Plane * p);

#endif 
