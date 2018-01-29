#include "serverManager.h"
#include "serverHandlerCore.h"
#include "serializeManager.h"
#include "deserializeManager.h"
#include "expandManager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

Flights * getFlights_Server(int socket){
	int status,bytes;
	char * read_buffer;
	Flights * fls;
	simpleCommand request = {.command = GET_FLIGHTS_CMD, .extra = 0};
	
	status = sendSimpleCmd(socket, &request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	fls = expandFlights();
	if(fls == NULL || deserializeFlights(read_buffer, bytes, fls) == DESERIALIZE_ERROR){
		freeFlights(fls); //If fls is a null pointer, no action occurs.
		fls = NULL;
	}

	free(read_buffer);
	return fls;
}

simpleMessage * insertFlight_Server(Flight * f, int socket){
	int status, bytes,ser_bytes;
	char * serialized, * read_buffer;
	simpleMessage  * request, * response;

	serialized = serializeFlight(f, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, INSERT_FLIGHT_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandSimpleMessage();
	if(response == NULL || deserializeSimpleMessage(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeExpandedSimpleMessage(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}

simpleMessage * deleteFlight_Server(Flight * f, int socket){
	int status, bytes,ser_bytes;
	char * serialized, * read_buffer;
	simpleMessage  * request, * response;

	serialized = serializeFlight(f, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, DELETE_FLIGHT_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandSimpleMessage();
	if(response == NULL || deserializeSimpleMessage(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeExpandedSimpleMessage(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}

Planes * getPlanes_Server(int socket){
	int status, bytes;
	char * read_buffer;
	Planes * pls;
	simpleCommand request = {.command = GET_PLANES_CMD, .extra = 0};
	
	status = sendSimpleCmd(socket, &request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	pls = expandPlanes();
	if(pls == NULL || deserializePlanes(read_buffer, bytes, pls) == DESERIALIZE_ERROR){
		freePlanes(pls); //If pls is a null pointer, no action occurs.
		pls = NULL;
	}

	free(read_buffer);
	return pls;
}

simpleMessage * insertPlane_Server(Plane * p, int socket){
	int status, bytes,ser_bytes;
	char * serialized, * read_buffer;
	simpleMessage  * request, * response;

	serialized = serializePlane(p, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, INSERT_PLANE_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandSimpleMessage();
	if(response == NULL || deserializeSimpleMessage(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeExpandedSimpleMessage(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}

simpleMessage * deletePlane_Server(Plane * p, int socket){
	int status, bytes,ser_bytes;
	char * serialized, * read_buffer;
	simpleMessage  * request, * response;

	serialized = serializePlane(p, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, DELETE_PLANE_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandSimpleMessage();
	if(response == NULL || deserializeSimpleMessage(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeExpandedSimpleMessage(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}

flightReservations * getReservations_Server(Flight * f, int socket){
	int status, bytes, ser_bytes;
	char * serialized,* read_buffer;
	simpleMessage  * request;
	flightReservations * response;

	serialized = serializeFlight(f, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, INSERT_FLIGHT_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandFlightReservations();
	if(response == NULL || deserializeFlightReservations(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeFlightReservations(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}

simpleMessage * insertReservation_Server(Reservation * r, int socket){
	int status, bytes,ser_bytes;
	char * serialized, * read_buffer;
	simpleMessage  * request, * response;

	serialized = serializeReservation(r, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, INSERT_FLIGHT_RESERVATION_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandSimpleMessage();
	if(response == NULL || deserializeSimpleMessage(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeExpandedSimpleMessage(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}

simpleMessage * insertCancellation_Server(Reservation * r, int socket){
	int status, bytes,ser_bytes;
	char * serialized, * read_buffer;
	simpleMessage  * request, * response;

	serialized = serializeReservation(r, &ser_bytes);
	if(serialized == NULL)
		return NULL;

	request = expandSimpleMessage();
	if(request == NULL){
		freeSerialized(serialized);
		return NULL;
	}

	setSimpleMessageSettings_w_bytes(request, DELETE_FLIGHT_RESERVATION_CMD, serialized, ser_bytes);
	freeSerialized(serialized);

	status = sendSimpleMsg(socket, request);
	freeExpandedSimpleMessage(request);
	if(status != SEND_DATA_OK)
		return NULL;

	status = getDataAndLengthFromServer(socket,&read_buffer,&bytes);
	if(status != RECEIVE_DATA_OK)
		return NULL;

	response = expandSimpleMessage();
	if(response == NULL || deserializeSimpleMessage(read_buffer, bytes, response) == DESERIALIZE_ERROR){
		freeExpandedSimpleMessage(response); //If response is a null pointer, no action occurs.
		response = NULL;
	}

	free(read_buffer);
	return response;
}