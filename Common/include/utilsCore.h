#ifndef _UTILS_H_
#define _UTILS_H_

#include "coreStructs.h"

int copyStr(char * str, const char * data,  int max);
int copyBytes(void * dest, const void * data, int bytes);
char * * dinamicMatrix(unsigned int rows, unsigned int columns);
void freeMatrix(char * * matrix, unsigned int rows);

#endif 
