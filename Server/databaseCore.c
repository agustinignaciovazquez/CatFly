#include "databaseCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int enableFK(sqlite3 * db);
int openAndExecDB(char * * sql);

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
int checkDB(){
	char * queries[] = {DB_CHECK_QUERY, 0};
	return openAndExecDB(queries);
}

/*Function returns SQLITE_OK IF IT INSTALLS DB SUCCESSFULLY*/
int installDB(){
	char * queries[] = {DB_CREATE_FLIGHTS_QUERY, DB_CREATE_PLANES_QUERY, DB_CREATE_RESERVATIONS_QUERY, 0};
	return openAndExecDB(queries);
}

/*Executes and array of queries */
int openAndExecDB(char * * sqlQueries){
	int rc,i;
	sqlite3 * db;

	rc = openDatabase(&db);
	if(rc != SQLITE_OK)
		return rc;

	for(i=0; sqlQueries[i] != 0; i++){
		rc = executeStaticSQL(db,sqlQueries[i]);
		if(rc != SQLITE_OK)
			fprintf(stderr,"Error: %s in DB\n", sqlite3_errmsg(db));
	}

	closeDatabase(db);
	return rc;
}

void closeDatabase(sqlite3 * db){
	sqlite3_close(db);
	//printf("DB closed\n");
}

int enableFK(sqlite3 * db){
	int rc;
   char * sql = "PRAGMA foreign_keys = ON;";
   
   rc = executeStaticSQL(db,sql);
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