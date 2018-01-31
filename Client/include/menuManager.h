#ifndef _MENU_MNG_H_
#define _MENU_MNG_H_

#include "coreStructs.h"
int displaySelection();
void displayMenu(int isAdmin, int socket);
//Shared functions between menuAdmin and menuSimple
void displayFlightsMenu(int socket);
void displayReservationsMenu(int socket);
#endif 
