
CREATE TABLE flights(
	flightCode TEXT PRIMARY KEY NOT NULL CHECK(LENGTH("flightCode") = 4),
	origin TEXT NOT NULL CHECK(LENGTH("origin") = 3),
	destination TEXT NOT NULL CHECK(LENGTH("destination") = 3),
	departureDate TEXT NOT NULL CHECK(LENGTH("departureDate") = 16),
	arrivalDate TEXT NOT NULL CHECK(LENGTH("arrivalDate") = 16),
	planeModel TEXT NOT NULL CHECK(LENGTH("planeModel") >= 3 AND LENGTH("planeModel") <= 10),
	FOREIGN KEY (planeModel) REFERENCES planes(model)
);
CREATE TABLE planes(
	model TEXT NOT NULL CHECK(LENGTH("model") <= 10),
	rows INT NOT NULL CHECK(rows > 0 AND rows < 99),
	columns INT NOT NULL CHECK(columns > 0 AND columns < 14)
);
CREATE TABLE reservation(
	flightCode TEXT PRIMARY KEY NOT NULL CHECK(LENGTH("flightCode") = 4),
	seat TEXT NOT NULL CHECK(LENGTH("seat") = 3),
	passportID TEXT NOT NULL CHECK(LENGTH("passportID") >= 7 AND LENGTH("passportID") <= 10),
	PRIMARY KEY (flightCode,seat),
	FOREIGN KEY (flightCode) REFERENCES flights(flightCode)
);
