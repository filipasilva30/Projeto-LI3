#ifndef USERCATALOG_H
#define USERCATALOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "hash.h"
#include "user.h"
#include "erros_user.h"
#include "parser.h"
#include "utilities.h"

typedef struct UserHashTable UserHashTable;
typedef struct UserHashNode UserHashNode;
typedef struct UserIterator UserIterator;

UserHashTable *initUHashTable();
void freeUHashTable(UserHashTable *table);
void resizeUHashTable(UserHashTable *table);
void insertUser(UserHashTable *table, User *user);

User *getNextUser(UserIterator *iterator);
UserIterator *initUserIterator(UserHashTable *table);

UserHashTable* processarUsers(const char *caminho);

int existeUser(const UserHashTable *table, const char *user_id);

User *findUserById(const UserHashTable *table, const char *user_id);
int getUserTableCount(const UserHashTable *table);

#endif