#ifndef _COMMANDPARSER_H_
#define _COMMANDPARSER_H_

#define SQL_ERROR -1
#define PARSE_ERROR -2
#define RESPONSE_OK 1
#define RESPONSE_OK_AND_DISCONNECT 2

#define SQL_ERROR_CODE "SQL_ERROR"
#define SQL_ERROR_CODE_LEN 10
#define SQL_DISCONNECT_CODE "GOODBYE"
#define SQL_DISCONNECT_CODE_LEN 8

int parseRequest(const char * command, int size, char * * response, int * response_bytes);

#endif
 
