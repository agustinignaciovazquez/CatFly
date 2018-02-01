#ifndef _CONS_H_
#define _CONS_H_

#define TRUE 1
#define FALSE 0

//#define DEBUG //COMMENT THIS IN FINAL REV

#define OCCUPIED_SEAT 1

//SERVER/CLIENT CMDS
#define GET_FLIGHTS_CMD 'F'
#define GET_FLIGHT_RESERVATIONS_CMD 'I'
#define INSERT_FLIGHT_CMD 'A'
#define DELETE_FLIGHT_CMD 'D'
#define GET_PLANES_CMD 'E'
#define INSERT_PLANE_CMD 'P'
#define DELETE_PLANE_CMD 'G'
#define GET_USER_RESERVATIONS_CMD 'U'
#define GET_CANCELLATIONS_CMD 'C'
#define INSERT_FLIGHT_RESERVATION_CMD 'R'
#define DELETE_FLIGHT_RESERVATION_CMD 'J'
#define LENGTH_CODE_CMD 'L'
#define REQUEST_CODE_CMD 'T'
#define RESPONSE_CODE_CMD 'r'
#define HELLO_CODE_CMD 'V'
#define HELLO_ADMIN_CODE_CMD 'v'
#define ERROR_CODE_CMD 'X'
#define DISCONNECT_CMD 'Q'
#define BACK_CMD 'B'

//SERVER/CLIENT CODES
#define CLIENT_RESPONSE_LENGTH_OK 200
#define CLIENT_RESPONSE_LENGTH_ERROR -200 
#define CLIENT_HELLO_OK 500
#define CLIENT_ADMIN_HELLO_OK 777
#define SERVER_HELLO_OK 501
#define SERVER_RESPONSE_INSERT_FLIGHT_OK "Flight added successfully"
#define SERVER_RESPONSE_INSERT_PLANE_OK "Plane added successfully"
#define SERVER_RESPONSE_INSERT_RES_OK "Reservation added successfully"
#define SERVER_RESPONSE_INSERT_RES_ERROR "Error adding reservation. Try again later."
#define SERVER_RESPONSE_INSERT_RES_NOT_AVAILABLE "Error seats are already occupied. Please select other"
#define SERVER_RESPONSE_INSERT_CANCELATION_OK "Reservation removed successfully"
#define SERVER_RESPONSE_INSERT_CANCELATION_INCORRECT_PASSPORT "Passport ID does not matches with reservated seats"
#define SERVER_RESPONSE_INSERT_CANCELATION_ERROR "Error removing reservation. Try again later."
#define SERVER_RESPONSE_DELETE_FLIGHT_OK "Flight removed successfully"
#define SERVER_RESPONSE_DELETE_PLANE_OK "Plane removed successfully"
#endif
 
