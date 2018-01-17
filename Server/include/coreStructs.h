#ifndef _CORE_STRUCTS_H_
#define _CORE_STRUCTS_H_

typedef struct {
	char * flightCode;
	char * origin;
	char * destination;
	char * departureDate;
	char * arrivalDate;
	char * planeModel;
}Flight;

typedef struct{
	char * planeModel;
	int rows;
	int columns;
}Plane;

#endif
 
