#include "flight_catalogs.h"

struct FlightHashNode {
    Flight *flight;
    struct FlightHashNode *next;
};

struct FlightHashTable {
    FlightHashNode **buckets;
    int size;
    int count;
};

struct FlightIterator {
    FlightHashTable *table;
    int currentBucket;
    FlightHashNode *currentNode;
};

/*
A função'initFHashTable()' inicializa uma tabela hash de voos. 
Configura o tamanho inicial, a contagem de elementos e os buckets vazios.
*/
FlightHashTable *initFHashTable() {
    FlightHashTable *table = malloc(sizeof(FlightHashTable));
    table->size = INITIAL_HASH_TABLE_SIZE;
    table->count = 0;
    table->buckets = calloc(table->size, sizeof(FlightHashNode*)); // Usa o calloc para inicializar com NULL
    return table;
}

/*
A 'freeFHashTable()' libera (free) a memória associada à tabela hash de voos, incluindo todos os voos e buckets.
*/
void freeFHashTable(FlightHashTable *table) {
    if (table == NULL) return;

    for (int i = 0; i < table->size; ++i) {
        FlightHashNode *current = table->buckets[i];
        while (current != NULL) {
            FlightHashNode *next = current->next;

            freeFlights(current->flight);

            free(current);

            current = next;
        }
    }

    free(table->buckets);
    free(table);
}

/*
'resizeFHashTable()' é uma função que redimensiona a tabela hash quando a mesma fica muito cheia.
Redimensiona aumentando o tamanho da mesma e reorganizando os elementos para evitar colisões excessivas.
*/
void resizeFHashTable(FlightHashTable *table) {
    int newSize = table->size * 2;
    FlightHashNode **newBuckets = calloc(newSize, sizeof(FlightHashNode*));

    // Re-hashing todos os elementos
    for (int i = 0; i < table->size; i++) {
        FlightHashNode *node = table->buckets[i];
        while (node != NULL) {
            // Calcula o novo índice
            unsigned long newIndex = hashFunction(getFlightId(node->flight)) % newSize;

            // Insere no novo bucket
            FlightHashNode *nextNode = node->next;
            node->next = newBuckets[newIndex];
            newBuckets[newIndex] = node;
            node = nextNode;
        }
    }

    // Substitui os antigos buckets pelos novos
    free(table->buckets);
    table->buckets = newBuckets;
    table->size = newSize;
}

/*
'insertFlight()' insere um voo na tabela hash de voos.
*/
void insertFlight(FlightHashTable *table, Flight *flight) {
    if (table == NULL || flight == NULL || getFlightId(flight) == NULL) return;

    // Aplica a função de hash no id do usuário para obter o índice
    unsigned long index = hashFunction(getFlightId(flight)) % table->size;

    // Cria um novo nó para a lista encadeada
    FlightHashNode *newNode = malloc(sizeof(FlightHashNode));
    if (newNode == NULL) {
        return;
    }
    newNode->flight = flight;
    newNode->next = NULL;

    // Insere na lista encadeada, no 'bucket' correspondente
    if (table->buckets[index] == NULL) {
        table->buckets[index] = newNode;
    } else {
        newNode->next = table->buckets[index];
        table->buckets[index] = newNode;
    }

    // Incrementa a contagem de elementos na tabela
    table->count++;

    if ((double)table->count / table->size >= 0.7) {
        resizeFHashTable(table);
    }
}

/*
'initFlightIterator()' inicializa um iterador para percorrer os voos na tabela hash.
*/
FlightIterator *initFlightIterator(FlightHashTable *table) {
    FlightIterator *iterator = malloc(sizeof(FlightIterator));
    if (iterator != NULL) {
        iterator->table = table;
        iterator->currentBucket = -1;
        iterator->currentNode = NULL;
    }
    return iterator;
}

//A função 'getNextFlight()' retorna o próximo voo na tabela, ou NULL se todos os voos foram iterados.
Flight *getNextFlight(FlightIterator *iterator) {
    if (iterator->currentBucket >= iterator->table->size) {
        return NULL;
    }

    // Avançar para o próximo nó ou bucket (caso seja necessário)
    if (iterator->currentNode == NULL || iterator->currentNode->next == NULL) {
        do {
            iterator->currentBucket++;
            if (iterator->currentBucket >= iterator->table->size) {
                return NULL; 
            }
            iterator->currentNode = iterator->table->buckets[iterator->currentBucket];
        } while (iterator->currentNode == NULL);
    } else {
        iterator->currentNode = iterator->currentNode->next;
    }

    if (iterator->currentNode != NULL) {
        return iterator->currentNode->flight;
    }

    return NULL; // Se chegar aqui, não há mais voos
}


/*
A 'processarFlights()' processa um arquivo CSV que contém informações dos voos, cria objetos dos mesmos e insere-os na respetiva tabela hash.
*/
FlightHashTable* processarFlights(const char *caminho) {
    // Concatena o caminho com o nome do arquivo CSV de voos
    char *caminho_flights = concat(caminho, "/flights.csv");

    // Abre o arquivo CSV de voos para leitura
    FILE *file_flights = fopen(caminho_flights, "r");
    if (file_flights == NULL) {
        perror("Erro ao abrir o arquivo");
        free(caminho_flights);
        return NULL;
    }
    free(caminho_flights);

    // Abre um arquivo para registrar voos inválidos
    FILE *invalidFlightsFile = fopen("Resultados/flights_errors.csv", "w");
    if (invalidFlightsFile == NULL) {
        perror("Erro ao abrir o arquivo de erros");
        fclose(file_flights);
        return NULL;
    }

    // Inicializa uma tabela hash de voos
    FlightHashTable *hashTable = initFHashTable();

    char line[1024];
    int lineCount = 0;

    // Lê cada linha do arquivo CSV e cria objetos de voo
    while (fgets(line, sizeof(line), file_flights)) {
        if (lineCount == 0) { 
            lineCount++;
            continue;
        }

        char **tokens = parseCSVLine(line);
        if (tokens != NULL) {
            // Cria um objeto de voo a partir dos tokens e registra voos inválidos
            Flight *newFlight = criarFlight(tokens, invalidFlightsFile);
            if (newFlight != NULL) {
                insertFlight(hashTable, newFlight);
            }
            freeTokens(tokens);
        }
        lineCount++;
    }

    fclose(file_flights);
    fclose(invalidFlightsFile);

    printf("Número de voos na tabela: %d\n", hashTable->count);
    
    return hashTable;
}

/*
A função 'existeVoo' verifica se um voo com um ID específico existe na tabela hash dos voos.
*/
int existeVoo(const FlightHashTable *table, const char *voo_id) {
    if (table == NULL || voo_id == NULL) {
        printf("Tabela de voos ou ID do voo é NULL.\n");
        return -1;
    }

    // Aplicar a função de hash no ID do voo para obter o índice
    unsigned long index = hashFunction(voo_id) % table->size;

    // Procurar na lista encadeada no 'bucket' correspondente
    FlightHashNode *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(getFlightId(current->flight), voo_id) == 0) {
            return 1; // Encontrou o voo
        }
        current = current->next;
    }

    return -1; // Não encontrou o voo
}


/*
A 'findFlightById()' localiza e retorna um voo com base no seu ID na respetiva hashtable.
*/
Flight *findFlightById(const FlightHashTable *table, const char *flightId) {
    if (table == NULL || flightId == NULL) {
        return NULL; // Verificação de segurança
    }

    // Aplicar a função de hash no ID do voo para obter o índice
    unsigned long index = hashFunction(flightId) % table->size;
    FlightHashNode *node = table->buckets[index];

    // Procurar na lista encadeada no 'bucket' correspondente
    while (node != NULL) {
        if (strcmp(getFlightId(node->flight), flightId) == 0) {
            return node->flight;  // Voo encontrado
        }
        node = node->next;
    }

    return NULL; // Voo não encontrado
}

/*
'getFlightTableSize()'é uma função que retorna o tamanho atual da tabela.
*/
int getFlightTableSize(const FlightHashTable *table) {
    if (table == NULL) {
        return -1; 
    }
    return table->size;
}

/*
Esta funçáo retorna o número de voos atuais na tabela.
*/
int getFlightTableCount(const FlightHashTable *table) {
    if (table == NULL) {
        return -1; 
    }
    return table->count;
}

// Retorna a lista encadeada de voos (bucket) num índice específico da hashtable.
FlightHashNode *getFlightTableBucket(const FlightHashTable *table, int indice) {
    if (table == NULL || indice < 0 || indice >= table->size) {
        return NULL;
    }
    return table->buckets[indice];
}

//Obtém o voo associado a um nó da respetiva tabela.
Flight *getFlightFromNode(const FlightHashNode *node) {
    if (node == NULL) {
        return NULL;
    }
    return node->flight;
}

// Obtém o próximo nó na lista encadeada de voos (usado internamente).
FlightHashNode *getNextFlightHashNode(const FlightHashNode *currentNode) {
    if (currentNode == NULL) {
        return NULL;
    }
    return currentNode->next;
}
