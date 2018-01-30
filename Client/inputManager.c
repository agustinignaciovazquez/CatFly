#include "inputManager.h"
#include "printManager.h"
#include "expandManager.h"
#include "constants.h"
#include "utilsCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#define LINE_MAX 77

#define CLEAN_BUFFER(ch) while ((ch = getchar()) != '\n');

int checkSeatIsInBounds(Plane * p, Reservation * rm);
int checkSeatIsAvailable(char * * res, Reservation * rm);

char getOption(){
  int i,c, aux;
  c = getchar();
  for(i = 1;(aux = getchar()) != '\n'; i++){
    if(aux == EOF)
      return EOF;
  }

  return (i == 1)? c : UNEXPECTED_ERROR;
}

int getInt(){

    char *p, s[LINE_MAX];
    int n;

    if(fgets(s, sizeof(s), stdin) != NULL) {
        n = strtol(s, &p, 10);
        if ((p != s && *p == '\n'))
            return n;
    }
    return UNEXPECTED_ERROR;
}

int getString(char * str, int max){
  if (fgets(str, max, stdin) != NULL) {
        return str;
   }

  return UNEXPECTED_ERROR;
}

ReservationMinimal getSeat(){
  ReservationMinimal rm;
  printf("Enter seat rows: ");
  rm.seatRow = getInt();
  printf("\nEnter seat columns: ");
  rm.seatColumn = getInt();
  return rm;
}

Reservation * getSeatForReservation(const Flight * f){
  Reservation * rm;
  rm = expandReservation(NULL);
  if(rm == NULL)
    return NULL;

  if(f != NULL){
    copyStr(rm->flightCode, f->flightCode, MAX_FLIGHTCODE);
  }else{
    printf("Enter Flight Code: ");
    getString(rm->flightCode, MAX_FLIGHTCODE);
  }
  
  printf("Enter PassportID: ");
  getString(rm->passportID, MAX_PASSPORTID);
  printf("Enter seat row: ");
  rm->seatRow = getInt() - 1;
  printf("Enter seat column: ");
  rm->seatColumn = getInt() - 1;

  return rm;
}

int checkSeatIsAvailable(char * * res, Reservation * rm){
  return (res[rm->seatRow][rm->seatColumn] == OCCUPIED_SEAT) ? FALSE : TRUE;
}

int checkSeatIsInBounds(Plane * p, Reservation * rm){
  return (rm->seatRow < p->rows && rm->seatColumn < p->columns ) ? TRUE : FALSE;
}

int checkReservationInput(Plane * p, char * * res, Reservation * rm){
  if(checkSeatIsInBounds(p, rm) == FALSE){
    printf("Error: Seat is out of bounds\n");
    return FALSE;
  }else{
    if(checkSeatIsAvailable(res, rm) == FALSE){
      printf("Error: Seat is not available\n");
      return FALSE;
    }
  }
  return TRUE;
}