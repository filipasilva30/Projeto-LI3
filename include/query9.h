#ifndef QUERY9_H
#define QUERY9_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include "utilities.h"
#include "user.h"
#include "user_catalogs.h"

int compareUsers(const void *a, const void *b);
void query9 (bool formatFlag, char*args, int commandLineNum, UserHashTable *userHashTable, bool interativo);
#endif