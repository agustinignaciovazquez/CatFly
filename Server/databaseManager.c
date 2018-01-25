#include "databaseManager.h"
#include "databaseCore.h"
#include "databaseConstants.h"
#include "expandManager.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int insertReservation_DB(Reservation * r, sqlite3 * db);
int deleteReservation_DB(Reservation * r, sqlite3 * db);
void setFlightData(Flight * f, sqlite3_stmt * stmt);
void bindFlightInsertData(Flight * f, sqlite3_stmt * stmt);
void bindFlightDeleteData(const char * fCode, sqlite3_stmt * stmt);
void setPlaneData(Plane * p, sqlite3_stmt * stmt);
void bindPlaneInsertData(Plane * p, sqlite3_stmt * stmt);
void bindPlaneDeleteData(const char * planeModel, sqlite3_stmt * stmt);
void bindFlightDeleteData(const char * planeModel, sqlite3_stmt * stmt);
void bindReservationInsertData(const Reservation * r, sqlite3_stmt * stmt);
void bindReservationDeleteData(const Reservation * r, sqlite3_stmt * stmt);
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
	sem_t * sem;
	sqlite3_stmt * stmt;
	Flights * flights;
   	Flight f;
	
   	flights = expandFlights();
	if(flights == NULL){
		return NULL;
	}

 	sem = openMutexSemaphore(_SEMAPHORE_FLIGHTS_NAME_);
	sem_wait(sem);
	rc = sqlite3_prepare_v2(db, DB_GET_FLIGHTS_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		sem_post(sem);
		sem_close(sem);
		return NULL;
	}

	while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		setFlightData(&f,stmt);
		if(addFlight(flights,&f) != EXPAND_OK){
			freeFlights(flights);
			sem_post(sem);
			sem_close(sem);
			return NULL;
		}
	}	

	rc = sqlite3_finalize(stmt);

	sem_post(sem);
	sem_close(sem);

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
	sem_t * sem;
	sqlite3_stmt * stmt;
	simpleMessage * smsg;

	smsg = expandSimpleMessage();
	if(smsg == NULL)
		return NULL;

	sem = openMutexSemaphore(_SEMAPHORE_FLIGHTS_NAME_);
	sem_wait(sem);

	rc = sqlite3_prepare_v2(db, DB_INSERT_FLIGHT_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		sem_post(sem);
		sem_close(sem);
		return smsg;
	}
	
	
  	bindFlightInsertData(f,stmt);
	rc = sqlite3_step(stmt); 
	if (rc != SQLITE_DONE) {
	    setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
	    sem_post(sem);
		sem_close(sem);
	    return smsg;
	}

      
	rc = sqlite3_finalize(stmt);
	
	sem_post(sem);
	sem_close(sem);

	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		return smsg;
	}

	setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_FLIGHT_OK);
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

simpleMessage * deleteFlight_DB(const char * flightCode, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	simpleMessage * smsg;

	smsg = expandSimpleMessage();
	if(smsg == NULL)
		return NULL;
	
	rc = sqlite3_prepare_v2(db, DB_DELETE_FLIGHT_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		return smsg;
	}
	
	
  	bindFlightDeleteData(flightCode, stmt);
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

	setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, SERVER_RESPONSE_DELETE_FLIGHT_OK);
	return smsg;
}

void bindFlightDeleteData(const char * fCode, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, 1, fCode, -1, SQLITE_STATIC); 
}

Planes * getPlanes_DB(sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	Planes * planes;
   	Plane p;

	rc = sqlite3_prepare_v2(db, DB_GET_PLANES_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK)
		return NULL;

	planes = expandPlanes();
	if(planes == NULL)
		return NULL;
  	
	while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		setPlaneData(&p,stmt);
		if(addPlane(planes,&p) != EXPAND_OK){
			freePlanes(planes);
			return NULL;
		}
	}	

      
	rc = sqlite3_finalize(stmt);
	if(rc != SQLITE_OK){
		freePlanes(planes);
		return NULL;
	}

	return planes;
}

void setPlaneData(Plane * p, sqlite3_stmt * stmt){
	p->planeModel = (char *) sqlite3_column_text(stmt, P_MODEL_COLUMN);
	p->rows = (int) sqlite3_column_int(stmt, P_ROWS_COLUMN);
	p->columns = (int) sqlite3_column_int(stmt, P_COLUMNS_COLUMN);
}

simpleMessage * insertPlane_DB(Plane * p, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	simpleMessage * smsg;

	smsg = expandSimpleMessage();
	if(smsg == NULL)
		return NULL;
	
	rc = sqlite3_prepare_v2(db, DB_INSERT_PLANE_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		return smsg;
	}
	
	
  	bindPlaneInsertData(p,stmt);
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

	setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_PLANE_OK);
	return smsg;
}

void bindPlaneInsertData(Plane * p, sqlite3_stmt * stmt){  
  	sqlite3_bind_text(stmt, P_MODEL_COLUMN+1, p->planeModel, -1, SQLITE_STATIC);  
  	sqlite3_bind_int(stmt, P_ROWS_COLUMN+1, p->rows); 
  	sqlite3_bind_int(stmt, P_COLUMNS_COLUMN+1, p->columns); 
}

simpleMessage * deletePlane_DB(const char * planeModel, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	simpleMessage * smsg;

	smsg = expandSimpleMessage();
	if(smsg == NULL)
		return NULL;
	
	rc = sqlite3_prepare_v2(db, DB_DELETE_PLANE_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, sqlite3_errmsg(db));
		return smsg;
	}
	
	
  	bindPlaneDeleteData(planeModel, stmt);
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

	setSimpleMessageSettings(smsg, RESPONSE_CODE_CMD, SERVER_RESPONSE_DELETE_PLANE_OK);
	return smsg;
}

void bindPlaneDeleteData(const char * planeModel, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, 1, planeModel, -1, SQLITE_STATIC); 
}

int insertReservation_DB(Reservation * r, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	
	rc = sqlite3_prepare_v2(db, DB_INSERT_RESERVATION_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		return rc;
	}
	
	
  	bindReservationInsertData(r,stmt);
	rc = sqlite3_step(stmt); 
	if (rc != SQLITE_DONE) {
	    return rc;
	}

	return sqlite3_finalize(stmt);
}

void bindReservationInsertData(const Reservation * r, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, R_FLIGHT_CODE_COLUMN+1, r->flightCode, -1, SQLITE_STATIC); 
	sqlite3_bind_int(stmt, R_SEAT_ROW_COLUMN+1, r->seatRow); 
  	sqlite3_bind_int(stmt, R_SEAT_COLUMN_COLUMN+1, r->seatColumn); 
	sqlite3_bind_text(stmt, R_PASSPORTID_COLUMN+1, r->passportID, -1, SQLITE_STATIC); 
}

int deleteReservation_DB(Reservation * r, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	
	rc = sqlite3_prepare_v2(db, DB_DELETE_RESERVATION_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		return rc;
	}
	
  	bindReservationDeleteData(r, stmt);
	rc = sqlite3_step(stmt); 
	if (rc != SQLITE_DONE) {
	    return rc;
	}
	return sqlite3_finalize(stmt);
}

void bindReservationDeleteData(const Reservation * r, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, R_FLIGHT_CODE_COLUMN+1, r->flightCode, -1, SQLITE_STATIC); 
	sqlite3_bind_int(stmt, R_SEAT_ROW_COLUMN+1, r->seatRow); 
  	sqlite3_bind_int(stmt, R_SEAT_COLUMN_COLUMN+1, r->seatColumn); 
}