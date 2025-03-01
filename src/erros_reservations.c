#include "erros_reservations.h"

// A função 'isIntegerValueValid' verifica se uma string representa um valor inteiro válido que é maior ou igual a um valor mínimo especificado.
int isIntegerValueValid(int value, int minValidValue) {
    return value >= minValidValue;
}

// A 'verificaBreakfast' inspeciona se o valor de uma string é uma representação válida para a disponibilidade de pequenos-almoços.
int verificaBreakfast(const char *breakfast) {

    if (breakfast == NULL || strlen (breakfast) == 0) return 1;

    // Compara com as strings permitidas, ignorando maiúsculas/minúsculas
    if (strcasecmp(breakfast, "False") == 0 || strcasecmp(breakfast, "True") == 0 ||
        strcasecmp(breakfast, "false") == 0 || strcasecmp(breakfast, "true") == 0 ||
        strcasecmp(breakfast, "1") == 0 || strcasecmp(breakfast, "0") == 0 ||
        strcasecmp(breakfast, "FALSE") == 0 || strcasecmp(breakfast, "TRUE")==0 ||
        strcasecmp(breakfast, "F") == 0 || strcasecmp(breakfast, "T") ==0 ||
        strcasecmp(breakfast, "f") == 0 || strcasecmp(breakfast, "t") == 0)   {
        return 1; 
    }
    
    return 0;
}

/* 
Esta função verifica a validade de uma classificação (rating). 
Ela considera válido se a string não for nula ou vazia e contiver exatamente um dígito (digito esse entre 1 e 5).
*/
int isRatingValid(int rate) {
    // Verifica se o número está entre 1 e 5
    if (rate >= 1 && rate <= 5) {
        return 1; 
    }

    return 0;
}
