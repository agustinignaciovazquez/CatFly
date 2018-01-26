#ifndef _COMMANDPARSER_H_
#define _COMMANDPARSER_H_

#define SQL_ERROR -1
#define PARSE_ERROR -2
#define RESPONSE_ERROR -3
#define RESPONSE_NO -4
#define RESPONSE_OK 1
#define RESPONSE_OK_AND_DISCONNECT 2

#define DISCONNECT_CODE "GOODBYE"
#define DISCONNECT_CODE_LEN 8

int parseRequest(const char * command, int size, char * * response, int * response_bytes);
int verifyResponseFromClient(const char * data, int bytes, int responseID);
int veryfyLengthResponse(const char * data, int bytes);

#endif
 
