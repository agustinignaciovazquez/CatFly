
#include "deserializeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cmd_id deserializeCommand(const char * * command, int * size){
	cmd_id action;
	if(*size < sizeof(cmd_id))
		return DESERIALIZE_ERROR; 

	memcpy(&action, *command, sizeof(cmd_id));
	*command += sizeof(cmd_id); 
	*size -= sizeof(cmd_id);
	return action;
}

int deserializeSimpleCommand(const char * data, int size, simpleCommand * cmd){
	if(size != SIMPLE_CMD_SERIALIZE_BYTES)
		return DESERIALIZE_ERROR;
	cmd->command = deserializeCommand(&data, &size); //Never returns PARSE_ERROR because previous check
	memcpy(&(cmd->extra), data, sizeof(cmd->extra));
	data += sizeof(cmd->extra);
	
	return DESERIALIZE_OK;
}