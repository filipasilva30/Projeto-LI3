#ifndef RESERVATIONSCATALOG_H
#define RESERVATIONSCATALOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "user.h"
#include "reservations.h"
#include "parser.h"
#include "user_catalogs.h"


typedef struct ReservationHashTable ReservationHashTable;
typedef struct ReservationHashNode ReservationHashNode;
typedef struct ReservationIterator ReservationIterator;

ReservationHashTable *initRHashTable();
void freeRHashTable(ReservationHashTable *table);
void resizeRHashTable(ReservationHashTable *table);
void insertReservation(ReservationHashTable *table, Reservation *reservation);


Reservation *getNextReservation(ReservationIterator *iterator);
ReservationIterator *initReservationIterator(ReservationHashTable *table);

ReservationHashTable* processarReservations(const char *caminho, UserHashTable *userHashTable);



int getReservationTableCount(const ReservationHashTable *table);
int getReservationTableSize(const ReservationHashTable *table);
ReservationHashNode *getReservationTableBucket(const ReservationHashTable *table, int index);
Reservation *getReservationFromNode(const ReservationHashNode *node);
ReservationHashNode *getNextReservationHashNode(const ReservationHashNode *currentNode);

Reservation *findReservationById(const ReservationHashTable *table, const char *reservationId);

#endif