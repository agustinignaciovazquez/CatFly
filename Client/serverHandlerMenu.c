#include "serverHandlerMenu.h" 
#include "menuManager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int serverHandler(int socket){
	int con_status;
	int isAdmin;
	isAdmin = displaySelection();
	con_status = getHelloFromServer(socket, isAdmin);

	//Verify we are connected to the server w/ unique Hello command
	if(con_status != HELLO_OK)
		return HELLO_ERROR;
	
	displayMenu(isAdmin, socket);

	return con_status;
}