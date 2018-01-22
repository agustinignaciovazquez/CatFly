#ifndef _DESER_MNGR_H_
#define _DESER_MNGR_H_

#include "coreStructs.h"
#include "constants.h"

#define DESERIALIZE_ERROR -1
#define DESERIALIZE_OK 1

cmd_id deserializeCommand(const char * * command, int * size);

int deserializeSimpleCommand(const char * data, int size, simpleCommand * cmd);


#endif
 