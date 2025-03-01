#ifndef PASSENGERCATALOG_H
#define PASSENGERCATALOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "hash.h"
#include "user.h"
#include "flights.h"
#include "reservations.h"
#include "passengers.h"
#include "parser.h"
#include "user_catalogs.h"
#include "flight_catalogs.h"
#include "utilities.h"


typedef struct PassengerCatalog PassengerCatalog;
void addPassengerToCatalog(PassengerCatalog *catalog, Passenger *passenger);
void freePassengerCatalog(PassengerCatalog *catalog);
PassengerCatalog* processarPassengers(const char *caminho, UserHashTable* userHashTable, FlightHashTable* flightHashTable);
int getPassengerCatalogNum(const PassengerCatalog *catalog);
int indexPassenger(const PassengerCatalog *catalog, const char *frase);
Passenger* getPassenger(const PassengerCatalog *catalog, int i);

#endif /*PASSENGERCATALOGS_H*/