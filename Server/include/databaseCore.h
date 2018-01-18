#ifndef _DBCORE_H_
#define _DBCORE_H_
/* 
fedora
yum install sqlite-devel
ubuntu
apt-get install libsqlite3-dev
*/
#include <sqlite3.h>
#include "databaseConstants.h"
#define DB_FILENAME "data.db"

int checkDB(sqlite3 * db);
int installDB(sqlite3 * db);
int openDatabase(sqlite3 * * db);
int openAndExecDB(sqlite3 * db, char * * sqlQueries);
int executeStaticSQL(sqlite3 * db, const char * sql);
void closeDatabase(sqlite3 * db);

#endif
 
