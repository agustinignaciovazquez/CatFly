#ifndef _CONS_H_
#define _CONS_H_

#define TRUE 1
#define FALSE 0

#define DEBUG //COMMENT THIS IN FINAL REV

//SERVER/CLIENT CMDS
#define GET_FLIGHTS_CMD 'A'
#define GET_FLIGHT_CANCELATIONS_CMD 'B'
#define INSERT_FLIGHT_CMD 'C'
#define DELETE_FLIGHT_CMD 'D'
#define GET_PLANES_CMD 'E'
#define INSERT_PLANE_CMD 'F'
#define DELETE_PLANE_CMD 'G'
#define GET_FLIGHT_RESERVATION_CMD 'H'
#define INSERT_FLIGHT_RESERVATION_CMD 'I'
#define DELETE_FLIGHT_RESERVATION_CMD 'J'
#define LENGTH_CODE_CMD 'L'
#define REQUEST_CODE_CMD 'T'
#define RESPONSE_CODE_CMD 'R'
#define HELLO_CODE_CMD 'V'
#define HELLO_ADMIN_CODE_CMD 'v'
#define ERROR_CODE_CMD 'X'
#define DISCONNECT_CMD 'Q'

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
 
