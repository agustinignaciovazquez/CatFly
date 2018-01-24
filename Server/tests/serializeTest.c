void testSerialSimpleCMD();
void printSimpleCMD(simpleCommand * cmd);
void testSerialFlight();
void testSerialFlights();
void printFlight(Flight * fl);
void printFlights(Flights * fls);
void testSerialPlanes();
void printPlane(Plane * pl);
void printPlanes(Planes * pls);
void testSerialRes();
void testSerialFlightRes();
void printFlightReservations(flightReservations * fres);
void printReservationMin(ReservationMinimal * resM, int size);

void testSerialSimpleCMD(){
	char * bytes_aux;
	int cmd_size;
	simpleCommand n_simpleCmd = {.command = 7, .extra = 7};
	simpleCommand simpleCmd = {.command = 65, .extra = 777};

	bytes_aux = serializeSimpleCommand(&simpleCmd, &cmd_size);
	printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", cmd_size);
	printSimpleCMD(&simpleCmd);
	if(deserializeSimpleCommand(bytes_aux, cmd_size, &n_simpleCmd) == DESERIALIZE_ERROR){
		printf("ERROR DESERIALIZE");
		return;
	}
	printf("DESERIALIZED: %d\n", cmd_size);
	printSimpleCMD(&n_simpleCmd);
}

void printSimpleCMD(simpleCommand * cmd){
	fprintf(stdout,"struct in : %p\nCommand: %c (at %p)\n", cmd, cmd->command, &(cmd->command));
	fprintf(stdout,"Extra int: %d (at %p)\n", cmd->extra, &(cmd->extra));
} 

void testSerialFlight(){
  char * bytes_aux;
  int bytes_size;

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
  freeExpandedFlight(fd, FALSE);
}

void testSerialFlights(){
  char * bytes_aux;
  int bytes_size;
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

void testSerialPlanes(){
  char * bytes_aux;
  int bytes_size;
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
void printReservation(Reservation * res);

void testSerialRes(){
  char * bytes_aux;
  int bytes_size;

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

  freeExpandedReservation(fd, FALSE);

}

void printReservation(Reservation * res){
  fprintf(stdout,"struct in : %p\nFlight Code: %s (at %p)\n", res, res->flightCode, (res->flightCode));
  fprintf(stdout,"row: %d (at %p)\n", res->seatRow, &(res->seatRow));
  fprintf(stdout,"col: %d (at %p)\n", res->seatColumn, &(res->seatColumn));
  fprintf(stdout,"Passport id: %s (at %p)\n", res->passportID, (res->passportID));
} 

void testSerialFlightRes(){
  char * bytes_aux;
  int bytes_size;
  flightReservations * fls = expandFlightReservations();
  flightReservations * fls_d = expandFlightReservations();
  Plane p1 = {.planeModel = "BOEING777", .rows = 12,.columns = 10};
  ReservationMinimal r1 = {.seatRow = 29, .seatColumn = 7};
  ReservationMinimal r2 = {.seatRow = 33, .seatColumn = 5};
  ReservationMinimal r3 = {.seatRow = 37, .seatColumn = 3};

  addReservation(fls, &r1);
  addReservation(fls, &r2);
  addReservation(fls, &r3);
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