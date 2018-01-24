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

#define MAX_FLIGHTCODE 4+1
#define MAX_ORIGIN 3+1
#define MAX_DESTINATION MAX_ORIGIN
#define MAX_DEP_DATE 16+1
#define MAX_ARR_DATE MAX_DEP_DATE
#define MAX_PLANE_MODEL 10+1
#define MAX_PASSPORTID 10+1
#define MAX_MESSAGE_LENGTH 1000

#define FLIGHT_SERIALIZE_BYTES ((MAX_FLIGHTCODE+MAX_ORIGIN+MAX_DESTINATION+MAX_DEP_DATE+MAX_ARR_DATE+MAX_PLANE_MODEL)*sizeof(char))
#define PLANE_SERIALIZE_BYTES ((MAX_PLANE_MODEL)*sizeof(char)+(2*sizeof(int)))
#define RESERVATION_SERIALIZE_BYTES ((MAX_FLIGHTCODE+MAX_PASSPORTID)*sizeof(char)+(2*sizeof(int)))
#define RESERVATION_MINIMAL_SERIALIZE_BYTES (2*sizeof(int))
#define FLIGHT_RESERVATION_SERIALIZE_BYTES (MAX_FLIGHTCODE*sizeof(char)+PLANE_SERIALIZE_BYTES+sizeof(int))
#define FLIGHTS_SERIALIZE_BYTES (sizeof(int))
#define PLANES_SERIALIZE_BYTES (sizeof(int))
#define SIMPLE_CMD_SERIALIZE_BYTES (sizeof(cmd_id) + sizeof(cmd_int))
#define SIMPLE_MSG_SERIALIZE_BYTES (sizeof(cmd_id) + MAX_MESSAGE_LENGTH*sizeof(char))
#endif
 
