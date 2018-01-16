#include "commandParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

char * getResponse(const char * command, int size, int * response_bytes){
	*response_bytes = 5;
	return "HOLA";
} 
