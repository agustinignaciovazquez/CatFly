#include "databaseCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int enableFK(sqlite3 * db);

int openDatabase(sqlite3 * * db){
	int rc;
	rc = sqlite3_open(DB_FILENAME,db);
	if(rc != SQLITE_OK){
		printf("Error: Opening DB in memory\n %s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
		return rc;
	}

	//Enable Foreign Keys
	rc = enableFK(*db);
	if(rc != SQLITE_OK){
		printf("Error: Enabling Foreign Key support\n %s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
	}

	printf("DB opened and FK enabled\n");
	return rc;
}

void closeDatabase(sqlite3 * db){
	sqlite3_close(db);
	printf("DB closed\n");
}

int enableFK(sqlite3 * db){
   int rc;
   sqlite3_stmt * stmt;
   char * sql = "PRAGMA foreign_keys = ON;";

   rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
   if(rc != SQLITE_OK)
      return rc;

   rc = sqlite3_step(stmt);
   if(rc != SQLITE_DONE)
      return rc;
      
   return sqlite3_finalize(stmt);
}