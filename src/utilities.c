#include "utilities.h"

/*
'trim' é uma função que remove espaços no início e no final de uma string.
A mesma remove primeiro espaços à esquerda, depois verifica se a string não ficou vazia, e por fim remove espaços à direita. 
Retorna a string sem os espaços extras.
 */
char* trim(char* str) {
    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = 0;

    return str;
}




/*
A função 'tolowerCase' converte uma string para letras minúsculas. 
Ela percorre cada caractere da string e usa a função 'tolower' para transformar cada letra na sua versão minúscula.
*/
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}



/*
'toUpperCase()' é uma função que converte todos os caracteres de uma string para maiúsculas e retorna a string resultante.
Se a string de entrada for nula, a função retorna nula.
*/
char *toUpperCase(char *str) {
    if (str == NULL) return NULL;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
    return str;
}

/*
A funçáo 'testsInteiro()' converte uma string para um valor inteiro e armazena o resultado em out_value. 
Retorna 1 se a conversão for bem-sucedida e 0 se falhar.
Se a string de entrada for nula, a função também retorna 0.
*/
int testaInteiro(const char *str, int *out_value) {
    if (str == NULL) return 0;

    //atoi converte a string para um inteiro. 
    int val = atoi(str);

    if (val == 0 && str[0] != '0') {
        return 0;
    }

    *out_value = val;
    return 1;
}

/*
'concat()' é uma função que concatena duas strings (s1 e s2) e retorna a nova string resultante. 
Se uma das strings de entrada for nula, a função retorna nula.
*/
char* concat(const char *s1, const char *s2) {

if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    if (result == NULL) {
        perror("Erro ao alocar memória para concatenação");
        exit(EXIT_FAILURE);
    }

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}


/*
A função 'calcularNoite()' calcula o número de noites entre duas datas no formato "AAAA/MM/DD". 
A função converte as datas em time_t, calcula a diferença em segundos e retorna o resultado em dias. 
Retorna -1 em caso de erro na conversão das datas.
*/
int calcularNoite(const char* data1, const char* data2) {
    struct tm tm1 = {0}, tm2 = {0};
    time_t t1, t2;

    // Ler os componentes da data diretamente da string
    if (sscanf(data1, "%d/%d/%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday) != 3 ||
        sscanf(data2, "%d/%d/%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday) != 3) {
        fprintf(stderr, "Erro ao converter datas.\n");
        return -1; 
    }   

    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;

    // Converter as datas para time_t
    t1 = mktime(&tm1);
    t2 = mktime(&tm2);

    // Calcula a diferença em segundos e converter para dias
    double diferenca = difftime(t2, t1);
    return (int)(diferenca / (60 * 60 * 24)); // Retorna a diferença em dias
}

/*
A função 'calcularPrecoTotal' calcula o preço total com base no preço por noite, taxa da cidade e o período de estadia.
*/
double calcularPrecoTotal(int x, int y, const char* data1, const char* data2) {
    double precoPorNoite = (double)x;
    double taxaCidade = (double)y;

    int numNoites = calcularNoite(data1, data2);

    double precoTotal = precoPorNoite * numNoites;
    double imposto = (precoTotal * taxaCidade) / 100;

    // Devolve o preço total incluindo o imposto
    return (precoTotal + imposto);
}

/*
A função 'calcularIdade()' calcula a idade com base na data de nascimento no formato "AAAA/MM/DD". 
A função obtém a data atual, extrai os componentes da data de nascimento, calcula a diferença em anos e ajusta a idade com base no mês e no dia de nascimento.
*/
int calcularIdade(const char *dataNascimento) {
    // Obter a data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Extrair ano, mês e dia da data de nascimento
    char anoStr[5], mesStr[3], diaStr[3];
    strncpy(anoStr, dataNascimento, 4); // Copia os primeiros 4 caracteres para o ano
    strncpy(mesStr, dataNascimento + 5, 2); // Copia os caracteres do mês
    strncpy(diaStr, dataNascimento + 8, 2); // Copia os caracteres do dia

    anoStr[4] = mesStr[2] = diaStr[2] = '\0';

    // Converte as strings para inteiros
    int anoNascimento = atoi(anoStr);
    int mesNascimento = atoi(mesStr);
    int diaNascimento = atoi(diaStr);

    // Calcula a idade
    int idade = tm.tm_year + 1900 - anoNascimento;
    // Se o mês atual é anterior ao mês de nascimento, ou se é o mesmo mês mas o dia atual é anterior ao dia de nascimento, subtrair 1 à idade
    if (tm.tm_mon + 1 < mesNascimento || (tm.tm_mon + 1 == mesNascimento && tm.tm_mday < diaNascimento)) {
        idade--;
    }

    return idade;
}

/*
'timeToSeconds()' é uma função que converte os componentes de data e hora em segundos, simplificando a conversão, sem levar em conta anos bissextos, etc.
*/
int timeToSeconds(int year, int month, int day, int hour, int minute, int second) {
    return second + minute * 60 + hour * 3600 + day * 86400 + month * 2629743 + year * 31556926;
}

/*
A função 'extractComponents()' extrai os componentes de data e hora de uma string no formato "AAAA/MM/DD hh:mm:ss".
*/
int extractComponents(const char* dateTime, int *year, int *month, int *day, int *hour, int *minute, int *second) {
    return sscanf(dateTime, "%d/%d/%d %d:%d:%d", year, month, day, hour, minute, second);
}

/*
 delay()' é uma função que calcula o atraso em segundos entre a duas horas. 
A função extrai os componentes de data e hora das duas horas de partida e calcula a diferença em segundos. 
*/
int delay(const char* hora_partida, const char* horareal_partida) {
    int year1, month1, day1, hour1, minute1, second1;
    int year2, month2, day2, hour2, minute2, second2;

    // Extrair componentes da hora de partida prevista
    if (extractComponents(hora_partida, &year1, &month1, &day1, &hour1, &minute1, &second1) != 6) {
        return -1; // Erro na extração de componentes
    }

    // Extrair componentes da hora de partida real
    if (extractComponents(horareal_partida, &year2, &month2, &day2, &hour2, &minute2, &second2) != 6) {
        return -1; // Erro na extração de componentes
    }

    // Converte para segundos
    int time1 = timeToSeconds(year1, month1, day1, hour1, minute1, second1);
    int time2 = timeToSeconds(year2, month2, day2, hour2, minute2, second2);

    // Calcula a diferença
    return abs(time2 - time1);
}

//A função 'isNumber' verifica se uma string passada como argumento é composta inteiramente por dígitos numéricos.
bool isNumber(const char *str) {
    while (*str) {
        if (!isdigit((unsigned char)*str)) return false;
        str++;
    }
    return true;
}

/*
A função 'dataCriarValida' afere se uma string representa uma data e hora no formato "AAAA/MM/DD HH:MM:SS". 
Retorna 1 se a string tem 19 caracteres e os separadores estão nas posições corretas.
Caso contrário, retorna 0.
*/
int dataCriarValida(const char *dataCriar) {
    // Verifica o comprimento da string (AAAA/MM/DD HH:MM:SS)
    if (dataCriar == NULL) return 0;
    if (strlen(dataCriar) != 19) return 0;

    // Verifica os separadores de data e Criar
    return dataCriar[4] == '/' && dataCriar[7] == '/' && 
           dataCriar[10] == ' ' && dataCriar[13] == ':' && dataCriar[16] == ':';
}