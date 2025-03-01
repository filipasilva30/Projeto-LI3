#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "query1.h"
#include "query2.h"
#include "query3.h"
#include "query4.h"
#include "query5.h"
#include "query6.h"
#include "query7.h"
#include "query8.h"
#include "query9.h"
#include "user_catalogs.h"
#include "flight_catalogs.h"
#include "reservations_catalog.h"
#include "passenger_catalogs.h"

int interpretador(char *caminho_input, UserHashTable *userHashTable, FlightHashTable *flightHashTable, ReservationHashTable *reservationHashTable, PassengerCatalog *passengerCatalog);
#endif