#include "printManager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
int numPlaces (int n);

void printSelectionMenu(){
  fprintf(stdout,"Flight On-Line Log-In\n");
  fprintf(stdout,"Options: \n");
  fprintf(stdout,"%d) Enter as Normal User \n", ENTER_AS_USER);
  fprintf(stdout,"%d) Enter as Administrator \n", ENTER_AS_ADMIN);
}

void printSimpleMenu(){
  fprintf(stdout,"Flight On-Line \n");
  fprintf(stdout,"Options: \n");
  fprintf(stdout,"%c) Show all flights\n", GET_FLIGHTS_CMD);
  fprintf(stdout,"%c) Reserve a seat\n", INSERT_FLIGHT_RESERVATION_CMD);
  fprintf(stdout,"%c) Show my reservations\n", GET_USER_RESERVATIONS_CMD);
  fprintf(stdout,"%c) Cancel a reservation\n", DELETE_FLIGHT_RESERVATION_CMD);
  fprintf(stdout,"%c) Exit\n", DISCONNECT_CMD);
}

void printAdminMenu(){
  fprintf(stdout,"Flight On-Line System Administration\n");
  fprintf(stdout,"Options: \n");
  fprintf(stdout,"%c) Show all flights\n", GET_FLIGHTS_CMD);
  fprintf(stdout,"%c) Add new flight\n", INSERT_FLIGHT_CMD);
  fprintf(stdout,"%c) Remove flight\n", DELETE_FLIGHT_CMD);
  fprintf(stdout,"%c) Reserve a seat\n", INSERT_FLIGHT_RESERVATION_CMD);
  fprintf(stdout,"%c) Show all planes models\n", GET_PLANES_CMD);
  fprintf(stdout,"%c) Add new plane model\n", INSERT_PLANE_CMD);
  fprintf(stdout,"%c) Remove plane model\n", DELETE_PLANE_CMD);
  fprintf(stdout,"%c) Exit\n", DISCONNECT_CMD);
}

void printFlightMenu(Flight * fl){
  fprintf(stdout,"Flight Details: \n");
  printFlight(fl);
  fprintf(stdout,"Options: \n");
  fprintf(stdout,"%c) Show reserved seats\n", GET_FLIGHT_RESERVATIONS_CMD);
  fprintf(stdout,"%c) Reserve a seat\n", INSERT_FLIGHT_RESERVATION_CMD);
  fprintf(stdout,"%c) Go back to menu\n", BACK_CMD);
}

void printFlightsMenu(Flights * fls){
  fprintf(stdout,"All Flights:\n");
  if(fls->qFlights == 0){
    fprintf(stdout,"There are no flights in Database\n ");
    return;
  }
  printFlights(fls);
}

void printFlight(Flight * fl){
  fprintf(stdout,"Flight Code: %s\n", fl->flightCode);
  fprintf(stdout,"Origin: %s\n", fl->origin);
  fprintf(stdout,"Destination: %s\n", fl->destination);
  fprintf(stdout,"Departure Date: %s \n", fl->departureDate);
  fprintf(stdout,"Arrival Date: %s \n", fl->arrivalDate);
  fprintf(stdout,"Plane Model: %s \n\n", fl->planeModel);
} 

void printFlightMin(Flight * fl, int i){
  if(i >= 0)
    fprintf(stdout,"Select ID: %d / ", (int)(i+1));

  fprintf(stdout,"Flight Code: %s / ", fl->flightCode);
  fprintf(stdout,"Origin: %s / ", fl->origin);
  fprintf(stdout,"Destination: %s / ", fl->destination);
  fprintf(stdout,"Departure Date: %s / ", fl->departureDate);
  fprintf(stdout,"Arrival Date: %s / ", fl->arrivalDate);
  fprintf(stdout,"Plane Model: %s / \n", fl->planeModel);
} 

void printFlights(Flights * fls){
  int q,i;

  for(i = 0, q = fls->qFlights; i<q;i++ ){
    Flight * f = fls->flights + i;
    printFlightMin(f, i);
  }
}

void printReservations(char * * reservations, Plane * p){
    int i,j,spaces,s;
    int rows, columns;
    rows = p->rows;
    columns = p->columns;
    //printf("FLIGHT SEAT TABLE \n\t"); 
    printf("%c: SEAT AVAILABLE \n\t",SEAT_AVAILABLE_DISPLAY);
    printf("%c: SEAT OCCUPIED \n\t", SEAT_OCCUPIED_DISPLAY);
    
    for(j = 0; j < columns; j++){
      printf("  %d", (j+1));
    }
    printf("\n\n");

    for(i = 0; i < rows; i++){
      printf("%d \t", (i+1));
      for(j = 0; j < columns; j++){
        spaces = numPlaces(j+1);
        for(s = 0; s < spaces;s++){
          putchar(' ');
        }
        printf("%c|", (reservations[i][j] != OCCUPIED_SEAT)? SEAT_AVAILABLE_DISPLAY : SEAT_OCCUPIED_DISPLAY);
      }
      printf("\n");
    }
}

void printPlane(Plane * pl, int i){
  fprintf(stdout,"Select ID: %d \n", (int)(i+1));
  fprintf(stdout,"Plane Model: %s\n", pl->planeModel);
  fprintf(stdout,"Rows: %d\n", pl->rows);
  fprintf(stdout,"Columns: %d\n\n", pl->columns);
}

void printPlaneMin(Plane * pl){
  fprintf(stdout,"Plane Model: %s / ", pl->planeModel);
  fprintf(stdout,"Rows: %d / ", pl->rows);
  fprintf(stdout,"Columns: %d /\n", pl->columns);
}

void printPlanesMenu(Planes * pls){
  fprintf(stdout,"All Planes:\n");
  if(pls->qPlanes == 0){
    fprintf(stdout,"There are no planes in Database\n ");
    return;
  }
  printPlanes(pls);
}

void printPlanes(Planes * pls){
  int q,i;
  Plane * p;
  for(i = 0, q = pls->qPlanes; i<q;i++ ){
    p = pls->planes + i;
    printPlane(p,i);
  }
}

void printReservationsMenu(Reservations * res){
  if(res->qReservations == 0){
    fprintf(stdout,"There are no reservations matching your passport ID\n");
    return;
  }
  fprintf(stdout,"Your reservations:\n");
  printUserReservations(res);
}

void printReservation(Reservation * r, int i){
  fprintf(stdout,"Select ID: %d /", (int)(i+1));
  fprintf(stdout,"Flight Code: %s / ", r->flightCode);
  fprintf(stdout,"Seat: %d x %d\n", r->seatRow+1, r->seatColumn+1);
}

void printUserReservations(Reservations * res){
  int q,i;
  Reservation * r;
  for(i = 0, q = res->qReservations; i<q;i++ ){
    r = res->reservations + i;
    printReservation(r,i);
  }
}

int numPlaces (int n) {
    if (n < 0) return numPlaces (-n);
    if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}
