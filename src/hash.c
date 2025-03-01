#include "hash.h"

/*
A função 'hashFunction()' calcula um valor hash para uma string de entrada percorrendo cada caractere na string, combinando os valores ASCII dos caracteres usando uma técnica de "hashing multiplication". 
O valor hash calculado é acumulado e, no final, é retornado como resultado da função.
Esse valor hash é usado para representar a string de entrada de forma compacta.
*/
unsigned long hashFunction(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash; // Retorna apenas o valor hash
}
