#ifndef _SER_MNGR_H_
#define _SER_MNGR_H_

#include "coreStructs.h"

#define DESERIALIZE_ERROR -1
#define DESERIALIZE_OK 1

char * serializeSimpleCommand(simpleCommand * cmd, int * size);
int deserializeSimpleCommand(const char * data, int size, simpleCommand * cmd);

#endif
 
