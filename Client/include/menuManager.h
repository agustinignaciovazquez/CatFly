#ifndef _MENU_MNG_H_
#define _MENU_MNG_H_

#include "coreStructs.h"

#define STATUS_ERROR_NOT_EXISTS -3
#define STATUS_ERROR_MATRIX_EXPAND -2
#define STATUS_OK 14

int displaySelection();
void displayMenu(int isAdmin, int socket);
//Shared functions between menuAdmin and menuSimple
int displayFlightsMenu(int socket);
int displayReservationsMenu(int socket);
#endif 
