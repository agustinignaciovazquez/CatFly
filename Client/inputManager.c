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

  printf(":>");

  c = getchar();
  for(i = 1;(aux = getchar()) != '\n'; i++){
    if(aux == EOF)
      return EOF;
  }

  return (i == 1)? c : INPUT_ERROR;
}

int getInt(const char * msg){

    char *p, s[LINE_MAX];
    int n;
    if(msg != NULL)
          printf("%s: ", msg);
    while(fgets(s, sizeof(s), stdin) != NULL) {
        n = strtol(s, &p, 10);
        if ((p != s && *p == '\n'))
            return n;
        //Error while parsing string  
        printf("Error: Not a number\n");
        if(msg != NULL)
          printf("%s: ", msg);
    }
    return INPUT_ERROR;
}

int getString(const char * msg, char * str, int max){
    char c, * start = str;

    if(msg != NULL)
          printf("%s: ", msg);

    while( (start - str) < (max - 1) && ( *start++ = getchar() ) != '\n' && *( start - 1 ) != EOF );
    *start = '\0';

    //Input is larger than expected, check must return false
    if(*( start - 1 ) != '\n' && *( start - 1 ) != EOF){
      CLEAN_BUFFER(c);
      *str = '\0'; //trick to return false in check
      return INPUT_OUTBOUNDS;
    }

    
    return (start - str);
}

Plane * getPlaneFromInput(){
  Plane * p;
  p = expandPlane(NULL);
  if(p == NULL)
    return NULL;
  getString("Enter Plane Model",p->planeModel, MAX_PLANE_MODEL);
  p->rows = getInt("Enter seat rows");
  p->columns = getInt("Enter seat columns");
  return p;
}

Flight * getFlightFromInput(){
  Flight * f;
  f = expandFlight(NULL);
  if(f == NULL)
    return NULL;
  
  getString("Enter Flight Code",f->flightCode, MAX_FLIGHTCODE);
  getString("Enter Origin",f->origin, MAX_ORIGIN);
  getString("Enter Destination",f->destination, MAX_DESTINATION);
  getString("Enter Departure Date",f->departureDate, MAX_DEP_DATE);
  getString("Enter Arrival Date",f->arrivalDate, MAX_ARR_DATE);
  getString("Enter Plane Model",f->planeModel, MAX_PLANE_MODEL);

  return f;
}

Reservation * getReservationFromInput(const Flight * f){
  Reservation * rm;
  rm = expandReservation(NULL);
  if(rm == NULL)
    return NULL;

  if(f != NULL){
    copyStr(rm->flightCode, f->flightCode, MAX_FLIGHTCODE);
  }else{
    getString("Enter Flight Code",rm->flightCode, MAX_FLIGHTCODE);
  }

  getString("Enter PassportID",rm->passportID, MAX_PASSPORTID);
  rm->seatRow = getInt("Enter seat row") - 1;
  rm->seatColumn = getInt("Enter seat column") - 1;

  return rm;
}

int checkSeatIsAvailable(char * * res, Reservation * rm){
  return (res[rm->seatRow][rm->seatColumn] == OCCUPIED_SEAT) ? FALSE : TRUE;
}

int checkSeatIsInBounds(Plane * p, Reservation * rm){
  return (rm->seatRow < p->rows && rm->seatColumn < p->columns ) ? TRUE : FALSE;
}

int checkPlaneInput(Plane * p){
  if(strlen(p->planeModel) != PLANE_MODEL_LEN){
    printf("Error: Plane model length must be %d characters\n", PLANE_MODEL_LEN);
    return FALSE;
  }
  return TRUE;
}

int checkFlightInput(Flight * f){
  if(strlen(f->flightCode) != FLIGHTCODE_LEN){
    printf("Error: Flight code length must be %d characters\n", FLIGHTCODE_LEN);
    return FALSE;
  }

  if(strlen(f->origin) != ORIGIN_LEN){
    printf("Error: Flight code length must be %d characters\n", ORIGIN_LEN);
    return FALSE;
  }

  if(strlen(f->destination) != DESTINATION_LEN){
    printf("Error: Flight code length must be %d characters\n", DESTINATION_LEN);
    return FALSE;
  }

  if(strlen(f->departureDate) != DEP_DATE_LEN){
    printf("Error: Flight code length must be %d characters\n", DEP_DATE_LEN);
    return FALSE;
  }

  if(strlen(f->arrivalDate) != ARR_DATE_LEN){
    printf("Error: Flight code length must be %d characters\n", ARR_DATE_LEN);
    return FALSE;
  }

  if(strlen(f->planeModel) != PLANE_MODEL_LEN){
    printf("Error: Plane model length must be %d characters\n", PLANE_MODEL_LEN);
    return FALSE;
  }
  return TRUE;
}

int checkReservationInput(Reservation * rm){
  if(strlen(rm->flightCode) != FLIGHTCODE_LEN){
    printf("Error: Flight code length must be %d characters\n", FLIGHTCODE_LEN);
    return FALSE;
  }
  if(!(strlen(rm->passportID) > MIN_PASSPORTID_LEN) && strlen(rm->passportID) < MAX_PASSPORTID_LEN){
    printf("Error: Passport ID length must be between %d and %d characters\n", MIN_PASSPORTID_LEN, MAX_PASSPORTID_LEN);
    return FALSE;
  }
    /* Leave this checks do it Server Side
  if(checkSeatIsInBounds(p, rm) == FALSE){
    printf("Error: Seat is out of bounds\n");
    return FALSE;
  }else{
    if(checkSeatIsAvailable(res, rm) == FALSE){
      printf("Error: Seat is not available\n");
      return FALSE;
    }
  }*/ 
  return TRUE;
}