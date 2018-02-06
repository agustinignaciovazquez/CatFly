#include "DoSTest.h"
#include "serverSecurityTests.h" //SERVER ADDR AND PORT
#include "serverManager.h"
#include "serverHandlerCore.h" //Send and recive functions
#include "clientCore.h" //createSocket
#include "constants.h"
#include "expandManager.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <time.h>

#define DDOSS_SEMPAHORE_NAME "___DDOSS_TEST_SEM___"
#define MAX_CHILDS 10

void simpleAttack();

void simpleDDoSS(){
	int pid;
	sem_t * sem;

	printf("\n\nServer DDOSS test\n");
	sem = openSemaphore(DDOSS_SEMPAHORE_NAME, MAX_CHILDS);
	while(TRUE){
		sem_wait(sem);
		pid = fork();
		if(pid < 0){
			fprintf(stderr,"Error: Fork failed \n");
			exit(-1);			
		}

		if(pid == 0){
			//Child process
			simpleAttack();
			sem_post(sem);
			exit(0); 
		}else{
			//do nothing
		}
	}
	sem_close(sem);

}
void simpleAttack(){
	int socket;
	while(TRUE){
		socket = createSocket(SERVER_ADDR, SERVER_PORT);
	    if(socket == SERVER_CONNECTION_ERROR){
	    	printf("Error server unreacheable\n");
	    	return;
	    }

		sendFuzzData(socket);
		close(socket);
	}
}