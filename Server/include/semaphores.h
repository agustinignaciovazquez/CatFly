#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H_

//Link with gcc -pthread.
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>

#define MAX_CHILD_PROCESS 10

sem_t * openSemaphore(const char * semaphoreKey, int val);
sem_t * openMutexSemaphore(const char * semaphoreKey);
sem_t * openChildsSemaphore(const char * semaphoreKey);

#endif

