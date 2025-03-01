#include "query9.h"

/*
Esta função tem como objetivo comparar dois objetos 'user', onde 'a' é o ponteiro para o primeiro objeto 'User' e 'b' para o segundo objeto 'User'.
Primeiro compara os nomes dos usuários. Se os nomes forem diferentes, retorna o resultado da comparação.
Se os nomes forem iguais, compara os IDs dos usuários.
Retorna o valor indicativo da ordem dos usuários (menor que, igual a, ou maior que zero).
*/
int compareUsers(const void *a, const void *b) {
    User *user1 = *(User**)a;
    User *user2 = *(User**)b;

    int nameComparison = strcoll(getUserName(user1), getUserName(user2));

    if (nameComparison != 0) {
        return nameComparison;
    }

    // Se os nomes são iguais, compara os identificadores
    return strcoll(getUserId(user1), getUserId(user2));
}

/*
A função 'query9()' tem como objetivo processar uma consulta sobre usuários, filtrando-os por um prefixo de nome e ordenando-os.
A mesma começa por extrair o prefixo do nome do usuário dos 'args'.
Itera sobre os users na tabela hash, selecionando aqueles que estão ativos e cujos nomes começam com o prefixo especificado.
Ordena os usuários selecionados usando a função 'compareUsers'.
Escreve os detalhes dos usuários selecionados e ordenados em um arquivo ou terminal, formatando-os conforme especificado usando a 'formatFlag'
*/
void query9 (bool formatFlag, char*args, int commandLineNum, UserHashTable *userHashTable, bool interativo) {
    setlocale(LC_COLLATE, "en_US.UTF-8");

    FILE *outputFile;
    char outputFileName[1024];

    if (interativo) {
        outputFile = stdout; // Saída para o terminal
    } else {
        sprintf(outputFileName, "Resultados/command%d_output.txt", commandLineNum);
        outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }
    }

    if (args == NULL || userHashTable == NULL) {
        fclose(outputFile);
        return;
    }

    char *prefix = trim(args);

    UserIterator *iterador = initUserIterator(userHashTable);
    User *currentUser;
    User **selectedUsers = malloc(sizeof(User*) * getUserTableCount(userHashTable));
    int selectedCount = 0;

    while ((currentUser = getNextUser(iterador)) != NULL) {
        const char* userStatus = getUserStatus(currentUser);
        if (strcasecmp(userStatus, "inactive") != 0 && strncmp(getUserName(currentUser), prefix, strlen(prefix)) == 0) {
            selectedUsers[selectedCount++] = currentUser;
        }
    }
    free(iterador);

    // Ordenar os usuários selecionados
    qsort(selectedUsers, selectedCount, sizeof(User*), compareUsers);

    // Escrever os usuários selecionados no arquivo
    for (int i = 0; i < selectedCount; i++) {
        if (formatFlag) {
            fprintf(outputFile, "--- %d ---\n", i + 1);
            fprintf(outputFile, "id: %s\n", getUserId(selectedUsers[i]));
            fprintf(outputFile, "name: %s\n", getUserName(selectedUsers[i]));
            if (i<selectedCount - 1) fprintf(outputFile,"\n");
            // Aqui você pode adicionar mais linhas se houver mais campos a serem impressos
        } else {
            fprintf(outputFile, "%s;%s\n", getUserId(selectedUsers[i]), getUserName(selectedUsers[i]));
    }
    }

    free(selectedUsers);
    
    if (interativo) {
    } else {
        fclose(outputFile);
    }
}
