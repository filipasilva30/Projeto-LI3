#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

void freeTokens(char **tokens);
char **splitLine(const char *line);
char **parseCSVLine(const char *line);

#endif /* PARSER_H */