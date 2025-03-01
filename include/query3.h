#ifndef QUERY3_H
#define QUERY3_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "reservations.h"
#include "reservations_catalog.h"
#include "erros_user.h"
#include "write_noLoop.h"


void query3(bool formatFlag, char *args, int commandLineNum,  ReservationHashTable* reservationTable, bool interativo);
#endif