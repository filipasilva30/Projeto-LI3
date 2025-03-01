#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

char* trim(char* str);
void toLowerCase(char *str);
char *toUpperCase(char *str);
int testaInteiro(const char *str, int *out_value);
char* concat(const char *s1, const char *s2);
int calcularNoite(const char* data1, const char* data2);
double calcularPrecoTotal(int x, int y, const char* data1, const char* data2);
int calcularIdade(const char *dataNascimento);
int timeToSeconds(int year, int month, int day, int hour, int minute, int second);
int extractComponents(const char* dateTime, int *year, int *month, int *day, int *hour, int *minute, int *second);
int delay(const char* hora_partida, const char* horareal_partida);
bool isNumber(const char *str);
int dataCriarValida(const char *dataCriar);

#endif /*UTILITIES_H*/