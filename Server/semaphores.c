/* SEMAPHORES IMPLEMENTATION*/
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>


sem_t * openSemaphore(const char * semaphoreKey, int val){
    sem_t * semaphore;
    
    semaphore = sem_open (semaphoreKey, O_CREAT, 0644, val); 

    if(semaphore == SEM_FAILED){
        printf("Error: Opening semaphore %s\n", semaphoreKey);
        return NULL;
    }
    //Destroy semaphore once all other processes that have the semaphore open close it.
    sem_unlink (semaphoreKey); 

    return semaphore;
}

sem_t * openMutexSemaphore(const char * semaphoreKey){
	return openSemaphore(semaphoreKey,1);
}

sem_t * openChildsSemaphore(const char * semaphoreKey){
	return openSemaphore(semaphoreKey,MAX_CHILD_PROCESS);
}