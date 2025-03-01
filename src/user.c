#include "user.h"


struct User {
    char* id;
    char* name;
    char* email;
    char* phone;
    char* data;
    char sex;
    char* passport;
    char* country;
    char* address;
    char* account;
    char* pay;
    char* status;
};

void freeUser(User *user) {
    if (user == NULL) return;
    free(user->id);
    free(user->name);
    free(user->email);
    free(user->phone);
    free(user->data);
    free(user->passport);
    free(user->country);
    free(user->address);
    free(user->account);
    free(user->pay);
    free(user->status);
    free(user);
}

/*
A função 'criarUser()' tem como objetivo criar e inicializar uma nova instância da estrutura User.
A mesma verifica se o cabeçalho para usuários inválidos já foi impresso, caso contrário imprime-o.
Aloca memória para um novo usuário e inicializa os seus campos com valores fornecidos.
Verifica a validade dos dados (email, data, país, entre outros). 
Caso seja inválido, registra no arquivo de usuários inválidos e liberta a memória.
*/
User* criarUser(char **tokens, FILE *invalidUsersFile) {
   
    static int headerPrinted = 0;

    if (headerPrinted == 0) {
        fprintf(invalidUsersFile, "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status\n");
        headerPrinted = 1;
    }


    User *newUser = malloc(sizeof(User));
    if (newUser == NULL) {
        return NULL;
    }

    *newUser = (User){0};
    newUser->id = tokens[0] && strcmp(tokens[0], "") != 0 ? strdup(tokens[0]) : NULL;
    newUser->name = tokens[1] && strcmp(tokens[1], "") != 0 ? strdup(tokens[1]) : NULL;
    newUser->email = tokens[2] && strcmp(tokens[2], "") != 0 ? strdup(tokens[2]) : NULL;
    newUser->phone = tokens[3] && strcmp(tokens[3], "") != 0 ? strdup(tokens[3]) : NULL;
    newUser->data = tokens[4] && strcmp(tokens[4], "") != 0 ? strdup(tokens[4]) : NULL;
    newUser->sex = tokens[5] && strcmp(tokens[5], "") != 0 ? tokens[5][0] : '\0';
    newUser->passport = tokens[6] && strcmp(tokens[6], "") != 0 ? strdup(tokens[6]) : NULL;
    newUser->country = tokens[7] && strcmp(tokens[7], "") != 0 ? strdup(tokens[7]) : NULL;
    newUser->address = tokens[8] && strcmp(tokens[8], "") != 0 ? strdup(tokens[8]) : NULL;
    newUser->account = tokens[9] && strcmp(tokens[9], "") != 0 ? strdup(tokens[9]) : NULL;
    newUser->pay = tokens[10] && strcmp(tokens[10], "") != 0 ? strdup(tokens[10]) : NULL;
    newUser->status = tokens[11] && strcmp(tokens[11], "") != 0 ? strdup(tokens[11]) : NULL;

    // Validações
    if (newUser->id == NULL || newUser->name == NULL || newUser->email == NULL ||
        newUser->phone == NULL || !contemArroba(newUser->email) ||
        newUser->data == NULL || !dataValida(newUser->data) ||
        (newUser->sex != 'F' && newUser->sex != 'M' && newUser->sex != 'm' && newUser->sex != 'f') ||
        newUser->passport == NULL || newUser->country == NULL ||
        newUser->address == NULL || !countryValido(newUser->country) ||
        newUser->account == NULL || newUser->pay == NULL ||
        !dataCriarValida(newUser->account) || comparaAnos(newUser->data, newUser->account) ||
        newUser->status == NULL || !statusValido(newUser->status)) {

        // Registrar o erro no arquivo
        fprintf(invalidUsersFile, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
            tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5],
            tokens[6], tokens[7], tokens[8], tokens[9], tokens[10], tokens[11]);

        freeUser(newUser);
        return NULL;
    }

    return newUser;
}

//'getUserId'- Retorna o ID do usuário.
const char* getUserId(const User *user) {
    if (user == NULL) return NULL;
    return user->id;
}

//'getUserName'- Retorna o nome do usuário.
const char* getUserName(const User *user) {
    if (user == NULL) return NULL;
    return user->name;
}

//'getUserSex'- Retorna o gênero do usuário.
char getUserSex(const User *user) {
    if (user == NULL) {
        return '\0'; 
    }
    return user->sex; 
}

//'getUserData'- Retorna a data relevante do usuário.
const char* getUserData (const User *user) {
    if (user == NULL) return NULL;
    return user->data;
}

//'getUserCc'- Retorna o código do país do usuário.
const char* getUserCc(const User *user) {
    if(user == NULL) return NULL;
    return user->country;
}

//'getUserPassport'- Retorna o número do passaporte do usuário.
const char* getUserPassport (const User *user) {
    if (user == NULL) return NULL;
    return user->passport;
}

//'getUserStatus'- Retorna o status da conta do usuário.
const char* getUserStatus (const User *user) {
    if (user == NULL) return NULL;
    return user->status;
}

