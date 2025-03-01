#ifndef QUERY6_H
#define QUERY6_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "flights.h"
#include "reservations.h"
#include "erros_reservations.h"
#include "flight_catalogs.h"
#include "passenger_catalogs.h"
#include "erros_flights.h"
#include "utilities.h"

typedef struct FlightIds  FlightIds;
typedef struct AirportPassengerCount AirportPassengerCount;

int compareAirport(const void *a, const void *b);

void addFlightIdToAirport(AirportPassengerCount **airportCounts, int *numAirports, int *capacity, const char *airportName, const char *flightId);
bool isFlightIdInAirport(const FlightIds *flightIds, const char *flightId);

void query6(bool formatFlag, char *args, int commandLineNum, FlightHashTable* flightHashTable, const PassengerCatalog *passengerCatalog, bool interativo);




#endif