#ifndef _DBCORE_H_
#define _DBCORE_H_
/* 
fedora
yum install sqlite-devel
ubuntu
apt-get install libsqlite3-dev
*/
#include <sqlite3.h>

#define DB_FILENAME "data.db"

int checkDB();
int openDatabase(sqlite3 * * db);
int executeStaticSQL(sqlite3 * db, const char * sql);
void closeDatabase(sqlite3 * db);

#define F_FLIGHT_CODE_COLUMN 0
#define F_ORIGIN_COLUMN 1
#define F_DESTINATION_COLUMN 2
#define F_DEPARTURE_DATE_COLUMN 3
#define F_ARRIVALDATE_DATE_COLUMN 4
#define F_PLANE_MODEL_COLUMN5

#define P_MODEL_COLUMN 0
#define P_ROWS_COLUMN 1
#define P_COLUMNS_COLUMN 2

#define R_FLIGHT_CODE_COLUMN 0
#define R_SEAT_ROW_COLUMN 1
#define R_SEAT_COLUMN_COLUMN 2
#define R_PASSPORTID_COLUMN 3


#endif
 
