#ifndef QUERY7_H
#define QUERY7_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>   
#include "utilities.h"
#include "flight_catalogs.h"
#include "flights.h"
#include "erros_user.h"
#include "interativo.h"


typedef struct AirportDelays AirportDelays;

int compararInt(const void* a, const void* b);
int encontrarAeroporto(AirportDelays* aeroportos, int tamanho, const char* nome);
int calcularMediana(int* atrasos, int n_voos);
int comparaDelays(const void *a, const void *b);
void query7(bool formatFlag, char *args, int commandLineNum,FlightHashTable* flightHashTable, bool interativo);


#endif /* QUERY7_H */