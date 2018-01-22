#ifndef _CORE_STRUCTS_H_
#define _CORE_STRUCTS_H_
#include "constants.h"

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

typedef struct{
	char * flightCode;
	int seatRow;
	int seatColumn;
	char * passportID;
}Reservation;

typedef struct{
	cmd_id command;
	cmd_int extra;
}simpleCommand;

#define MAX_FLIGHTCODE 4+1
#define MAX_ORIGIN 3+1
#define MAX_DESTINATION MAX_ORIGIN
#define MAX_DEP_DATE 16+1
#define MAX_ARR_DATE MAX_DEP_DATE
#define MAX_PLANE_MODEL 10+1
#define MAX_PASSPORTID 10+1

#define FLIGHT_SERIALIZE_BYTES ((MAX_FLIGHTCODE+MAX_ORIGIN+MAX_DESTINATION+MAX_DEP_DATE+MAX_ARR_DATE+MAX_PLANE_MODEL)*sizeof(char))
#define PLANE_SERIALIZE_BYTES ((MAX_PLANE_MODEL)*sizeof(char)+(2*sizeof(int)))
#define RESERVATION_SERIALIZE_BYTES ((MAX_FLIGHTCODE+MAX_PASSPORTID)*sizeof(char)+(2*sizeof(int)))
#define SIMPLE_CMD_SERIALIZE_BYTES (sizeof(cmd_id) + sizeof(cmd_int))
#endif
 
