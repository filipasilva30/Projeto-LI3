#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "user.h"
#include "erros_reservations.h"
#include "user_catalogs.h"


typedef struct Reservation Reservation;

void freeReservation(Reservation *reservation);
Reservation* criarReservation(char **tokens, FILE *invalidReservationsFile, UserHashTable *userHashTable);

const char* getReservationId(const Reservation *reservation);
const char* getReservationUserId(const Reservation *reservation);
const char* getReservationHotelId(const Reservation *reservation);
int getReservationRate(const Reservation *reservation);
const char* getReservationDataIni(const Reservation *reservation);
const char* getReservationDataFim(const Reservation *reservation);
int getReservationPrice(const Reservation *reservation);
int getReservationTax(const Reservation *reservation);
const char* getReservationBreakfast(const Reservation *reservation);
const char* getReservationHotel_Name(const Reservation *reservation);
int getReservationStars(const Reservation *reservation);


#endif /* RESERVATIONS_H */