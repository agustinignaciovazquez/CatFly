#include "serverCore.h"
#include "semaphores.h"
#include "databaseCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT  7777

int main(int argc , char *argv[]){
  	int status;

  	if(checkDB() != SQLITE_OK){
  		printf("Please install the DB prior execution of Server\n");
  		return SERVER_DB_ERROR;
  	}

  	status = initializeServer(SERVER_ADDR,SERVER_PORT);
  	if(status == SERVER_INIT_ERROR){
  		printf("Server could not be initiated please check address (%s) and port (%d) \n",SERVER_ADDR,SERVER_PORT);
  	}
  	
    return status;
}  
