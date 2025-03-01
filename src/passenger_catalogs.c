#include "passenger_catalogs.h"

struct PassengerCatalog{
    Passenger **passenger;
    int numPassengers;
};

/*
A função 'addPassengerToCatalog()' adiciona um passageiro ao catálogo de passageiros (PassengerCatalog).
Faz realocação de memória conforme necessário para acomodar o novo passageiro.
*/
void addPassengerToCatalog(PassengerCatalog *catalog, Passenger *passenger) {
    if (catalog == NULL || passenger == NULL) return;

    int newCount = catalog->numPassengers + 1;
    Passenger **newArray = realloc(catalog->passenger, newCount * sizeof(Passenger*));
    if (newArray == NULL) {
        return;
    }

    catalog->passenger = newArray;
    catalog->passenger[catalog->numPassengers] = passenger;
    catalog->numPassengers = newCount;
}


/*
'freePassengerCatalog()' é uma função que libera a memória associada ao catálogo de passageiros, incluindo todos os passageiros e o próprio catálogo.
*/
void freePassengerCatalog(PassengerCatalog *catalog) {
    if (catalog == NULL) {
        return;
    }

    for (int i = 0; i < catalog->numPassengers; ++i) {
        freePassenger(catalog->passenger[i]);
    }

    free(catalog->passenger);
    free(catalog);
}

/*
A função 'processarPassengers()' lê um arquivo CSV que contém informações de passageiros, cria objetos de passageiros e insere os mesmos no catálogo de passageiros. 
Também verifica a validade dos dados dos passageiros.
*/
PassengerCatalog* processarPassengers(const char *caminho, UserHashTable* userHashTable, FlightHashTable* flightHashTable) {
    // Concatena o caminho com o nome do arquivo CSV de passageiros
    char *caminho_passengers = concat(caminho, "/passengers.csv");

    // Abre o arquivo CSV de passageiros para leitura
    FILE *file_passengers = fopen(caminho_passengers, "r");
    if (file_passengers == NULL) {
        perror("Erro ao abrir o arquivo");
        free(caminho_passengers);
        return NULL;
    }
    free(caminho_passengers);

    // Abre um arquivo para registrar passageiros inválidos
    FILE *invalidPassengersFile = fopen("Resultados/passengers_errors.csv", "w");
    if (invalidPassengersFile == NULL) {
        perror("Erro ao abrir o arquivo de erros");
        fclose(file_passengers);
        return NULL;
    }

    // Inicializa um catálogo de passageiros
    PassengerCatalog *catalog = malloc(sizeof(PassengerCatalog));
    if (catalog == NULL) {
        fclose(invalidPassengersFile);
        fclose(file_passengers);
        return NULL;
    }
    catalog->passenger = NULL;
    catalog->numPassengers = 0;

    char line[1024];
    int lineCount = 0;

    // Lê cada linha do arquivo CSV e cria objetos de passageiros
    while (fgets(line, sizeof(line), file_passengers)) {
        if (lineCount == 0) {
            lineCount++;
            continue;
        }

        char **tokens = parseCSVLine(line);
        if (tokens != NULL) {
            // Cria um objeto de passageiro a partir dos tokens, valida informações de usuário e voo e registra passageiros inválidos
            Passenger *newPassenger = criarPassenger(tokens, invalidPassengersFile, userHashTable, flightHashTable);
            if (newPassenger != NULL) {
                // Adiciona o passageiro ao catálogo de passageiros
                addPassengerToCatalog(catalog, newPassenger);
            }
            freeTokens(tokens);
        }
        lineCount++;
    }

    fclose(file_passengers);
    fclose(invalidPassengersFile);

    printf("Número de passageiros na tabela: %d\n", catalog->numPassengers);

    return catalog;
}

/*
'getPassengerCatalogNum()' retorna o número de passageiros no catálogo de passageiros. 
*/
int getPassengerCatalogNum(const PassengerCatalog *catalog) {
    if (catalog == NULL) return 0;
    return catalog->numPassengers;
}

/*
A 'indexPassenger()' é uma função que procura um passageiro com um ID específico no catálogo de passageiros e retorna o índice do passageiro caso tenha sido encontrado, ou -1 caso contrário.
*/
int indexPassenger(const PassengerCatalog *catalog, const char *frase) {
    if (catalog == NULL || frase == NULL) {
        printf("Catálogo de voos ou ID do voo é NULL.\n");
        return -1;
    }

    for (int i = 0; i < catalog->numPassengers; i++) {
        const char* currentPassengerId = getPassengerId(catalog->passenger[i]); // Usando a função getter
        if (currentPassengerId != NULL && (strcmp(currentPassengerId,frase) == 0)) {
                return i;  // Encontrou um voo com o ID correspondente
        }
    }
    return -1;  // Não encontrou nenhum voo com o ID correspondente
}

/*
A 'getPassenger()' retorna o passageiro no índice especificado no catálogo de passageiros.
*/
Passenger* getPassenger(const PassengerCatalog *catalog, int i) {
    if (catalog == NULL || i < 0 || i >= catalog->numPassengers) {
        return NULL;
    }
    return catalog->passenger[i];
}