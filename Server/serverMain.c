#include "serverCore.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT  7777

int main(int argc , char *argv[]){
  	int status;
  	
  	status = initializeServer(SERVER_ADDR,SERVER_PORT);
  	if(status == SERVER_INIT_ERROR){
  		printf("Error: Server could not be initiated\n");
  	}
  	
    return 0;
}  
