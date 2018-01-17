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

int openDatabase(sqlite3 * * db);
void closeDatabase(sqlite3 * db);

#endif
 
