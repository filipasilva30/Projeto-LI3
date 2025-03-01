#ifndef ERROS_RESERVATIONS_H
#define ERROS_RESERVATIONS_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int isIntegerValueValid(int value, int minValidValue);
int verificaBreakfast(const char *breakfast);
int isRatingValid(int rate);

#endif /* ERROS_RESERVATION_H */