#include "databaseCore.h"
#include "databaseConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int enableFK(sqlite3 * db);

int openDatabase(sqlite3 * * db){
	int rc;
	rc = sqlite3_open(DB_FILENAME,db);
	if(rc != SQLITE_OK){
		fprintf(stderr,"Error: Opening DB in memory %s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
		return rc;
	}

	//Enable Foreign Keys (Foreign key constraints are disabled by default (for backwards compatibility), so must be enabled separately for each database connection).
	rc = enableFK(*db);

	return rc;
}

/*Function returns SQLITE_OK IF DB EXISTS*/
int checkDB(sqlite3 * db){
	return executeStaticSQL(db, DB_CHECK_QUERY);
}

/*Function returns SQLITE_OK IF IT INSTALLS DB SUCCESSFULLY*/
int installDB(sqlite3 * db){
	char * queries[] = {DB_CREATE_FLIGHTS_QUERY, DB_CREATE_PLANES_QUERY, DB_CREATE_RESERVATIONS_QUERY, DB_CREATE_CANCELATIONS_QUERY, 0};
	return executeStaticSQLQueries(db, queries);
}

int checkAndInstallDB(){
	int rc;
	sqlite3 *db;

	rc = openDatabase(&db);
	if(rc != SQLITE_OK)
		return rc;

	//Check and install the DB if neccesary
  	if((rc = checkDB(db)) != SQLITE_OK){
  		fprintf(stdout,"Installing empty DB\n");
  		if((rc = installDB(db)) != SQLITE_OK)
  			 return rc;
  		fprintf(stdout,"DB Installed successfully!\n");
  	}
  	
  	closeDatabase(db);

  	return rc;
}

/*Executes and array of queries */
int executeStaticSQLQueries(sqlite3 * db, char * * sqlQueries){
	int rc,i;

	for(i=0; sqlQueries[i] != 0; i++){
		rc = executeStaticSQL(db,sqlQueries[i]);
		if(rc != SQLITE_OK){
			fprintf(stderr,"Error: %s in DB\n", sqlite3_errmsg(db));
			return rc;
		}
	}

	return rc;
}

void closeDatabase(sqlite3 * db){
	sqlite3_close(db);
}

int enableFK(sqlite3 * db){
	int rc;   
   rc = executeStaticSQL(db,DB_ENABLE_FK);

   if(rc != SQLITE_OK){
		fprintf(stderr,"Error: Enabling Foreign Key support %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	
	return rc;
}

int executeStaticSQL(sqlite3 * db, const char * sql){
	int rc;
   sqlite3_stmt * stmt;
   
   rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
   if(rc != SQLITE_OK)
      return rc;

   rc = sqlite3_step(stmt);
   if(rc != SQLITE_DONE)
      return rc;
      
   return sqlite3_finalize(stmt);
}