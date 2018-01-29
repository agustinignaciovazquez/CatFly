#include "printManager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
int numPlaces (int n);

void printSimpleMenu(){
  fprintf(stdout,"Flight On-Line \n");
  fprintf(stdout,"Please select one option: \n");
  fprintf(stdout,"1) Show all flights\n");
  fprintf(stdout,"2) Reserve a seat\n");
  fprintf(stdout,"3) Show my flights\n");
}

void printAdminMenu(){
  fprintf(stdout,"Flight On-Line System Administration\n");
  fprintf(stdout,"Please select one option: \n");
  fprintf(stdout,"1) Show all flights\n");
  fprintf(stdout,"2) Add new flight\n");
  fprintf(stdout,"3) Reserve a seat\n");
  fprintf(stdout,"4) Show all planes models\n");
  fprintf(stdout,"5) Add new plane model\n");
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

void printPlane(Plane * pl, int i){
  fprintf(stdout,"Select ID: %d \n", (int)(i+1));
  fprintf(stdout,"Plane Model: %s\n", pl->planeModel);
  fprintf(stdout,"Rows: %d\n", pl->rows);
  fprintf(stdout,"Columns: %d\n\n", pl->columns);
}

void printPlanes(Planes * pls){
  int q,i;

  for(i = 0, q = pls->qPlanes; i<q;i++ ){
    Plane * f = pls->planes + i;
    printPlane(f,i);
  }
}

void printReservations(char * * reservations, int rows, int columns){
    int i,j,spaces,s;

    printf("ROWSxCOLUMNS \n\t");
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

int numPlaces (int n) {
    if (n < 0) return numPlaces (-n);
    if (n < 10) return 1;
    return 1 + numPlaces (n / 10);
}
