#include "query5.h"
#include "flight_catalogs.h"

/*
A função 'comparaDatas()' tem como objetivo comparar duas datas no formato AAAA/MM/DD HH:MM:SS.
A mesma desmembra as datas em componentes de ano, mês, dia, hora, minuto e segundo e as compara sequencialmente.
*/
int comparaDatas(const char *date1, const char *date2) {
    int year1, month1, day1, hour1, min1, sec1;
    int year2, month2, day2, hour2, min2, sec2;

    if (sscanf(date1, "%d/%d/%d %d:%d:%d", &year1, &month1, &day1, &hour1, &min1, &sec1) != 6 ||
        sscanf(date2, "%d/%d/%d %d:%d:%d", &year2, &month2, &day2, &hour2, &min2, &sec2) != 6) {
        return 0; 
    }

    if (year1 != year2) return year1 - year2;
    if (month1 != month2) return month1 - month2;
    if (day1 != day2) return day1 - day2;
    if (hour1 != hour2) return hour1 - hour2;
    if (min1 != min2) return min1 - min2;
    return sec1 - sec2;
}

/*
A função 'comparaVoos()' tem como objetivo comparar dois voos para ordenação:
Esta compara os voos pela data de partida usando a 'comparaDatas'.
Caso as datas sejam iguais, usa-se o ID do voo como critério de desempate.
A ordem de comparação é invertida para que os voos sejam ordenados do mais recente para o mais antigo.
*/
int comparaVoos(const void *a, const void *b) {
    const Flight *flightA = *(const Flight **)a;
    const Flight *flightB = *(const Flight **)b;

    // Comparar por data de partida estimada
    char* data_A = strdup(getFlightPartida(flightA));
    char* data_B = strdup(getFlightPartida(flightB));

    int dateComparison = comparaDatas(data_A, data_B);

    free(data_A);
    free(data_B);

    if (dateComparison != 0) {
        return -dateComparison;  // Inverte a ordem da comparação
    }

    // Em caso de empate na data, comparar pelo identificador do voo (ordem crescente)
    return strcmp(getFlightId(flightA), getFlightId(flightB));
}

/*
A função 'query5()' tem como objetivo processar uma consulta sobre voos de um aeroporto específico dentro de um intervalo de tempo.
A mesma começa por extrair o código do aeroporto e as datas de início e fim do intervalo.
Inicializa um iterador para a tabela hash de voos.
Filtra os voos com base no aeroporto de origem e no intervalo de datas.
Ordena os voos filtrados usando a função 'comparaVoos'.
Escreve os detalhes dos voos filtrados e ordenados em um arquivo ou terminal.
Libertando por fim os recursos alocados e fechando o arquivo de saída caso seja necessário.
*/
void query5(bool formatFlag, char *args, int commandLineNum, FlightHashTable* flightHashTable, bool interativo) {

    int conta = 1;
    char aeroporto[4], data_inicio[20], data_fim[20];
    sscanf(args, "%s \"%19[^\"]\" \"%19[^\"]\"", aeroporto, data_inicio, data_fim);

    FILE *outputFile;
    char outputFileName[1024];

    //configuração de saída
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

    int n_voos = getFlightTableCount(flightHashTable);

    int numFiltrados = 0;
    Flight **flightsFiltrados = malloc(n_voos * sizeof(Flight *));
    if (flightsFiltrados == NULL) {
        return;
    }

    FlightIterator *iterator = initFlightIterator(flightHashTable);
        if (iterator == NULL) {
        return;
    }

    Flight *voo;

    while ((voo = getNextFlight(iterator)) != NULL) {
        
        char* partida = strdup(getFlightPartida(voo));
        char* chegada = strdup(getFlightChegada(voo));

        if (strcasecmp(getFlightOrigin(voo), aeroporto) == 0 && comparaDatas(partida, data_inicio) >= 0 &&
            comparaDatas(chegada, data_fim) <= 0) {
            if (numFiltrados < n_voos) {
                flightsFiltrados[numFiltrados++] = voo;//O array é redimensionado conforme necessário para acomodar todos os voos relevantes.
            }
        }
        free(partida);
        free(chegada);
    }

    free(iterator);
    // função de comparação, onde a ordenação é feita primeiramente por data de partida e, em caso de empate, pelo ID do voo.
    qsort(flightsFiltrados, numFiltrados, sizeof(Flight *), comparaVoos);


    for (int i = 0; i < numFiltrados; i++) {

        char *dest = strdup(getFlightDest(flightsFiltrados[i]));
        toUpperCase(dest);

        if (formatFlag){
        fprintf(outputFile, "--- %d ---\n", conta);
        fprintf(outputFile, "id: %s\n", getFlightId(flightsFiltrados[i]));
        fprintf(outputFile, "schedule_departure_date: %s\n", getFlightPartida(flightsFiltrados[i]));
        fprintf(outputFile, "destination: %s\n", dest);
        fprintf(outputFile, "airline: %s\n", getFlightAirline(flightsFiltrados[i]));
        fprintf(outputFile, "plane_model: %s\n", getFlightPlane(flightsFiltrados[i]));
        if (i<numFiltrados-1) fprintf(outputFile,"\n");

        conta++;
      }
    else {
        fprintf(outputFile, "%s;%s;%s;%s;%s\n", getFlightId(flightsFiltrados[i]), getFlightPartida(flightsFiltrados[i]), dest, getFlightAirline(flightsFiltrados[i]), getFlightPlane(flightsFiltrados[i]));
    }
        free(dest);
    }

    //Libera o array flightsFiltrados para evitar perca de memória.
    free(flightsFiltrados);

    if (interativo) {
    } else {
        fclose(outputFile);
    }
    
}

