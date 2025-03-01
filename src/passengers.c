#include "passengers.h"


struct Passenger {
    char* flight_id;
    char* user_id;
};

void freePassenger(Passenger* passenger) {
    if (passenger == NULL) return;
    free(passenger->flight_id);
    free(passenger->user_id);
    free(passenger);
}

/*
A função 'criarPassenger()' tem como objetivo criar e inicializar uma nova instância da estrutura Passenger.
A mesma verifica se o cabeçalho para passageiros inválidos já foi impresso, caso não tenha sido é impresso na hora.
Aloca memória para um novo passageiro e inicializa os seus campos com valores fornecidos.
Verifica a validade dos ID's do voo e do usuário usando as tabelas hash fornecidas.
Caso seja inválido, registra no arquivo de passageiros inválidos e liberta a memória.
*/
Passenger* criarPassenger(char **tokens, FILE *invalidPassengersFile, UserHashTable *userHashTable, FlightHashTable *flightHashTable) {
   
    static int headerPrinted = 0;  

    if (headerPrinted == 0) {
        fprintf(invalidPassengersFile, "flight_id;user_id\n");
        headerPrinted = 1;
    }
   
    Passenger *newPassenger = malloc(sizeof(Passenger));
    if (newPassenger == NULL) {
        return NULL; 
    }

    *newPassenger = (Passenger){0};
    newPassenger->flight_id = tokens[0] && strcmp(tokens[0], "") != 0 ? strdup(tokens[0]) : NULL;
    newPassenger->user_id = tokens[1] && strcmp(tokens[1], "") != 0 ? strdup(tokens[1]) : NULL;

    // Validações
    if (newPassenger->flight_id == NULL || (existeVoo(flightHashTable, newPassenger->flight_id)==-1) ||
        newPassenger->user_id == NULL || (existeUser(userHashTable, newPassenger->user_id)==-1)) {

        
        fprintf(invalidPassengersFile, "%s;%s\n", 
            tokens[0], tokens[1]);

        freePassenger(newPassenger);
        return NULL;
    }

    return newPassenger;
}

/*
A função 'organizaPassenger()' processa múltiplos registros de passageiros e cria uma lista de passageiros válidos.
Esta função aloca memória para armazenar ponteiros de Passenger.
Itera sobre os registros, criando passageiros válidos e adicionando-os à lista.
Caso seja necessário, a mesma redimensiona a lista para remover espaços não utilizados.
Atualiza o número de passageiros válidos e retorna a lista.
*/
Passenger** organizaPassenger(char ***records, int numLines, FILE *invalidPassengersFile, int *numValidPassengers, UserHashTable *userHashTable, FlightHashTable* flightHashTable) {
    fprintf(invalidPassengersFile, "flight_id;user_id\n");

    Passenger **passengers = malloc(numLines * sizeof(Passenger*));
    if (passengers == NULL) {
        *numValidPassengers = 0;
        return NULL; 
    }

    int validCount = 0;
    for (int i = 0; i < numLines; ++i) {
        Passenger *newPassenger = criarPassenger(records[i], invalidPassengersFile, userHashTable, flightHashTable);
        if (newPassenger != NULL) {
            passengers[validCount++] = newPassenger;
        }
    }

    if (validCount < numLines) {
        Passenger **temp = realloc(passengers, validCount * sizeof(Passenger*));
        if (temp == NULL) {
            // Liberar recursos alocados
            for (int i = 0; i < validCount; ++i) {
                freePassenger(passengers[i]);
            }
            free(passengers);
            *numValidPassengers = 0;
            return NULL;
        }
        passengers = temp;
    }

    *numValidPassengers = validCount;
    return passengers;
}

//'getPassengerFlight'- Retorna o ID do voo do passageiro.
const char* getPassengerFlight(const Passenger *passenger) {
    if (passenger == NULL) return NULL;
    return passenger->flight_id;  
}

//'getPassengerId'- Retorna o ID do usuário (passageiro).
const char* getPassengerId(const Passenger *passenger) {
    if (passenger == NULL) return NULL;
    return passenger->user_id;  
}

