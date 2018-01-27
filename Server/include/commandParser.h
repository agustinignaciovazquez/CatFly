#ifndef _COMMANDPARSER_H_
#define _COMMANDPARSER_H_

#include "coreStructs.h"

#define SQL_ERROR -1
#define PARSE_ERROR -2
#define RESPONSE_ERROR -3
#define RESPONSE_NO -4
#define RESPONSE_OK 1
#define RESPONSE_OK_AND_DISCONNECT 2

int parseRequest(const char * command, int size, char * * response, int * response_bytes, int isAdmin);
int verifySimpleCmdFromClient(const char * data, int bytes, cmd_id type_id,int responseID);
int veryfyLengthResponse(const char * data, int bytes);
int verifyHelloResponse(const char * data, int bytes);
int verifyHelloAdminResponse(const char * data, int bytes);
#endif
 
