#ifndef _DBCONS_H_
#define _DBCONS_H_

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

#define DB_CHECK_QUERY "SELECT * FROM flights, planes, reservations"

//use https://www.freeformatter.com/java-dotnet-escape.html#ad-output to escape the string

#define DB_CREATE_FLIGHTS_QUERY "CREATE TABLE flights(\
flightCode TEXT PRIMARY KEY NOT NULL CHECK(LENGTH(\"flightCode\") = 4),\
origin TEXT NOT NULL CHECK(LENGTH(\"origin\") = 3),destination TEXT NOT NULL CHECK(LENGTH(\"destination\") = 3),\
departureDate TEXT NOT NULL CHECK(LENGTH(\"departureDate\") = 16),\
arrivalDate TEXT NOT NULL CHECK(LENGTH(\"arrivalDate\") = 16),\
planeModel TEXT NOT NULL CHECK(LENGTH(\"planeModel\") >= 3 AND LENGTH(\"planeModel\") <= 10),\
FOREIGN KEY (planeModel) REFERENCES planes(model));" 

#define DB_CREATE_PLANES_QUERY "CREATE TABLE planes(\
model TEXT PRIMARY KEY NOT NULL CHECK(LENGTH(\"model\") <= 10),\
rows INT NOT NULL CHECK(rows > 0 AND rows < 99),\
columns INT NOT NULL CHECK(columns > 0 AND columns < 99));"

#define DB_CREATE_RESERVATIONS_QUERY "CREATE TABLE reservations(\
flightCode TEXT NOT NULL CHECK(LENGTH(\"flightCode\") = 4),\
seatRow INT NOT NULL CHECK(seatRow > 0 AND seatRow < 99),\
seatColumn INT NOT NULL CHECK(seatColumn > 0 AND seatColumn < 99),\
passportID TEXT NOT NULL CHECK(LENGTH(\"passportID\") >= 7 AND LENGTH(\"passportID\") <= 10),\
PRIMARY KEY (flightCode,seatRow,seatColumn),\
FOREIGN KEY (flightCode) REFERENCES flights(flightCode));"

#endif
 
