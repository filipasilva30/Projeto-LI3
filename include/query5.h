#ifndef QUERY5_H
#define QUERY5_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "hash.h"
#include "flights.h"
#include "erros_flights.h"
#include "flight_catalogs.h"
#include "utilities.h"

int comparaDatas(const char *date1, const char *date2);
int comparaVoos(const void *a, const void *b);
void query5(bool formatFlag, char *args, int commandLineNum,FlightHashTable* flightHashTable, bool interativo);

#endif