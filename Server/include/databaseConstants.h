#ifndef _DBCONS_H_
#define _DBCONS_H_

#define F_FLIGHT_CODE_COLUMN 0
#define F_ORIGIN_COLUMN 1
#define F_DESTINATION_COLUMN 2
#define F_DEPARTURE_DATE_COLUMN 3
#define F_ARRIVALDATE_DATE_COLUMN 4
#define F_PLANE_MODEL_COLUMN 5

#define P_MODEL_COLUMN 0
#define P_ROWS_COLUMN 1
#define P_COLUMNS_COLUMN 2

#define R_FLIGHT_CODE_COLUMN 0
#define R_SEAT_ROW_COLUMN 1
#define R_SEAT_COLUMN_COLUMN 2
#define R_PASSPORTID_COLUMN 3

#define C_ID_COLUMN 0
#define C_FLIGHT_CODE_COLUMN 1
#define C_SEAT_ROW_COLUMN 2
#define C_SEAT_COLUMN_COLUMN 3
#define C_PASSPORTID_COLUMN 4

#define DB_CHECK_QUERY "SELECT * FROM flights, planes, reservations, cancelations;"
#define DB_ENABLE_FK "PRAGMA foreign_keys = ON;"
//use https://www.freeformatter.com/java-dotnet-escape.html#ad-output to escape the string

#define DB_CREATE_FLIGHTS_QUERY "CREATE TABLE IF NOT EXISTS flights(\
flightCode TEXT PRIMARY KEY NOT NULL CHECK(LENGTH(\"flightCode\") = 4),\
origin TEXT NOT NULL CHECK(LENGTH(\"origin\") = 3),\
destination TEXT NOT NULL CHECK(LENGTH(\"destination\") = 3),\
departureDate TEXT NOT NULL CHECK(LENGTH(\"departureDate\") = 16),\
arrivalDate TEXT NOT NULL CHECK(LENGTH(\"arrivalDate\") = 16),\
planeModel TEXT NOT NULL CHECK(LENGTH(\"planeModel\") >= 3 AND LENGTH(\"planeModel\") <= 10),\
FOREIGN KEY (planeModel) REFERENCES planes(model) ON DELETE CASCADE ON UPDATE CASCADE);" 

#define DB_CREATE_PLANES_QUERY "CREATE TABLE IF NOT EXISTS planes(\
model TEXT PRIMARY KEY NOT NULL CHECK(LENGTH(\"model\") <= 10),\
rows INT NOT NULL CHECK(rows > 0 AND rows < 99),\
columns INT NOT NULL CHECK(columns > 0 AND columns < 99));"

#define DB_CREATE_RESERVATIONS_QUERY "CREATE TABLE IF NOT EXISTS reservations(\
flightCode TEXT NOT NULL CHECK(LENGTH(\"flightCode\") = 4),\
seatRow INT NOT NULL CHECK(seatRow >= 0 AND seatRow < 99),\
seatColumn INT NOT NULL CHECK(seatColumn >= 0 AND seatColumn < 99),\
passportID TEXT NOT NULL CHECK(LENGTH(\"passportID\") >= 7 AND LENGTH(\"passportID\") <= 10),\
PRIMARY KEY (flightCode,seatRow,seatColumn),\
FOREIGN KEY (flightCode) REFERENCES flights(flightCode) ON DELETE CASCADE ON UPDATE CASCADE );"

#define DB_CREATE_CANCELATIONS_QUERY "CREATE TABLE IF NOT EXISTS cancelations(\
id INTEGER PRIMARY KEY AUTOINCREMENT,\
flightCode TEXT NOT NULL CHECK(LENGTH(\"flightCode\") = 4),\
seatRow INT NOT NULL CHECK(seatRow >= 0 AND seatRow < 99),\
seatColumn INT NOT NULL CHECK(seatColumn >= 0 AND seatColumn < 99),\
passportID TEXT NOT NULL CHECK(LENGTH(\"passportID\") >= 7 AND LENGTH(\"passportID\") <= 10),\
FOREIGN KEY (flightCode) REFERENCES flights(flightCode) ON DELETE CASCADE ON UPDATE CASCADE);"

#define DB_INSERT_FLIGHT_QUERY "INSERT INTO flights VALUES(?,?,?,?,?,?);"
#define DB_INSERT_PLANE_QUERY "INSERT INTO planes VALUES(?,?,?);"
#define DB_INSERT_RESERVATION_QUERY "INSERT INTO reservations VALUES(?,?,?,?);"
#define DB_INSERT_CANCELATION_QUERY "INSERT INTO cancelations SELECT NULL,* FROM reservations WHERE flightCode = ? AND seatRow = ? AND seatColumn = ?;"

#define DB_DELETE_FLIGHT_QUERY "DELETE FROM flights WHERE flightCode = ?;"
#define DB_DELETE_PLANE_QUERY "DELETE FROM planes WHERE model = ?;"
#define DB_DELETE_RESERVATION_QUERY "DELETE FROM reservation WHERE flightCode = ? AND seatRow = ? AND seatColumn = ?;"

#define DB_GET_FLIGHTS_QUERY "SELECT * FROM flights;"
#define DB_GET_FLIGHT_QUERY "SELECT * FROM flights WHERE flightCode = ?;"
#define DB_GET_PLANES_QUERY "SELECT * FROM planes;"
#define DB_GET_PLANE_FROM_FLIGHT_CODE_QUERY "SELECT planes.* FROM planes,flights WHERE flights.planeModel = planes.model AND flights.flightCode = ?;"
#define DB_GET_RESERVATIONS_QUERY "SELECT * FROM reservations WHERE flightCode = ?;" 
#define DB_GET_USER_RESERVATIONS_QUERY "SELECT * FROM reservations WHERE passportID = ?;" 
#define DB_GET_CANCELATIONS_QUERY "SELECT * FROM cancelations WHERE flightCode = ?;"

#define DB_CHECK_RESERVATION_QUERY "SELECT count(*) FROM reservations WHERE flightCode = ? AND seatRow = ? AND seatColumn = ?;"
#define DB_CHECK_CANCELATION_QUERY "SELECT count(*) FROM reservations WHERE flightCode = ? AND seatRow = ? AND seatColumn = ? AND passportID = ?;"
#endif
 
