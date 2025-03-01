#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "erros_flights.h"
#include "utilities.h"


typedef struct Flight Flight;

void freeFlights(Flight *flight);
Flight* criarFlight(char **tokens, FILE *invalidFlightsFile);
Flight **organizaFlight(char ***records, int numLines, FILE *invalidFlightsFile, int *numValidFlights);
const char* getFlightId(const Flight *flight);
const char* getFlightAirline(const Flight *flight);
const char* getFlightModel(const Flight *flight);
const char* getFlightOrigin(const Flight *flight);
const char* getFlightDest(const Flight *flight);
const char* getFlightPartida(const Flight *flight);
const char* getFlightRealPartida(const Flight *flight);
const char* getFlightChegada(const Flight *flight);
const char* getFlightPlane(const Flight *flight);
bool anoCoincide(const Flight *flight, int year);


//Flight* organizaFlight(FILE *file, int *n_flight);

#endif /* FLIGHTS_H */