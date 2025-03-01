#ifndef QUERY4_H
#define QUERY4_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "user.h"
#include "reservations.h"
#include "reservations_catalog.h"
#include "user_catalogs.h"
#include "erros_user.h"
#include "erros_reservations.h"
#include "utilities.h"

int compareReservations(const void *a, const void *b);
void query4(bool formatFlag, char *args, int commandLineNum, ReservationHashTable *reservationHashTable, bool interativo);
#endif /* QUERY4_H */