#include "parser.h"

/*
A função 'freeTokens()' é usada para libertar a memória alocada para um array de strings (tokens). 
Ela verifica se o array de tokens é nulo e, em seguida, percorre cada string no array, libertando cada uma delas individualmente. 
Finalmente, liberta o próprio array de strings.
*/
void freeTokens(char **tokens) {
    if (tokens == NULL) {
        return; 
    }

    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }

    free(tokens);
}

/*
'splitLine()' é uma função que recebe uma linha de texto no formato CSV e divide essa linha em tokens usando o ponto e vírgula como delimitador.
Ela conta quantos tokens existem na linha, aloca a memória para armazenar esses mesmos tokens num array de strings e copia cada token para esse mesmo array.
No final, a função retorna o array de tokens.
*/
char **splitLine(const char *line) {
    int tokenCount = 1; 
    char **tokens = NULL;
    char *token, *endOfLine, *lineCopy;

    lineCopy = strdup(line);
    if (lineCopy == NULL) {
        return NULL; 
    }

    endOfLine = strchr(lineCopy, '\n');
    if (endOfLine) {
        *endOfLine = '\0';
    }

    for (char *tmp = lineCopy; *tmp; tmp++) {
        if (*tmp == ';') {
            tokenCount++;
        }
    }

    tokens = malloc((tokenCount + 1) * sizeof(char *));
    if (tokens == NULL) {
        free(lineCopy);
        return NULL;
    }

    char *saveptr = lineCopy;
    int i = 0;
    while ((token = strsep(&saveptr, ";")) != NULL) {
        tokens[i++] = strdup(token);
    }
    tokens[i] = NULL;

    free(lineCopy); 
    return tokens;
}

/*
A função 'parseCSVLine()' é uma função que encapsula a chamada da função 'splitLine'.
Ela recebe uma linha de texto no formato CSV e chama a 'splitLine' para dividir a linha em tokens.
De seguida, a mesma retorna o array de tokens resultante.
*/
char **parseCSVLine(const char *line) {
    return splitLine(line); 
}