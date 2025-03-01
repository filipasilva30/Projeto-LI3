#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "erros_user.h"
#include "utilities.h"


typedef struct User User;

void freeUser(User *user);
User* criarUser(char **tokens, FILE *invalidUsersFile);

const char* getUserId(const User *user);
const char* getUserName(const User *user);
char getUserSex(const User *user);
const char* getUserData (const User *user);
const char* getUserCc(const User *user);
const char* getUserPassport (const User *user);
const char* getUserStatus (const User *user);

#endif /* USER_H */