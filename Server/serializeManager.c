/* Server Core */

#include "serializeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * serializeSimpleCommand(simpleCommand * cmd, int * size){
	char * s;

	s = malloc(SIMPLE_CMD_SERIALIZE_BYTES);
	if(s == NULL)
		return s;

	memcpy(s, &(cmd->command), sizeof(cmd->command));
	s += sizeof(cmd->command);
	memcpy(s, &(cmd->extra), sizeof(cmd->extra));
	s += sizeof(cmd->extra);

	*size = SIMPLE_CMD_SERIALIZE_BYTES;
	return s;
}

int deserializeSimpleCommand(const char * data, int size, simpleCommand * cmd){
	if(size != SIMPLE_CMD_SERIALIZE_BYTES)
		return DESERIALIZE_ERROR;

	memcpy(&(cmd->command), data, sizeof(cmd->command));
	data += sizeof(cmd->command);
	memcpy(&(cmd->extra), data, sizeof(cmd->extra));
	data += sizeof(cmd->extra);
	
	return DESERIALIZE_OK;
}