#ifndef QUERY2_H
#define QUERY2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "reservations_catalog.h"
#include "flight_catalogs.h"
#include "passenger_catalogs.h"
#include "user.h"
#include "utilities.h"
#include "interativo.h"


typedef struct CombinedInfo  CombinedInfo;
int compareCombinedInfo(const void *a, const void *b);
int compareDates(const char *date1, const char *date2);
void query2 (bool formatFlag, char *args, int commandLineNum, const UserHashTable *userHashTable,const FlightHashTable *flightHashTable,ReservationHashTable *reservationHashTable, const PassengerCatalog *passengerCatalog, bool interativo);
#endif