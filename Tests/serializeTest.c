#include "serializeTest.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define STR_EQUALS(s1,s2) ((strcmp(s1,s2) == 0)? TRUE:FALSE)

void printSimpleCMD(simpleCommand * cmd);
void printFlight(Flight * fl);
void printFlights(Flights * fls);
void printPlane(Plane * pl);
void printPlanes(Planes * pls);
void printFlightReservations(flightReservations * fres);
void printReservationMin(ReservationMinimal * resM, int size);
void printSimpleMsg(simpleMessage * fd);
void printReservation(Reservation * res);
int checkFlight(Flight *f1, Flight * f2);
int checkFlights(Flights *f1, Flights * f2);
int checkPlane(Plane * p1, Plane * p2);
int checkPlanes(Planes * p1, Planes * p2);
int checkSimpleMSG(simpleMessage * s1, simpleMessage * s2);
int checkSimpleCMD(simpleCommand * s1, simpleCommand * s2);
int checkReservation(Reservation * r1, Reservation * r2);
int checkFlightReservations(flightReservations * r1, flightReservations * r2);

void testSerialSimpleCMD(){
	char * bytes_aux;
	int cmd_size;
	simpleCommand n_simpleCmd = {.command = 7, .extra = 7};
	simpleCommand simpleCmd = {.command = 65, .extra = 777};

	printf("\n\nTESTING SIMPLE CMD SER/DES\n");

  bytes_aux = serializeSimpleCommand(&simpleCmd, &cmd_size);
	printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", cmd_size);
	printSimpleCMD(&simpleCmd);

	if(deserializeSimpleCommand(bytes_aux, cmd_size, &n_simpleCmd) == DESERIALIZE_ERROR){
		printf("ERROR DESERIALIZE");
		return;
	}

	printf("DESERIALIZED: %d\n", cmd_size);
	printSimpleCMD(&n_simpleCmd);

  if(checkSimpleCMD(&n_simpleCmd,&simpleCmd) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }
}

void printSimpleCMD(simpleCommand * cmd){
	fprintf(stdout,"struct in : %p\nCommand: %c (at %p)\n", cmd, cmd->command, &(cmd->command));
	fprintf(stdout,"Extra int: %d (at %p)\n", cmd->extra, &(cmd->extra));
} 

int checkSimpleCMD(simpleCommand * s1, simpleCommand * s2){
  if(s1->extra != s2->extra)
    return FALSE;

  if(s1->command != s2->command)
    return FALSE;

  return TRUE;
}

void testSerialSimpleMsg(){
  char * bytes_aux;
  int bytes_size;

  printf("\n\nTESTING SIMPLE MSG SER/DES\n");

  simpleMessage * fd = expandSimpleMessage();
  simpleMessage * fd2 = expandSimpleMessage();
  setSimpleMessageSettings(fd, 'A', "TEST DES SER OK ?? :)(:");

  bytes_aux = serializeSimpleMessage(fd, &bytes_size, 0);
  printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", bytes_size);
  printSimpleMsg(fd);

  if(deserializeSimpleMessage(bytes_aux, bytes_size, fd2) == DESERIALIZE_ERROR){
    printf("ERROR DESERIALIZE");
    return;
  }

  printf("DESERIALIZED: %d\n", bytes_size);
  printSimpleMsg(fd2);

  if(checkSimpleMSG(fd,fd2) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }

  freeExpandedSimpleMessage(fd);
  freeExpandedSimpleMessage(fd2);
}

void printSimpleMsg(simpleMessage * fd){
  fprintf(stdout,"struct in : %p\nCommand: %c (at %p)\n", fd, fd->command, &(fd->command));
  fprintf(stdout,"Message: %s (at %p)\n", fd->msg, (fd->msg));
}

int checkSimpleMSG(simpleMessage * s1, simpleMessage * s2){
  if(STR_EQUALS(s1->msg,s2->msg) == FALSE)
    return FALSE;

  if(s1->command != s2->command)
    return FALSE;

  return TRUE;
}

void testSerialFlight(){
  char * bytes_aux;
  int bytes_size;

  printf("\n\nTESTING FLIGHT SER/DES\n");

  Flight f1 = {.flightCode = "AAA1", .origin = "EZE", .destination = "LAX", .departureDate = "depda1", .arrivalDate = "arrda1",.planeModel = "model1"};
  Flight * fd = expandFlight(NULL);

  bytes_aux = serializeFlight(&f1, &bytes_size);
  printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", bytes_size);
  printFlight(&f1);

  if(deserializeFlight(bytes_aux, bytes_size, fd) == DESERIALIZE_ERROR){
    printf("ERROR DESERIALIZE");
    return;
  }

  printf("DESERIALIZED: %d\n", bytes_size);
  printFlight(fd);

  if(checkFlight(&f1,fd) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }

  freeExpandedFlight(fd, FALSE);
}

void testSerialFlights(){
  char * bytes_aux;
  int bytes_size;

  printf("\n\nTESTING FLIGHTS SER/DES\n");

  Flights * fls = expandFlights();
  Flights * fls_d = expandFlights();

  Flight f1 = {.flightCode = "AAA1", .origin = "EZE", .destination = "LAX", .departureDate = "depda1", .arrivalDate = "arrda1",.planeModel = "model1"};
  Flight f2 = {.flightCode = "BBB1", .origin = "LAX", .destination = "EZE", .departureDate = "depda2", .arrivalDate = "arrda2",.planeModel = "model2"};
  Flight f3 = {.flightCode = "CCC1", .origin = "MIA", .destination = "LAX", .departureDate = "depda3", .arrivalDate = "arrda3",.planeModel = "model3"};

  addFlight(fls, &f1);
  addFlight(fls, &f2);
  addFlight(fls, &f3);

  bytes_aux = serializeFlights(fls, &bytes_size);
  printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", bytes_size);
  printFlights(fls);

  if(deserializeFlights(bytes_aux, bytes_size, fls_d) == DESERIALIZE_ERROR){
    printf("ERROR DESERIALIZE");
    return;
  }

  printf("DESERIALIZED: %d\n", bytes_size);
  printFlights(fls_d);

  if(checkFlights(fls,fls_d) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }

  freeFlights(fls_d);
  freeFlights(fls);
}

void printFlight(Flight * fl){
  fprintf(stdout,"struct in : %p\nFlight Code: %s (at %p)\n", fl, fl->flightCode, (fl->flightCode));
  fprintf(stdout,"Origin: %s (at %p)\n", fl->origin, (fl->origin));
  fprintf(stdout,"Dest: %s (at %p)\n", fl->destination, (fl->destination));
  fprintf(stdout,"dep date: %s (at %p)\n", fl->departureDate, (fl->departureDate));
  fprintf(stdout,"arr date: %s (at %p)\n", fl->arrivalDate, (fl->arrivalDate));
  fprintf(stdout,"plane model: %s (at %p)\n", fl->planeModel, (fl->planeModel));
} 

void printFlights(Flights * fls){
  int q,i;

  for(i = 0, q = fls->qFlights; i<q;i++ ){
    Flight * f = fls->flights + i;
    printFlight(f);
  }
}

int checkFlight(Flight *f1, Flight * f2){
  if(STR_EQUALS(f1->flightCode,f2->flightCode) == FALSE)
    return FALSE;

  if(STR_EQUALS(f1->origin,f2->origin) == FALSE)
    return FALSE;

  if(STR_EQUALS(f1->destination,f2->destination) == FALSE)
    return FALSE;

  if(STR_EQUALS(f1->departureDate,f2->departureDate) == FALSE)
    return FALSE;

  if(STR_EQUALS(f1->arrivalDate,f2->arrivalDate) == FALSE)
    return FALSE;

  if(STR_EQUALS(f1->planeModel,f2->planeModel) == FALSE)
    return FALSE;

  return TRUE;
}

int checkFlights(Flights *f1, Flights * f2){
  int i,q;

  if(f1->qFlights != f2->qFlights)
    return FALSE;

  for(i = 0, q = f1->qFlights; i<q;i++ ){
    Flight * fl1 = f1->flights + i;
    Flight * fl2 = f2->flights + i;

    if(checkFlight(fl1,fl2) == FALSE)
      return FALSE;
  }

  return TRUE;
}

void testSerialPlanes(){
  char * bytes_aux;
  int bytes_size;

  printf("\n\nTESTING PLANES SER/DES\n");

  Planes * fls = expandPlanes();
  Planes * fls_d = expandPlanes();
  Plane p1 = {.planeModel = "BOEING777", .rows = 29, .columns = 7};
  Plane p2 = {.planeModel = "BOEING666", .rows = 33, .columns = 5};
  Plane p3 = {.planeModel = "BOEING333", .rows = 37, .columns = 3};

  addPlane(fls, &p1);
  addPlane(fls, &p2);
  addPlane(fls, &p3);

  bytes_aux = serializePlanes(fls, &bytes_size);
  printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", bytes_size);
  printPlanes(fls);

  if(deserializePlanes(bytes_aux, bytes_size, fls_d) == DESERIALIZE_ERROR){
    printf("ERROR DESERIALIZE");
    return;
  }

  printf("DESERIALIZED: %d\n", bytes_size);
  printPlanes(fls_d);

  if(checkPlanes(fls,fls_d) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }

  freePlanes(fls);
  freePlanes(fls_d);
}

void printPlane(Plane * pl){
  fprintf(stdout,"struct in : %p\nPlane Model: %s (at %p)\n", pl, pl->planeModel, (pl->planeModel));
  fprintf(stdout,"rows: %d (at %p)\n", pl->rows, &(pl->rows));
  fprintf(stdout,"cols: %d (at %p)\n", pl->columns, &(pl->columns));
}

void printPlanes(Planes * pls){
  int q,i;

  for(i = 0, q = pls->qPlanes; i<q;i++ ){
    Plane * f = pls->planes + i;
    printPlane(f);
  }
}

int checkPlane(Plane * p1, Plane * p2){
  if(STR_EQUALS(p1->planeModel,p2->planeModel) == FALSE)
    return FALSE;

  if(p1->rows != p2->rows)
    return FALSE;

  if(p1->columns != p2->columns)
    return FALSE;
  return TRUE;
}

int checkPlanes(Planes * p1, Planes * p2){
  int i,q;

  if(p1->qPlanes != p2->qPlanes)
    return FALSE;

  for(i = 0, q = p1->qPlanes; i<q;i++ ){
    Plane * f = p1->planes + i;
    Plane * f2 = p2->planes + i;

  if(checkPlane(f,f2) == FALSE)
      return FALSE;
  }

  return TRUE;
}

void testSerialRes(){
  char * bytes_aux;
  int bytes_size;
  
  printf("\n\nTESTING RESERVATION SER/DES\n");
  
  Reservation r1 = {.flightCode = "AAA1", .seatRow = 7, .seatColumn = 5,.passportID = "39765402"};
  Reservation * fd = expandReservation(NULL);
  bytes_aux = serializeReservation(&r1, &bytes_size);
  printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", bytes_size);
  printReservation(&r1);

  if(deserializeReservation(bytes_aux, bytes_size, fd) == DESERIALIZE_ERROR){
    printf("ERROR DESERIALIZE");
    return;
  }

  printf("DESERIALIZED: %d\n", bytes_size);
  printReservation(fd);

  if(checkReservation(fd,&r1) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }

  freeExpandedReservation(fd, FALSE);

}

void printReservation(Reservation * res){
  fprintf(stdout,"struct in : %p\nFlight Code: %s (at %p)\n", res, res->flightCode, (res->flightCode));
  fprintf(stdout,"row: %d (at %p)\n", res->seatRow, &(res->seatRow));
  fprintf(stdout,"col: %d (at %p)\n", res->seatColumn, &(res->seatColumn));
  fprintf(stdout,"Passport id: %s (at %p)\n", res->passportID, (res->passportID));
} 

int checkReservation(Reservation * r1, Reservation * r2){
  if(STR_EQUALS(r1->flightCode,r2->flightCode) == FALSE)
    return FALSE;

  if(STR_EQUALS(r1->passportID,r2->passportID) == FALSE)
    return FALSE;

  if(r1->seatRow != r2->seatRow)
    return FALSE;

  if(r1->seatColumn != r2->seatColumn)
    return FALSE;
  return TRUE;
}

void testSerialFlightRes(){
  char * bytes_aux;
  int bytes_size;

  printf("\n\nTESTING FLIGHT RESERVATIONS SER/DES\n");

  flightReservations * fls = expandFlightReservations();
  flightReservations * fls_d = expandFlightReservations();
  Plane p1 = {.planeModel = "BOEING777", .rows = 12,.columns = 10};
  ReservationMinimal r1 = {.seatRow = 29, .seatColumn = 7};
  ReservationMinimal r2 = {.seatRow = 33, .seatColumn = 5};
  ReservationMinimal r3 = {.seatRow = 37, .seatColumn = 3};

  addFlightReservation(fls, &r1);
  addFlightReservation(fls, &r2);
  addFlightReservation(fls, &r3);
  setFlightReservationsSettings(fls, "AA33", &p1);

  bytes_aux = serializeFlightReservations(fls, &bytes_size);
  printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", bytes_size);
  printFlightReservations(fls);

  if(deserializeFlightReservations(bytes_aux, bytes_size, fls_d) == DESERIALIZE_ERROR){
    printf("ERROR DESERIALIZE");
    return;
  }

  printf("DESERIALIZED: %d\n", bytes_size);
  printFlightReservations(fls_d);

  if(checkFlightReservations(fls,fls_d) == TRUE){
    printf("TEST SUCCESS!\n");
  }else{
    printf("ERROR STRUCT DOES NOT MATCHES PLEASE CHECK CODE\n");
  }

  freeFlightReservations(fls);
  freeFlightReservations(fls_d);
}

void printFlightReservations(flightReservations * fres){
  fprintf(stdout,"struct in : %p\nFlight Code: %s (at %p)\n Plane struct:\n", fres, fres->flightCode, (fres->flightCode));
  printPlane(fres->planeSeats);
  printReservationMin(fres->reservations, fres->qReservations);
}

void printReservationMin(ReservationMinimal * resM, int size){
  int i;
  ReservationMinimal * res;
  for(i = 0; i < size; i++){
    res = resM+i;
     fprintf(stdout,"row: %d (at %p)\n", res->seatRow, &(res->seatRow));
      fprintf(stdout,"col: %d (at %p)\n", res->seatColumn, &(res->seatColumn));
  }
}

int checkFlightReservations(flightReservations * r1, flightReservations * r2){
  
  if(r1->qReservations != r2->qReservations)
    return FALSE;

  if(STR_EQUALS(r1->flightCode,r2->flightCode) == FALSE)
    return FALSE;

  if(checkPlane(r1->planeSeats,r2->planeSeats) == FALSE)
    return FALSE;

  for(int i =0; i < r1->qReservations; i++){
    if((r1->reservations)[i].seatRow != (r2->reservations)[i].seatRow)
      return FALSE;

    if((r1->reservations)[i].seatColumn != (r2->reservations)[i].seatColumn)
      return FALSE;
  }

  return TRUE;
}