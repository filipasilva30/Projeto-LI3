#ifndef ERROS_USERS_H
#define ERROS_USERS_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"


int dataValida(const char *data);
int contemArroba(const char *str);
int countryValido(const char *country);
int dataCriarValida(const char *dataCriar);
int comparaAnos(const char *dataNascimento, const char *dataCriaConta);
int statusValido(const char *estado);

#endif /* ERROS_USERS_H */


