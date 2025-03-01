#include "reservations_catalog.h"

    struct ReservationHashTable {
        ReservationHashNode **buckets;
        int size;
        int count;
    };

    struct ReservationHashNode {
        Reservation *reservation;
        struct ReservationHashNode *next;
    };

    struct ReservationIterator {
        ReservationHashTable *table;
        int currentBucket;
        ReservationHashNode *currentNode;
    };



/*
'initRHashTable()' inicializa uma hashtable para armazenar reservas, alocando a memória e definindo os seus atributos iniciais.
*/
ReservationHashTable *initRHashTable() {
    ReservationHashTable *table = malloc(sizeof(ReservationHashTable));
    table->size = INITIAL_HASH_TABLE_SIZE;
    table->count = 0;
    table->buckets = calloc(table->size, sizeof(ReservationHashNode*)); // Usa calloc para inicializar com NULL
    return table;
}
/*
'freeRHashTable()' libera a memória alocada para a respetiva tabela de reservas, incluindo para além das reservas os nós.
*/
void freeRHashTable(ReservationHashTable *table) {
    if (table == NULL) return;

    for (int i = 0; i < table->size; ++i) {
        ReservationHashNode *current = table->buckets[i];
        while (current != NULL) {
            ReservationHashNode *next = current->next;

            freeReservation(current->reservation);
            free(current);

            current = next;
        }
    }

    free(table->buckets);
    free(table);
}
/*
A função 'resizeRHashTable()' redimensiona a hashtable das reservas, reorganizando os elementos quando a mesma estiver muito cheia.
*/
void resizeRHashTable(ReservationHashTable *table) {
    int newSize = table->size * 2;
    ReservationHashNode **newBuckets = calloc(newSize, sizeof(ReservationHashNode*));

    for (int i = 0; i < table->size; i++) {
        ReservationHashNode *node = table->buckets[i];
        while (node != NULL) {
            // Calcula o novo índice
            unsigned long newIndex = hashFunction(getReservationId(node->reservation)) % newSize;

            // Insere no novo bucket
            ReservationHashNode *nextNode = node->next;
            node->next = newBuckets[newIndex];
            newBuckets[newIndex] = node;
            node = nextNode;
        }
    }

    free(table->buckets);
    table->buckets = newBuckets;
    table->size = newSize;
}

/*
'insertReservation()' tem como objetivo inserir uma reserva na respetiva hashtable.
*/
void insertReservation(ReservationHashTable *table, Reservation *reservation) {
    if (table == NULL || reservation == NULL || getReservationId(reservation) == NULL) return;

    // Aplica-se a função hash no id do usuário para obter o índice
    unsigned long index = hashFunction(getReservationId(reservation)) % table->size;

    ReservationHashNode *newNode = malloc(sizeof(ReservationHashNode));
    if (newNode == NULL) {
        return;
    }
    newNode->reservation = reservation;
    newNode->next = NULL;

    // Inserção na lista encadeada no 'bucket' correspondente
    if (table->buckets[index] == NULL) {
        table->buckets[index] = newNode;
    } else {
        newNode->next = table->buckets[index];
        table->buckets[index] = newNode;
    }

    table->count++;

    if ((double)table->count / table->size >= 0.7) {
        resizeRHashTable(table);
    }
}

/*
'initReservationIterator()' é uma função cujo objetivo é inicializar um iterador para percorrer as reservas na hashtable.
*/
ReservationIterator *initReservationIterator(ReservationHashTable *table) {
    ReservationIterator *iterator = malloc(sizeof(ReservationIterator));
    if (iterator != NULL) {
        iterator->table = table;
        iterator->currentBucket = 0;
        iterator->currentNode = NULL;
    }
    return iterator;
}
/*
A função 'getNextReservation()'obtém a próxima reserva da tabela hash de reservas usando o iterador.
*/
Reservation *getNextReservation(ReservationIterator *iterator) {
    while (iterator->currentBucket < iterator->table->size) {
        if (iterator->currentNode == NULL) {
            iterator->currentNode = iterator->table->buckets[iterator->currentBucket++];
        }

        while (iterator->currentNode != NULL) {
            Reservation *currentReservation = iterator->currentNode->reservation;
            iterator->currentNode = iterator->currentNode->next;

            if (currentReservation != NULL) {
                return currentReservation;
            }
        }
    }
    return NULL;
}

/*
A função 'processarReservations()' lê um arquivo CSV contendo informações de reservas, cria objetos de reserva, valida as informações do usuário associadas às reservas e insere as mesmas na respetiva hashtable.
*/
ReservationHashTable* processarReservations(const char *caminho, UserHashTable *userHashTable) {
    char *caminho_reservations = concat(caminho, "/reservations.csv");
    FILE *file_reservations = fopen(caminho_reservations, "r");
    free(caminho_reservations);
    if (file_reservations == NULL) {
        perror("Erro ao abrir o arquivo de reservas");
        return NULL;
    }

    FILE *invalidReservationsFile = fopen("Resultados/reservations_errors.csv", "w");
    if (invalidReservationsFile == NULL) {
        perror("Erro ao abrir o arquivo de erros de reservas");
        fclose(file_reservations);
        return NULL;
    }

    ReservationHashTable *hashTable = initRHashTable();
    char line[1024];
    int lineCount = 0;

    while (fgets(line, sizeof(line), file_reservations)) {
        if (lineCount == 0) { // Ignorar cabeçalho
            lineCount++;
            continue;
        }

        char **tokens = parseCSVLine(line);
        if (tokens != NULL) {
            Reservation *newReservation = criarReservation(tokens, invalidReservationsFile, userHashTable);
            if (newReservation != NULL) {
                insertReservation(hashTable, newReservation);
            }
            freeTokens(tokens);
        }
        lineCount++;
    }

    fclose(file_reservations);
    fclose(invalidReservationsFile);

    printf("Número de reservas na tabela: %d\n", hashTable->count);
    return hashTable;
}

/*
A função 'getReservationTableCount()' obtém o número de reservas na hashtable de reservas.
*/
int getReservationTableCount(const ReservationHashTable *table) {
    if (table == NULL) {
        return -1; 
    }
    return table->count;
}

/*
'getReservationTableSize()' indica o tamanho atual da tabela da respetiva tabela hash.
*/
int getReservationTableSize(const ReservationHashTable *table) {
    if (table == NULL) {
        return -1;
    }
    return table->size;
}

/*
'getReservationTableBucket()'é a função que obtém o bucket (lista encadeada) de reservas em um índice específico da respetiva tabela.
*/
ReservationHashNode *getReservationTableBucket(const ReservationHashTable *table, int indice) {
    if (table == NULL || indice < 0 || indice >= table->size) {
        return NULL;
    }
    return table->buckets[indice];
}

/*
'getReservationFromNode()' é a função que obtém a reserva associada a um nó da lista encadeada de reservas.
*/
Reservation *getReservationFromNode(const ReservationHashNode *node) {
    if (node == NULL) {
        return NULL;
    }
    return node->reservation;
}

/*
A função 'getNextReservationHashNode()' obtém o próximo nó da lista encadeada de reservas a partir do nó atual.
*/
ReservationHashNode *getNextReservationHashNode(const ReservationHashNode *currentNode) {
    if (currentNode == NULL) {
        return NULL;
    }
    return currentNode->next;
}


/*
A 'findReservationById()' encontra uma reserva na hashtable de reservas com base no seu respetivo ID.
*/
Reservation *findReservationById(const ReservationHashTable *table, const char *reservationId) {
    if (table == NULL || reservationId == NULL) {
        return NULL; 
    }
    unsigned long index = hashFunction(reservationId) % table->size;
    ReservationHashNode *node = table->buckets[index];

    // Procura na lista encadeada no 'bucket' correspondente
    while (node != NULL) {
        if (strcmp(getReservationId(node->reservation), reservationId) == 0) {
            return node->reservation; 
        }
        node = node->next;
    }

    return NULL;
}