#ifndef _MENU_MNG_H_
#define _MENU_MNG_H_

#include "coreStructs.h"
#include "menuAdminManager.h"

void displaySimpleMenu(int socket);
int displayFlightsMenu(int socket);
int displayFlightMenu(Flight * fl, int socket);
int displayReservationsMenu(int socket);
int displayReservationsMenu(int socket);
int displayFlightReservations(Flight * fl, int socket);
int displayInsertFlightReservation(Flight * fl, int socket);
int displayUserReservationsMenu(int socket);
int displayDeleteUserReservationsMenu(int socket);
Reservations * getUserReservationsById(int socket);
void displayCancellation(Reservation * r, int socket);

#endif 