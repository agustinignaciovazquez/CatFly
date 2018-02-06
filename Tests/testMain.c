#include "serializeTest.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

int main(int argc , char *argv[]){
	testSerialSimpleCMD();
	testSerialFlight();
	testSerialFlights();
	testSerialPlanes();
	testSerialRes();
	testSerialFlightRes();
	testSerialSimpleMsg();
    return 0;
}  
 
