#include "query6.h"

struct FlightIds {
    char **ids;
    int numIds;
};


struct AirportPassengerCount {
    char* name;
    int passengerCount;
    FlightIds flightIds;
};

/*
Esta função tem como objetivo comparar dois aeroportos com base no número de passageiros e, em caso de empate, pelo nome.
A mesma retorna um valor que determina a ordem dos aeroportos na ordenação.
*/
int compareAirport(const void *a, const void *b) {
    const AirportPassengerCount *airportA = (const AirportPassengerCount *)a;
    const AirportPassengerCount *airportB = (const AirportPassengerCount *)b;

    if (airportB->passengerCount != airportA->passengerCount) {
        return airportB->passengerCount - airportA->passengerCount;
    }
    return strcmp(airportA->name, airportB->name);
}

/*
A função 'isFlightIdInAirport()' tem como objetivo verificar se um ID de voo específico está associado a um aeroporto.
Esta compara o ID do voo dado com os IDs armazenados no aeroporto.
*/
bool isFlightIdInAirport(const FlightIds *flightIds, const char *flightId) {
    if (flightIds == NULL || flightId == NULL) {
        return false;
    }

    for (int i = 0; i < flightIds->numIds; i++) {
        if (strcmp(flightIds->ids[i], flightId) == 0) {
            return true; 
        }
    }

    return false;
}

/*
A função 'addFlightIdToAirport()' tem como objetivo adicionar um ID de voo ao registro de um aeroporto.
A mesma localiza o aeroporto no array.
Se necessário, aloca memória para um novo aeroporto.
Adiciona o ID do voo ao aeroporto.
*/
void addFlightIdToAirport(AirportPassengerCount **airportCounts, int *numAirports, int *capacity, const char *airportName, const char *flightId) {
    if (airportName == NULL || flightId == NULL) {
        return; 
    }

    int airportIndex = -1;
    // Procurar o aeroporto no array
    for (int i = 0; i < *numAirports; i++) {
        if (strcmp((*airportCounts)[i].name, airportName) == 0) {
            airportIndex = i;
            break;
        }
    }

    // Se o aeroporto não estiver no array, adicione-o
    if (airportIndex == -1) {
        if (*numAirports == *capacity) {
            // Expandir a capacidade, se necessário
            *capacity *= 2;
            *airportCounts = realloc(*airportCounts, (*capacity) * sizeof(AirportPassengerCount));
            if (*airportCounts == NULL) {
                perror("Failed to realloc airportCounts");
                exit(EXIT_FAILURE);
            }
        }
        airportIndex = *numAirports;
        (*numAirports)++;
        (*airportCounts)[airportIndex].name = strdup(airportName);
        (*airportCounts)[airportIndex].passengerCount = 0;
        (*airportCounts)[airportIndex].flightIds.ids = NULL;
        (*airportCounts)[airportIndex].flightIds.numIds = 0;
    }

    // Adicionar ID do voo ao aeroporto
    AirportPassengerCount *airport = &((*airportCounts)[airportIndex]);
    airport->flightIds.ids = realloc(airport->flightIds.ids, (airport->flightIds.numIds + 1) * sizeof(char *));
    if (airport->flightIds.ids == NULL) {
        perror("Failed to realloc flightIds.ids");
        exit(EXIT_FAILURE);
    }
    airport->flightIds.ids[airport->flightIds.numIds] = strdup(flightId);
    airport->flightIds.numIds++;
}

/*
A função 'query6()' tem como objetivo processar a contagem de passageiros por aeroporto para um ano específico.
A mesma extrai o ano e o número N (para listar os top N aeroportos) dos argumentos.
Prepara um array dinâmico para armazenar os dados dos aeroportos.
Usa um iterador para passar por todos os voos, adicionando seus IDs aos aeroportos de origem e destino se o voo ocorrer no ano especificado.
Para cada passageiro no catálogo de passageiros, incrementa a contagem de passageiros do aeroporto correspondente ao seu voo.
Ordena os aeroportos com base na contagem de passageiros.
Prepara um arquivo ou terminal para saída e escreve informações sobre os top N aeroportos. E por fim liberta a memória alocada.
*/
void query6(bool formatFlag, char *args, int commandLineNum, FlightHashTable *flightHashTable, const PassengerCatalog *passengerCatalog, bool interativo) {
    int year, N;
    sscanf(args, "%d %d", &year, &N);

    // Estrutura para armazenar a contagem de passageiros por aeroporto
    AirportPassengerCount *airportCounts = calloc(1, sizeof(AirportPassengerCount));
    int numAirports = 0;
    int capacity = 1;

    /// Iterar sobre todos os voos, adicionando seus IDs aos aeroportos correspondentes
    FlightIterator *iterator = initFlightIterator(flightHashTable);
    Flight *currentFlight;
    while ((currentFlight = getNextFlight(iterator)) != NULL) {
        if (anoCoincide(currentFlight, year)) {
            addFlightIdToAirport(&airportCounts, &numAirports, &capacity, getFlightOrigin(currentFlight), getFlightId(currentFlight));
            addFlightIdToAirport(&airportCounts, &numAirports, &capacity, getFlightDest(currentFlight), getFlightId(currentFlight));
        }
    }
    free(iterator);

    // Contar passageiros para cada aeroporto percorrendo o passengerCatalog
    for (int i = 0; i < getPassengerCatalogNum(passengerCatalog); i++) {
        Passenger *passenger = getPassenger(passengerCatalog, i);
        const char* passengerFlightId = getPassengerFlight(passenger);
        for (int j = 0; j < numAirports; j++) {
            if (isFlightIdInAirport(&airportCounts[j].flightIds, passengerFlightId)) {
                airportCounts[j].passengerCount++;
            }
        }
    }

    // Ordenar os aeroportos com base na contagem de passageiros
    qsort(airportCounts, numAirports, sizeof(AirportPassengerCount), compareAirport);

    // Preparar arquivo de saída
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

    // Escrever os top N aeroportos
    for (int i = 0; i < N && i < numAirports; i++) {
        int menorValor = (N < numAirports) ? N : numAirports;
        if (formatFlag) {
            fprintf(outputFile, "--- %d ---\n", i + 1);
            fprintf(outputFile, "name: %s\n", airportCounts[i].name);
            fprintf(outputFile, "passengers: %d\n", airportCounts[i].passengerCount);
            if (i < menorValor -1) fprintf(outputFile, "\n");
        } else {
            fprintf(outputFile, "%s;%d\n", airportCounts[i].name, airportCounts[i].passengerCount);
        }
    }

    if (interativo) {
    } else {
        fclose(outputFile);
    }
    
    for (int i = 0; i < numAirports; i++) {
        free(airportCounts[i].name);
    }
    free(airportCounts);
}