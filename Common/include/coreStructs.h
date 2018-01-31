#ifndef _CORE_STRUCTS_H_
#define _CORE_STRUCTS_H_
#include "constants.h"

//TYPEDEFS 
typedef unsigned char cmd_id;
typedef int cmd_int;

typedef struct {
	char * flightCode;
	char * origin;
	char * destination;
	char * departureDate;
	char * arrivalDate;
	char * planeModel;
}Flight;

typedef struct{
	int qFlights;
	Flight * flights;
}Flights;

typedef struct{
	char * planeModel;
	int rows;
	int columns;
}Plane;

typedef struct{
	int qPlanes;
	Plane * planes;
}Planes;

typedef struct{
	char * flightCode;
	int seatRow;
	int seatColumn;
	char * passportID;
}Reservation;

typedef struct{
	int qReservations;
	Reservation * reservations;
}Reservations;

typedef struct{
	int seatRow;
	int seatColumn;
}ReservationMinimal;

typedef struct {
	char * flightCode;
	Plane * planeSeats;
	int qReservations;
	ReservationMinimal * reservations;
}flightReservations;

typedef struct{
	cmd_id command;
	cmd_int extra;
}simpleCommand;

typedef struct{
	cmd_id command;
	char * msg;
}simpleMessage;

//SQL Constraints
#define FLIGHTCODE_LEN 4
#define ORIGIN_LEN 3
#define DESTINATION_LEN ORIGIN_LEN
#define DEP_DATE_LEN 16
#define ARR_DATE_LEN DEP_DATE_LEN
#define PLANE_MAX_MODEL_LEN 10
#define MAX_PASSPORTID_LEN 10
#define MIN_PASSPORTID_LEN 7

//Serialize MAXS the 0 counts
#define MAX_FLIGHTCODE FLIGHTCODE_LEN+1
#define MAX_ORIGIN ORIGIN_LEN+1
#define MAX_DESTINATION DESTINATION_LEN+1
#define MAX_DEP_DATE DEP_DATE_LEN+1
#define MAX_ARR_DATE ARR_DATE_LEN+1
#define MAX_PLANE_MODEL PLANE_MAX_MODEL_LEN+1
#define MAX_PASSPORTID MAX_PASSPORTID_LEN+1
#define MAX_MESSAGE_LENGTH 1000

//SERVER/CLIENT BYTES
#define CMD_BYTES sizeof(cmd_id)
#define CODE_BYTES sizeof(cmd_int)
#define RESPONSE_TOT_BYTES (CMD_BYTES+ CODE_BYTES)
#define LENGTH_TOT_BYTES (CMD_BYTES+ CODE_BYTES)
#define SERVER_MAX_INPUT_LENGTH (MAX_MESSAGE_LENGTH-1)

#define FLIGHT_SERIALIZE_BYTES ((MAX_FLIGHTCODE+MAX_ORIGIN+MAX_DESTINATION+MAX_DEP_DATE+MAX_ARR_DATE+MAX_PLANE_MODEL)*sizeof(char))
#define PLANE_SERIALIZE_BYTES ((MAX_PLANE_MODEL)*sizeof(char)+(2*sizeof(int)))
#define RESERVATION_SERIALIZE_BYTES ((MAX_FLIGHTCODE+MAX_PASSPORTID)*sizeof(char)+(2*sizeof(int)))
#define RESERVATION_MINIMAL_SERIALIZE_BYTES (2*sizeof(int))
#define FLIGHT_RESERVATION_SERIALIZE_BYTES (MAX_FLIGHTCODE*sizeof(char)+PLANE_SERIALIZE_BYTES+sizeof(int))
#define FLIGHTS_SERIALIZE_BYTES (sizeof(int))
#define PLANES_SERIALIZE_BYTES (sizeof(int))
#define RESERVATIONS_SERIALIZE_BYTES (sizeof(int))
#define SIMPLE_CMD_SERIALIZE_BYTES (CMD_BYTES + CODE_BYTES)
#define SIMPLE_MSG_SERIALIZE_BYTES (CMD_BYTES + MAX_MESSAGE_LENGTH*sizeof(char))

#endif
 
