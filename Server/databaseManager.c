#include "databaseManager.h"
#include "databaseCore.h"
#include "databaseConstants.h"
#include "expandManager.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEM_POST_N_CLOSE(sem) {sem_post(sem);\
								sem_close(sem);}
								
/* Warning all this functions are unsync use semaphores to sync them*/
Flights * getFlights_DB_wo_Semaphores(sqlite3 * db);
Plane * getPlaneFromFlight_DB_wo_Semaphores(const char * flightCode, sqlite3 * db);
simpleMessage * insertFlight_DB_wo_Semaphores(Flight * f, sqlite3 * db);
simpleMessage * deleteFlight_DB_wo_Semaphores(const char * flightCode, sqlite3 * db);
Planes * getPlanes_DB_wo_Semaphores(sqlite3 * db);
simpleMessage * insertPlane_DB_wo_Semaphores(Plane * p, sqlite3 * db);
simpleMessage * deletePlane_DB_wo_Semaphores(const char * planeModel, sqlite3 * db);
flightReservations * getFlightReservations_DB_wo_Semaphores(const char * flightCode, sqlite3 * db);
Reservations * getUserReservations_DB_wo_Semaphores(const char * passportID, sqlite3 * db);
int insertReservation_DB_wo_Semaphores(Reservation * r, sqlite3 * db);
int checkReservation_wo_Semaphores(Reservation * r, sqlite3 *db);
int deleteReservation_DB_wo_Semaphores(Reservation * r, sqlite3 * db);
int insertCancellation_DB_wo_Semaphores(Reservation * r, sqlite3 * db);
int checkCancellation_wo_Semaphores(Reservation * r, sqlite3 *db);

int isValidSeat(Plane * plane, Reservation * res);
void setPlaneData(Plane * p, sqlite3_stmt * stmt);
void setFlightData(Flight * f, sqlite3_stmt * stmt);
void setUserReservationData(Reservation * rm, sqlite3_stmt * stmt);
void setFlightReservationData(ReservationMinimal * rm, sqlite3_stmt * stmt);
void bindFlightInsertData(Flight * f, sqlite3_stmt * stmt);
void bindFlightDeleteData(const char * fCode, sqlite3_stmt * stmt);
void bindGetPlaneData(const char * pCode, sqlite3_stmt * stmt);
void bindPlaneInsertData(Plane * p, sqlite3_stmt * stmt);
void bindPlaneDeleteData(const char * planeModel, sqlite3_stmt * stmt);
void bindFlightDeleteData(const char * planeModel, sqlite3_stmt * stmt);
void bindReservationInsertData(const Reservation * r, sqlite3_stmt * stmt);
void bindReservationCheckData(const Reservation * r, sqlite3_stmt * stmt);
void bindReservationData(const Reservation * r, sqlite3_stmt * stmt);
void bindGetFlightReservation(const char * flightCode, sqlite3_stmt * stmt);
void bindGetUserReservation(const char * passportID, sqlite3_stmt * stmt);


Flights * getFlights_DB(sqlite3 * db){
	return getFlights_DB_wo_Semaphores(db);
}

Flights * getFlights_DB_wo_Semaphores(sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	Flights * flights;
   	Flight f;
	
   	flights = expandFlights();
	if(flights == NULL){
		return NULL;
	}

	rc = sqlite3_prepare_v2(db, DB_GET_FLIGHTS_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		freeFlights(flights);
		return NULL;
	}

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
	simpleMessage * smsg;
	sem_t * sem, * sem_2;

	sem = openMutexSemaphore(_SEMAPHORE_FLIGHTS_NAME_);
	sem_2 = openMutexSemaphore(_SEMAPHORE_PLANES_NAME_);
	sem_wait(sem);
	sem_wait(sem_2);

	smsg = insertFlight_DB_wo_Semaphores(f,db);

	SEM_POST_N_CLOSE(sem);
	SEM_POST_N_CLOSE(sem_2);
	return smsg;
}

simpleMessage * insertFlight_DB_wo_Semaphores(Flight * f, sqlite3 * db){
	int rc;
	
	sqlite3_stmt * stmt;
	simpleMessage * smsg;

	smsg = expandSimpleMessage();
	if(smsg == NULL)
		return NULL;

	rc = sqlite3_prepare_v2(db, DB_INSERT_FLIGHT_QUERY, -1, &stmt, 0);
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
	simpleMessage * smsg;
	sem_t * sem;

	sem = openMutexSemaphore(_SEMAPHORE_FLIGHTS_NAME_); // plane semaphore not necessary when deleting
	sem_wait(sem);

	smsg = deleteFlight_DB_wo_Semaphores(flightCode,db);

	SEM_POST_N_CLOSE(sem);
	return smsg;
}

simpleMessage * deleteFlight_DB_wo_Semaphores(const char * flightCode, sqlite3 * db){
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
	return getPlanes_DB_wo_Semaphores(db);
}

Planes * getPlanes_DB_wo_Semaphores(sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	Planes * planes;
   	Plane p;
	
	planes = expandPlanes();
	if(planes == NULL)
		return NULL;

	rc = sqlite3_prepare_v2(db, DB_GET_PLANES_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		
		freePlanes(planes);
		return NULL;
	}
  	
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

Reservations * getUserReservations_DB(const char * passportID, sqlite3 * db){
	return getUserReservations_DB_wo_Semaphores(passportID,db);
}

Reservations * getUserReservations_DB_wo_Semaphores(const char * passportID, sqlite3 * db){
	int rc;
	Reservations * reservations;
	Reservation r;
	sqlite3_stmt * stmt;

	reservations = expandReservations();
	if(reservations == NULL)
		return NULL;
	
	rc = sqlite3_prepare_v2(db, DB_GET_USER_RESERVATIONS_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		freeUserReservations(reservations);
		return NULL;
	}

	bindGetUserReservation(passportID, stmt);
	while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		setUserReservationData(&r,stmt);
		if(addUserReservation(reservations,&r) != EXPAND_OK){
			freeUserReservations(reservations);
			return NULL;
		}
	}	

	rc = sqlite3_finalize(stmt);

	if(rc != SQLITE_OK){
		freeUserReservations(reservations);
		return NULL;
	}

	return reservations;
}

void bindGetUserReservation(const char * passportID, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, 1, passportID, -1, SQLITE_STATIC); 
}

void setUserReservationData(Reservation * rm, sqlite3_stmt * stmt){
	rm->flightCode = (char *)sqlite3_column_text(stmt, R_FLIGHT_CODE_COLUMN);
	rm->seatRow = (int) sqlite3_column_int(stmt, R_SEAT_ROW_COLUMN);
	rm->seatColumn = (int) sqlite3_column_int(stmt, R_SEAT_COLUMN_COLUMN);
	rm->passportID = (char *)sqlite3_column_text(stmt, R_PASSPORTID_COLUMN);
}

Plane * getPlaneFromFlight_DB_wo_Semaphores(const char * flightCode, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
   	Plane * p;

   	p = expandPlane(NULL);
	if(p == NULL)
		return NULL;

	rc = sqlite3_prepare_v2(db, DB_GET_PLANE_FROM_FLIGHT_CODE_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		freeExpandedPlane(p, FALSE);
		return NULL;
	}

  	bindGetPlaneData(flightCode, stmt);
  	rc = sqlite3_step(stmt);
	if(rc != SQLITE_ROW){
		freeExpandedPlane(p, FALSE);
		return NULL;
	}	
	setPlaneData(p,stmt);
      
	rc = sqlite3_finalize(stmt);

	if(rc != SQLITE_OK){
		freeExpandedPlane(p, FALSE);
		return NULL;
	}

	return p;
}

void bindGetPlaneData(const char * flightCode, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, 1, flightCode, -1, SQLITE_STATIC); 
}

simpleMessage * insertPlane_DB(Plane * p, sqlite3 * db){
	simpleMessage * smsg;
	sem_t * sem;

	sem = openMutexSemaphore(_SEMAPHORE_PLANES_NAME_);
	sem_wait(sem);

	smsg = insertPlane_DB_wo_Semaphores(p,db);

	SEM_POST_N_CLOSE(sem);
	return smsg;
}

simpleMessage * insertPlane_DB_wo_Semaphores(Plane * p, sqlite3 * db){
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
	simpleMessage * smsg;
	sem_t * sem;
	sem = openMutexSemaphore(_SEMAPHORE_PLANES_NAME_);
	sem_wait(sem);

	smsg = deletePlane_DB_wo_Semaphores(planeModel, db);

	SEM_POST_N_CLOSE(sem);
	return smsg;
}

simpleMessage * deletePlane_DB_wo_Semaphores(const char * planeModel, sqlite3 * db){
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

simpleMessage * insertReservation_DB(Reservation * r, sqlite3 * db){
	sem_t * sem, *sem_1, *sem_2;
	simpleMessage * msg = expandSimpleMessage();
	if(msg == NULL)
		return NULL;

	sem = openMutexSemaphore(_SEMAPHORE_RESERVATION_NAME_);
	sem_1 = openMutexSemaphore(_SEMAPHORE_FLIGHTS_NAME_); 
	sem_2 = openMutexSemaphore(_SEMAPHORE_PLANES_NAME_); 
	sem_wait(sem_1);//block flight queries
	sem_wait(sem_2);//block plane queries (to verify seat rows and columns are OK)
	sem_wait(sem);

	setSimpleMessageSettings(msg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_RES_NOT_AVAILABLE);
	if(checkReservation_wo_Semaphores(r,db) == TRUE){
		if(insertReservation_DB_wo_Semaphores(r,db) == SQLITE_OK){
			setSimpleMessageSettings(msg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_RES_OK);
		}else{
			setSimpleMessageSettings(msg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_RES_ERROR);
		}
	}

	SEM_POST_N_CLOSE(sem_1);
	SEM_POST_N_CLOSE(sem_2);
	SEM_POST_N_CLOSE(sem);
	return msg;
}

int insertReservation_DB_wo_Semaphores(Reservation * r, sqlite3 * db){
	int rc;
	Plane * plane;
	sqlite3_stmt * stmt;

	plane = getPlaneFromFlight_DB_wo_Semaphores(r->flightCode, db);
	if(plane == NULL || isValidSeat(plane, r) == FALSE){
		freeExpandedPlane(plane, FALSE);
		return INVALID_SEAT_OR_PLANE;
	}
	freeExpandedPlane(plane, FALSE);

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

/* Returns TRUE IF Seat is Available, FALSE if seat is unavailable */
int checkReservation_wo_Semaphores(Reservation * r, sqlite3 *db){
	int rc,q;
	int check = FALSE;
	sqlite3_stmt * stmt;

	rc = sqlite3_prepare_v2(db, DB_CHECK_RESERVATION_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		return check;
	}
	
  	bindReservationData(r, stmt);
	rc = sqlite3_step(stmt); 
	if (rc == SQLITE_ROW) {
		q = sqlite3_column_int(stmt, 0);
	    check = (q == 0)? TRUE:FALSE;
	}
     
	rc = sqlite3_finalize(stmt);
	if(rc != SQLITE_OK){
		return check;
	}
	
	return check;
}

void bindReservationData(const Reservation * r, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, R_FLIGHT_CODE_COLUMN+1, r->flightCode, -1, SQLITE_STATIC); 
	sqlite3_bind_int(stmt, R_SEAT_ROW_COLUMN+1, r->seatRow); 
  	sqlite3_bind_int(stmt, R_SEAT_COLUMN_COLUMN+1, r->seatColumn); 
}

simpleMessage * insertCancellation_DB(Reservation * r, sqlite3 * db){
	sem_t * sem;
	simpleMessage * msg = expandSimpleMessage();
	if(msg == NULL)
		return NULL;

	sem = openMutexSemaphore(_SEMAPHORE_RESERVATION_NAME_);
	sem_wait(sem);

	setSimpleMessageSettings(msg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_CANCELATION_INCORRECT_PASSPORT);
	if(checkCancellation_wo_Semaphores(r,db) == TRUE){
		if(insertCancellation_DB_wo_Semaphores(r,db) == SQLITE_OK){
			setSimpleMessageSettings(msg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_CANCELATION_OK);
			deleteReservation_DB_wo_Semaphores(r,db);
		}else{
			setSimpleMessageSettings(msg, RESPONSE_CODE_CMD, SERVER_RESPONSE_INSERT_CANCELATION_ERROR);
		}
	}

	SEM_POST_N_CLOSE(sem);
	return msg;
}

/* Returns TRUE IF Seat is reserved by passport id, FALSE otherwise */
int checkCancellation_wo_Semaphores(Reservation * r, sqlite3 *db){
	int rc,q;
	int check = FALSE;
	sqlite3_stmt * stmt;

	rc = sqlite3_prepare_v2(db, DB_CHECK_CANCELATION_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		return check;
	}
	
  	bindReservationCheckData(r, stmt);
	rc = sqlite3_step(stmt); 
	if (rc == SQLITE_ROW) {
		q = sqlite3_column_int(stmt, 0);
	    check = (q > 0)? TRUE:FALSE;
	}
     
	rc = sqlite3_finalize(stmt);
	if(rc != SQLITE_OK){
		return check;
	}
	
	return check;
}

void bindReservationCheckData(const Reservation * r, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, R_FLIGHT_CODE_COLUMN+1, r->flightCode, -1, SQLITE_STATIC); 
	sqlite3_bind_int(stmt, R_SEAT_ROW_COLUMN+1, r->seatRow); 
  	sqlite3_bind_int(stmt, R_SEAT_COLUMN_COLUMN+1, r->seatColumn); 
  	sqlite3_bind_text(stmt, R_PASSPORTID_COLUMN+1, r->passportID, -1, SQLITE_STATIC); 
}

int isValidSeat(Plane * plane, Reservation * res){
	return ((res->seatRow < plane->rows ) && (res->seatColumn < plane->columns))? TRUE:FALSE;
}

int insertCancellation_DB_wo_Semaphores(Reservation * r, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	
	rc = sqlite3_prepare_v2(db, DB_INSERT_CANCELATION_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		return rc;
	}
	
  	bindReservationData(r, stmt);
	rc = sqlite3_step(stmt); 
	if (rc != SQLITE_DONE) {
	    return rc;
	}
	return sqlite3_finalize(stmt);
}

int deleteReservation_DB_wo_Semaphores(Reservation * r, sqlite3 * db){
	int rc;
	sqlite3_stmt * stmt;
	
	rc = sqlite3_prepare_v2(db, DB_DELETE_RESERVATION_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		return rc;
	}
	
  	bindReservationData(r, stmt);
	rc = sqlite3_step(stmt); 
	if (rc != SQLITE_DONE) {
	    return rc;
	}
	return sqlite3_finalize(stmt);
}

flightReservations * getFlightReservations_DB(const char * flightCode, sqlite3 * db){
	return getFlightReservations_DB_wo_Semaphores(flightCode,db);
}

flightReservations * getFlightReservations_DB_wo_Semaphores(const char * flightCode, sqlite3 * db){
	int rc;
	flightReservations * reservations;
	Plane * p;
	ReservationMinimal rm;
	sqlite3_stmt * stmt;

	reservations = expandFlightReservations();
	if(reservations == NULL)
		return NULL;

	if((p = getPlaneFromFlight_DB_wo_Semaphores(flightCode, db)) == NULL){
		freeFlightReservations(reservations);
		return NULL;
	}
	setFlightReservationsSettings(reservations, flightCode, p);
	freeExpandedPlane(p, FALSE);
	
	rc = sqlite3_prepare_v2(db, DB_GET_RESERVATIONS_QUERY, -1, &stmt, 0);
	if(rc != SQLITE_OK){
		freeFlightReservations(reservations);
		return NULL;
	}

	bindGetFlightReservation(flightCode, stmt);
	while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
		setFlightReservationData(&rm,stmt);
		if(addFlightReservation(reservations,&rm) != EXPAND_OK){
			freeFlightReservations(reservations);
			return NULL;
		}
	}	

	rc = sqlite3_finalize(stmt);

	if(rc != SQLITE_OK){
		freeFlightReservations(reservations);
		return NULL;
	}

	return reservations;
}

void bindGetFlightReservation(const char * flightCode, sqlite3_stmt * stmt){
	sqlite3_bind_text(stmt, 1, flightCode, -1, SQLITE_STATIC); 
}

void setFlightReservationData(ReservationMinimal * rm, sqlite3_stmt * stmt){
	rm->seatRow = (int) sqlite3_column_int(stmt, R_SEAT_ROW_COLUMN);
	rm->seatColumn = (int) sqlite3_column_int(stmt, R_SEAT_COLUMN_COLUMN);
}