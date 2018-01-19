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

typedef struct{
	char * flightCode;
	int seatRow;
	int seatColumn;
	char * passportID;
}Reservation;

#define MAX_FLIGHTCODE 4+1
#define MAX_ORIGIN 3+1
#define MAX_DESTINATION MAX_ORIGIN
#define MAX_DEP_DATE 16+1
#define MAX_ARR_DATE MAX_DEP_DATE
#define MAX_PLANE_MODEL 10+1
#define MAX_PASSPORTID 10+1

#define MAX_FLIGHT_SERIALIZE ((MAX_FLIGHTCODE+MAX_ORIGIN+MAX_DESTINATION+MAX_DEP_DATE+MAX_ARR_DATE+MAX_PLANE_MODEL)*sizeof(char))
#define MAX_PLANE_SERIALIZE ((MAX_PLANE_MODEL)*sizeof(char)+(2*sizeof(int)))
#define MAX_RESERVATION_SERIALIZE ((MAX_FLIGHTCODE+MAX_PASSPORTID)*sizeof(char)+(2*sizeof(int)))

#endif
 
