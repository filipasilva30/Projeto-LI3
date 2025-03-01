#ifndef QUERY8_H
#define QUERY8_H

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
#include "interativo.h"

int compareDates2(const char *data1, const char *data2);
int comparar_datas(const char *data1_str, const char *data2_str);
void query8(bool formatFlag, char* args, int commandLineNum, ReservationHashTable *reservationHashTable, bool interativo);

#endif
