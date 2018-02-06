#include "clientCore.h"
#include "serverHandlerMenu.h" 
#include "coreStructs.h"
#include "constants.h"
#include "printManager.h"
#include "utilsCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define SERVER_ADDR NULL
#define SERVER_PORT 7777

int connectToServer(const char * address, int port);

int main(int argc , char *argv[]){
  	int status;
  	char * address = SERVER_ADDR;
  	int port = SERVER_PORT;

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

int connectToServer(const char * address, int port){
  int socket;

    socket = createSocket(address, port);
    if(socket == SERVER_CONNECTION_ERROR)
      return SERVER_CONNECTION_ERROR;
    
    return serverHandler(socket);
} 