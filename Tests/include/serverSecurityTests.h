#ifndef _SERVER_TESTS_H_
#define _SERVER_TESTS_H_

#include "coreStructs.h"

#define SERVER_ADDR NULL
#define SERVER_PORT 7777

void fuzzTest();
void fuzzTest_in_HELLO_REQ();
void notAdminTest();
int sendFuzzData(int socket);
#endif 
