#include "clientCore.h"
#include "coreStructs.h"
#include "constants.h"
#include "printManager.h"
#include "utilsCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define SERVER_ADDR NULL
#define SERVER_PORT 7777


int main(int argc , char *argv[]){
  	int status;
  	char * address = SERVER_ADDR;
  	int port = 7777;
    char * * res = dinamicMatrix(30,30);
    printReservations(res,30,30);
  	status = connectToServer(address,port);
  	switch(status){
  		case SERVER_CONNECTION_ERROR:
  			printf("Could not reach server ... please check address (%s) and port (%d) \n",(address == NULL)? "ANYADDR":address,port);
  			break;
  		case HELLO_ERROR:
  			printf("Hello request failed ... check address (%s) and port (%d) is the correct Server\n",(address == NULL)? "ANYADDR":address,port);
  			break;
  	}
  	
    return status;
}  
