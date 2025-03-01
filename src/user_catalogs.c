#include "user_catalogs.h"

struct UserHashTable {
    UserHashNode **buckets;
    int size;
    int count;
};

struct UserHashNode {
    User *user;
    struct UserHashNode *next;
};

struct UserIterator {
    UserHashTable *table;
    int currentBucket;
    UserHashNode *currentNode;
};


/*
A função 'initUHashTable()' inicializa uma nova hashtable de usuários.
*/
UserHashTable *initUHashTable() {
    UserHashTable *table = malloc(sizeof(UserHashTable));
    table->size = INITIAL_HASH_TABLE_SIZE;
    table->count = 0;
    table->buckets = calloc(table->size, sizeof(UserHashNode*)); // Utilização de um 'calloc' para inicializar com NULL
    return table;
}
/*
A 'freeUHashTable()' libera a memória alocada para a respetiva tabela hash, incluindo todos os usuários e 'UserHashNode'.
*/
void freeUHashTable(UserHashTable *table) {
    if (table == NULL) return;

    for (int i = 0; i < table->size; ++i) {
        UserHashNode *current = table->buckets[i];
        while (current != NULL) {
            UserHashNode *next = current->next;

            freeUser(current->user);

            free(current);

            current = next;
        }
    }

    free(table->buckets);

    free(table);
}

/*
'resizeUHashTable()' é uma função que redimensiona a tabela hash de usuários, aumentando o número de 'buckets'.
*/
void resizeUHashTable(UserHashTable *table) {
    int newSize = table->size * 2;
    UserHashNode **newBuckets = calloc(newSize, sizeof(UserHashNode*));

    // Re-hashing todos os elementos
    for (int i = 0; i < table->size; i++) {
        UserHashNode *node = table->buckets[i];
        while (node != NULL) {
            unsigned long newIndex = hashFunction(getUserId(node->user)) % newSize;

            UserHashNode *nextNode = node->next;
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
'insertUser()' insere um novo usuário na respetiva tabela hash.
*/
void insertUser(UserHashTable *table, User *user) {
    if (table == NULL || user == NULL || getUserId(user) == NULL) return;

    // Aplicar a função de hash no id do usuário para obter o índice
    unsigned long index = hashFunction(getUserId(user)) % table->size;

    // Criar um novo nó para a lista encadeada
    UserHashNode *newNode = malloc(sizeof(UserHashNode));
    if (newNode == NULL) {
        return;
    }
    newNode->user = user;
    newNode->next = NULL;

    // Inserção na lista encadeada no 'bucket' correspondente
    if (table->buckets[index] == NULL) {
        table->buckets[index] = newNode;
    } else {
        newNode->next = table->buckets[index];
        table->buckets[index] = newNode;
    }

    // Incrementar a contagem de elementos na tabela
    table->count++;

    if ((double)table->count / table->size >= 0.7) {
        resizeUHashTable(table);
    }
}

/*
A 'initUserIterator()' inicializa um novo iterador para a tabela hash de usuários.
*/
UserIterator *initUserIterator(UserHashTable *table) {
    UserIterator *iterator = malloc(sizeof(UserIterator));
    if (iterator != NULL) {
        iterator->table = table;
        iterator->currentBucket = -1;
        iterator->currentNode = NULL;
    }
    return iterator;
}

/*
'getNextUser()' é uma função que obtém o próximo user da respetiva hashtable durante a iteração.
*/
User *getNextUser(UserIterator *iterator) {
    if (iterator->currentBucket >= iterator->table->size) {
        return NULL;
    }
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
        return iterator->currentNode->user;
    }

    return NULL;
}

/*
A função 'processarUsers()' processa um arquivo CSV que contém informações de usuários, cria objetos dos mesmos e insere-os na respetiva hashtable.
*/
UserHashTable* processarUsers(const char *caminho) {
    char *caminho_users = concat(caminho, "/users.csv");
    FILE *file_users = fopen(caminho_users, "r");
    free(caminho_users);
    if (file_users == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    FILE *invalidUsersFile = fopen("Resultados/users_errors.csv", "w");
    if (invalidUsersFile == NULL) {
        perror("Erro ao abrir o arquivo de erros");
        fclose(file_users);
        return NULL;
    }

    UserHashTable *hashTable = initUHashTable();
    char line[1024];
    int lineCount = 0;

    while (fgets(line, sizeof(line), file_users)) {
        if (lineCount == 0) { 
            lineCount++;
            continue;
        }

        // Usa-se parseCSVLine para dividir a linha
        char **tokens = parseCSVLine(line);
        if (tokens != NULL) {
            User *newUser = criarUser(tokens, invalidUsersFile);
            if (newUser != NULL) {
                insertUser(hashTable, newUser);
            }
            freeTokens(tokens);
        }
        lineCount++;
    }

    fclose(file_users);
    fclose(invalidUsersFile);

    printf("Número de usuários na tabela: %d\n", hashTable->count);
    return hashTable;
}

/*
A 'existeUser()' verifica se um usuário com o ID especificado existe na tabela hash de users.
*/
int existeUser(const UserHashTable *table, const char *user_id) {
    if (table == NULL || user_id == NULL) {
        printf("Tabela de usuários ou ID do usuário é NULL.\n");
        return -1;
    }

    //obtenção o índice
    unsigned long index = hashFunction(user_id) % table->size;

    // Procurar na lista encadeada no 'bucket' correspondente
    UserHashNode *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(getUserId(current->user), user_id) == 0) {
            return 1; 
        }
        current = current->next;
    }

    return -1; 
}

/*
'findUserById()' encontra e retorna um usuário com o ID especificado na tabela.
*/
User *findUserById(const UserHashTable *table, const char *user_id) {
    if (table == NULL || user_id == NULL) {
        return NULL; 
    }

    // Aplicar a função de hash no ID do user para obter o índice
    unsigned long index = hashFunction(user_id) % table->size;

    // Procurar na lista encadeada no 'bucket' correspondente
    UserHashNode *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(getUserId(current->user), user_id) == 0) {
            return current->user; 
        }
        current = current->next;
    }

    return NULL;
}

/*
A função 'getUserTableCount()' obtém o número de usuários na hashtable de users.
*/
int getUserTableCount(const UserHashTable *table) {
    if (table == NULL) return 0;
    return table->count;
}