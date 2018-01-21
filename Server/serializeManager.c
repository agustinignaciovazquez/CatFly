/* Server Core */

#include "serializeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * serializeSimpleCommand(simpleCommand * cmd, int * size){
	char * s, *aux;
	s = malloc(SIMPLE_CMD_SERIALIZE_BYTES);
	if(s == NULL)
		return s;
	aux = s;
	memcpy(aux, &(cmd->command), sizeof(cmd->command));
	aux += sizeof(cmd->command);
	memcpy(aux, &(cmd->extra), sizeof(cmd->extra));
	aux += sizeof(cmd->extra);

	*size = SIMPLE_CMD_SERIALIZE_BYTES;
	return s;
}

