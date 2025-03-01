#include "query7.h"


struct AirportDelays{
    char* nome;
    int* atrasos;
    int n_voos;
    int mediana;
};

/*
Esta função tem como objetivo comparar dois inteiros, usada para ordenar arrays de inteiros.
A mesma retorna a diferença entre dois inteiros, usada na função 'qsort'.
*/
int compararInt(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_a - int_b;
}

/*
Esta função tem como objetivo encontrar o índice de um aeroporto em um array com base em seu nome.
Percorre o array de 'AirportDelays' e compara os nomes dos aeroportos.
*/
int encontrarAeroporto(AirportDelays* aeroportos, int tamanho, const char* nome) {
    for (int i = 0; i < tamanho; i++) {
        if (strcasecmp(aeroportos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/*
Esta tem como objetivo calcular a mediana dos atrasos de um aeroporto.
Ordena o array de atrasos.
Calcula a mediana com base no número de elementos (par ou ímpar).
*/
int calcularMediana(int* atrasos, int n_voos) {
    if (n_voos == 0) return 0;

    qsort(atrasos, n_voos, sizeof(int),compararInt);

    if (n_voos % 2 != 0) {
    // Se o número de voos é ímpar, retorna o valor do meio
    return atrasos[n_voos / 2];
    } else {
    // Se é par, retorna a média dos dois valores do meio
    int meio = n_voos / 2;
    return (atrasos[meio - 1] + atrasos[meio]) / 2;
    }
}

/*
Esta função tem como objetivo comparar dois 'AirportDelays' para ordenar.
Primeiro, compara com base na mediana de atrasos.
Em caso de empate, compara os nomes dos aeroportos.
*/
int comparaDelays(const void *a, const void *b) {
    const AirportDelays *aeroportoA = (const AirportDelays *)a;
    const AirportDelays *aeroportoB = (const AirportDelays *)b;

    // Primeiro, comparar pela mediana de atraso (do maior para o menor)
    if (aeroportoA->mediana != aeroportoB->mediana) {
        return aeroportoB->mediana - aeroportoA->mediana; // Invertendo a ordem para decrescente
    }

    // Em caso de empate, comparar pelo nome do aeroporto (ordem crescente)
    return strcmp(aeroportoA->nome, aeroportoB->nome);
}

/*
A função 'query7()' tem como objetivo processar e exibir a mediana de atrasos dos aeroportos.
Começa por extrair o número N dos argumentos para listar os top N aeroportos.
Inicializa um array de 'AirportDelays' e processa cada voo no 'flightHashTable', adicionando atrasos aos aeroportos correspondentes.
Calcula a mediana de atrasos para cada aeroporto ordenando posteriormente os aeroportos por mediana de atrasos.
Prepara um arquivo ou terminal para saída e escreve informações sobre os top N aeroportos. Libertando por fim a memória alocada.
*/
void query7(bool formatFlag, char *args, int commandLineNum,FlightHashTable* flightHashTable, bool interativo) {

    int N = atoi(trim(args)); // Número de aeroportos a serem listados
    int conta = 1;

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

    // Inicializar array de AirportDelays
    AirportDelays* aeroportos = malloc(getFlightTableSize(flightHashTable) * sizeof(AirportDelays));
    int numAeroportos = 0;

    FlightIterator *iterador = initFlightIterator(flightHashTable);
    Flight *voo;

    // Processar cada voo
    while ((voo = getNextFlight(iterador)) != NULL) {

        const char* nomeAeroporto = getFlightOrigin(voo);
        char* hora_partida = strdup(getFlightPartida(voo));  // DAR FREE DISTO
        char* hora_partida_real = strdup(getFlightRealPartida(voo));  // DAR FREE DISTO
        int atraso = delay(hora_partida, hora_partida_real);

        free(hora_partida);
        free(hora_partida_real);

        int indice = encontrarAeroporto(aeroportos, numAeroportos, nomeAeroporto);
    
        if (indice == -1) {
        // Aeroporto não encontrado, adiciona novo
            aeroportos[numAeroportos].nome = strdup(nomeAeroporto); // Duplicar string para armazenar nome
            aeroportos[numAeroportos].atrasos = malloc(sizeof(int)); // Inicializar array de atrasos
            aeroportos[numAeroportos].atrasos[0] = atraso;
            aeroportos[numAeroportos].n_voos = 1;
            numAeroportos++;
        } else {
        // Aeroporto encontrado, atualiza dados existentes
            aeroportos[indice].n_voos++;
            aeroportos[indice].atrasos = realloc(aeroportos[indice].atrasos, aeroportos[indice].n_voos * sizeof(int));
            aeroportos[indice].atrasos[aeroportos[indice].n_voos - 1] = atraso;
        }
    }   

    free(iterador);

    for (int i = 0; i < numAeroportos; i++) {
        aeroportos[i].mediana = calcularMediana(aeroportos[i].atrasos, aeroportos[i].n_voos);
    }

    qsort(aeroportos, numAeroportos, sizeof(AirportDelays), comparaDelays);

    for (int i = 0; i < N && i < numAeroportos; i++) {
        int menorValor = (N < numAeroportos) ? N : numAeroportos;

        char lineBuffer[256]; // Buffer para armazenar uma única linha
        int lineLength;      

    if (interativo) {
        if (formatFlag) {
            lineLength = snprintf(lineBuffer, sizeof(lineBuffer), 
                                  "--- %d ---\nname: %s\nmedian:%d\n", 
                                  conta, aeroportos[i].nome, aeroportos[i].mediana);
        } else {
               lineLength = snprintf(lineBuffer, sizeof(lineBuffer), 
                                      "%s;%d\n", aeroportos[i].nome, aeroportos[i].mediana);
            
        }

        // Verificamos se o buffer de linha não excedeu o tamanho
        if (lineLength >= sizeof(lineBuffer)) {
            // Se chegarmos aqui, o buffer de linha está cheio e precisamos lidar com isso.
            // Podemos, por exemplo, imprimir o que coube e seguir para a próxima linha.
            lineBuffer[sizeof(lineBuffer) - 1] = '\0'; // Garantimos que a string está terminada
        }

        // Imprime o conteúdo do buffer de linha na tela.
        mvprintw((LINES - 10 + i) / 2, (COLS - strlen(lineBuffer)) / 2, "%s", lineBuffer);
        refresh();


    } else { 

        if (formatFlag){
            fprintf(outputFile, "--- %d ---\n", conta);
            fprintf(outputFile, "name: %s\n", aeroportos[i].nome);
            fprintf(outputFile, "median: %d\n", aeroportos[i].mediana);
            if (i < menorValor -1) fprintf(outputFile,"\n");

            conta++;
        }
        else {
            fprintf(outputFile, "%s;%d\n", aeroportos[i].nome, aeroportos[i].mediana);
        }}
    
    }

    if (interativo) {
    } else {
        fclose(outputFile);
    }
    for (int i = 0; i < numAeroportos; i++) {
        free(aeroportos[i].nome);
        free(aeroportos[i].atrasos);
    }
    free(aeroportos);

}