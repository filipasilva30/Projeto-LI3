#ifndef FLIGHTCATALOG_H
#define FLIGHTCATALOG_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "hash.h"
#include "flights.h"
#include "parser.h"
#include "utilities.h"


typedef struct FlightHashNode FlightHashNode;
typedef struct FlightHashTable FlightHashTable;
typedef struct FlightIterator FlightIterator;


FlightHashTable *initFHashTable();
void freeFHashTable(FlightHashTable *table);
void resizeFHashTable(FlightHashTable *table);
void insertFlight(FlightHashTable *table, Flight *flight);

FlightIterator* initFlightIterator(FlightHashTable *table);
Flight *getNextFlight(FlightIterator *iterator);




FlightHashTable* processarFlights(const char *caminho);





int existeVoo(const FlightHashTable *table, const char *voo_id);
Flight *findFlightById(const FlightHashTable *table, const char *flightId);

int getFlightTableSize(const FlightHashTable *table); 
int getFlightTableCount(const FlightHashTable *table);
FlightHashNode *getFlightTableBucket(const FlightHashTable *table, int indice);
Flight *getFlightFromNode(const FlightHashNode *node);
FlightHashNode *getNextFlightHashNode(const FlightHashNode *currentNode);



#endif /* FLIGHTCATALOGS_H */
