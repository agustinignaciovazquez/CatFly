#include "databaseManager.h"
#include "databaseCore.h"
#include "databaseConstants.h"
#include "expandManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setFlightData(Flight * f, sqlite3_stmt * stmt);
void bindFlightInsertData(Flight * f, sqlite3_stmt * stmt);
/*
sqlite3_bind_text(stmt, 1, "Susan", -1, SQLITE_STATIC);                                        
	sqlite3_bind_int(stmt, 2, 21);  
	
	while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1)
*/
  /* if(rc != SQLITE_DONE) // if rc returns this there is no data
      return rc;*/
Flights * getFlights_DB(sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	Flights * flights;
   	Flight f;

	rc = sqlite3_prepare_v2(db, DB_GET_FLIGHTS_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK)
		return NULL;

	flights = expandFlights();
	if(flights == NULL)
		return NULL;
  	
	while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		setFlightData(&f,stmt);
		if(addFlight(flights,&f) != EXPAND_OK){
			freeFlights(flights);
			return NULL;
		}
	}	

      
	rc = sqlite3_finalize(stmt);
	if(rc != SQLITE_OK){
		freeFlights(flights);
		return NULL;
	}

	return flights;
}

void setFlightData(Flight * f, sqlite3_stmt * stmt){
	f->flightCode = (char *)sqlite3_column_text(stmt, F_FLIGHT_CODE_COLUMN);
	f->origin = (char *)sqlite3_column_text(stmt, F_ORIGIN_COLUMN);
	f->destination = (char *)sqlite3_column_text(stmt, F_DESTINATION_COLUMN);
	f->departureDate = (char *)sqlite3_column_text(stmt, F_DEPARTURE_DATE_COLUMN);
	f->arrivalDate = (char *)sqlite3_column_text(stmt, F_ARRIVALDATE_DATE_COLUMN);
	f->planeModel = (char *)sqlite3_column_text(stmt, F_PLANE_MODEL_COLUMN);
}

simpleMessage * insertFlight_DB(Flight * f, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	simpleMessage * smsg;

	smsg = expandSimpleMessage();
	if(smsg == NULL)
		return NULL;
	
	rc = sqlite3_prepare_v2(db, DB_GET_FLIGHTS_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		return smsg;
	}
	
	
  	bindFlightInsertData(f,stmt);
	rc = sqlite3_step(stmt); 
	if (rc != SQLITE_DONE) {
	    setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
	    return smsg;
	}

      
	rc = sqlite3_finalize(stmt);
	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		return smsg;
	}

	setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, SUCCESS_INSERT_FLIGHT_STR);
	return smsg;
}

void bindFlightInsertData(Flight * f, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, F_FLIGHT_CODE_COLUMN+1, f->flightCode, -1, SQLITE_STATIC);  
  	sqlite3_bind_text(stmt, F_ORIGIN_COLUMN+1, f->origin, -1, SQLITE_STATIC);  
  	sqlite3_bind_text(stmt, F_DESTINATION_COLUMN+1, f->destination, -1, SQLITE_STATIC);  
  	sqlite3_bind_text(stmt, F_DEPARTURE_DATE_COLUMN+1, f->departureDate, -1, SQLITE_STATIC);  
  	sqlite3_bind_text(stmt, F_ARRIVALDATE_DATE_COLUMN+1, f->arrivalDate, -1, SQLITE_STATIC);  
  	sqlite3_bind_text(stmt, F_PLANE_MODEL_COLUMN+1, f->planeModel, -1, SQLITE_STATIC);  
}