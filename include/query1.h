#ifndef QUERY1_H
#define QUERY1_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "flights.h"
#include "user.h"
#include "reservations.h"
#include "erros_user.h"
#include "erros_reservations.h"
#include "reservations_catalog.h"
#include "user_catalogs.h"
#include "passenger_catalogs.h"
#include "user_catalogs.h"
#include "erros_flights.h"
#include "utilities.h"
#include "interativo.h"
#include "write_noLoop.h"



int determinarID (const char* id);
void query1 (bool formatFlag, char* args, int commandLineNum,const UserHashTable* userHashTable,const FlightHashTable* flightHashTable, const PassengerCatalog* passengerCatalog, ReservationHashTable* reservationHashTable, bool interativo);

#endif