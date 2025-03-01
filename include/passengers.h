#ifndef PASSENGERS_H
#define PASSENGERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "user_catalogs.h"
#include "flight_catalogs.h"


typedef struct Passenger Passenger;

void freePassenger(Passenger* passenger);
Passenger* criarPassenger(char **tokens, FILE *invalidPassengersFile, UserHashTable *userHashTable, FlightHashTable *flightHashTable);
Passenger** organizaPassenger(char ***records, int numLines, FILE *invalidPassengersFile, int *numValidPassengers, UserHashTable *userHashTable, FlightHashTable* flightHashTable);

const char* getPassengerFlight(const Passenger *passenger);
const char* getPassengerId(const Passenger *passenger);

#endif /* PASSENGERS_H */
