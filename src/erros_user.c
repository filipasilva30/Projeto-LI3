#include "erros_user.h"

/*
A 'dataValida' verifica se uma string representa uma data no formato "AAAA/MM/DD". 
Retorna 1 se a string tem 10 caracteres e barras nas posições corretas e caso contrário retorna 0.
*/
int dataValida(const char *data) {
    if (strlen(data) != 10) return 0;

    // Verifica se os caracteres nas posições corretas são barras ('/')
    return data[4] == '/' && data[7] == '/';
}
/*
'contemArroba' conta quantas vezes o caractere '@' aparece em uma string.
Retorna 1 se houver exatamente um '@' senão, retorna 0.
*/
int contemArroba(const char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '@') {
            count++;
            // Verifica se o '@' é o último caractere
            if (str[i + 1] == '\0') {
                return 0; 
            }
        }
    }
    return count == 1; 
}

/*
A 'countryValido' averigua se uma string é igual a "PT" (Portugal). 
Retorna 1 se for igual e 0 se for diferente.
*/
int countryValido(const char* country) {
    return (strcmp(country, "PT") == 0); 
}

/*
A 'comparaAnos' compara os anos entre duas datas, uma de nascimento e outra de criação de conta. 
Retorna 1 se o ano de nascimento for maior (mais recente) que o ano de criação da conta, e 0 caso contrário.
*/
int comparaAnos(const char *data1, const char *data2) {
    // Extrai o ano de dataNascimento
    char ano1[5];
    strncpy(ano1, data1 + 6, 4); // Ano começa no índice 6
    ano1[4] = '\0';

    // Extrai o ano de dataCriaConta
    char ano2[5];
    strncpy(ano2, data2, 4); // Ano é os primeiros 4 caracteres
    ano2[4] = '\0';

    // Converte os anos para inteiro
    int anoNasc = atoi(ano1);
    int anoCria = atoi(ano2);

    return anoNasc > anoCria;
}


/*
'statusValido' é uma função que inspeciona se uma string representa um estado válido, que pode ser "active" ou "inactive". 
A função ignora maiúsculas/minúsculas e espaços extras no início e no final da string.
*/
int statusValido(const char* estado) {
    char* estadoTrimmed = trim((char*)estado);
    toLowerCase(estadoTrimmed);
    return (strcmp(estadoTrimmed, "active") == 0 || strcmp(estadoTrimmed, "inactive") == 0);
}
