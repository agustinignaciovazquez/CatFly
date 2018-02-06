#include "serializeTest.h"
#include "serverSecurityTests.h"
#include "DoSTest.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

int main(int argc , char *argv[]){
	//Serialize/Deserialize Tests
	/*testSerialSimpleCMD();
	testSerialFlight();
	testSerialFlights();
	testSerialPlanes();
	testSerialRes();
	testSerialFlightRes();
	testSerialSimpleMsg();*/
	//Server Tests
	fuzzTest();
	fuzzTest_in_HELLO_REQ();
	notAdminTest();
	//simpleDDoSS();
    return 0;
}  
 
