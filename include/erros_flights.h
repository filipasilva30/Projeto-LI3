#ifndef ERROS_FLIGHTS_H
#define ERROS_FLIGHTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

bool isValidAirportCode(const char *str);
int horaVoo(const char *dataVoo);
bool isDepartureBeforeArrival(const char *departure, const char *arrival);

#endif