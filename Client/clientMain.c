#include "clientCore.h"
#include "coreStructs.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define SERVER_ADDR NULL
#define SERVER_PORT 7777


int main(int argc , char *argv[]){
  	int status;
  	char * address = SERVER_ADDR;
  	int port = SERVER_PORT;

  	status = connectToServer(address,port);
  	if(status == SERVER_CONNECTION_ERROR){
  		printf("Could not reach server ... please check address (%s) and port (%d) \n",(address == NULL)? "ANYADDR":address,port);
  	}
  	
    return status;
}  
