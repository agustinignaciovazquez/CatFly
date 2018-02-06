#include "serverCore.h"
#include "semaphores.h"
#include "databaseCore.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define SERVER_ADDR NULL
#define SERVER_PORT 7777

int startServer(char * address, int port, int foreground);
int checkAndInstallDB();

int main(int argc , char *argv[]){
    int status;
  	int foreground = FALSE;
  	char * address = SERVER_ADDR;
  	int port = SERVER_PORT;

  	//Check foreground arg
  	if(argc > 1){
  			foreground = (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--foreground") == 0)? TRUE : FALSE;
  	}

  	if(checkAndInstallDB() != SQLITE_OK){
  		  return SERVER_DB_ERROR;
    }
    
  	status = startServer(address,port,foreground);
  	if(status == SERVER_INIT_ERROR){
  		printf("Server could not be initiated please check address (%s) and port (%d) \n",(address == NULL)? "ANYADDR":address,port);
  	}
  	
    return status;
}  

int startServer(char * address, int port, int foreground){
	int status;

	if(foreground){
  		printf("Starting server in background\n");
  		status = initializeServerForeground(address,port);
  	}else{
  		printf("Starting server\n");
  		status = initializeServer(address,port);
  	}
  return status;
}

