#include "flights.h"


struct Flight {
    char* id;
    char* airline;
    char* modelo;
    int seats;
    char* origin;
    char* destination;
    char* departure_date;
    char* arrival_date;
    char* real_departure_date;
    char* real_arrival_date;
    char* pilot;
    char* copilot;
    char* notes;
};

void freeFlights(Flight *flight) {
    if(flight==NULL) return;
    free(flight->id);
    free(flight->airline);
    free(flight->modelo);
    free(flight->origin);
    free(flight->destination);
    free(flight->departure_date);
    free(flight->arrival_date);
    free(flight->real_departure_date);
    free(flight->real_arrival_date);
    free(flight->pilot);
    free(flight->copilot);
    free(flight->notes);
    free(flight);
}

/*
A função 'criaFlight()' cria e inicializa um novo objeto Flight a partir de um array de strings tokens. 
A mesma registra voos inválidos em um arquivo especificado.
Verifica ainda se o cabeçalho para voos inválidos já foi impresso no arquivo, caso contrário imprime-o.
Aloca memória para um novo voo e inicializa os seus campos com os valores fornecidos em tokens.
Verifica a validade dos dados (códigos de aeroporto, horários, e consistência de datas). 
Caso algum dado seja inválido, o voo é registrado no arquivo de voos inválidos, e a memória alocada é liberada. 
*/
Flight* criarFlight(char **tokens, FILE *invalidFlightsFile) {

    static int headerPrinted = 0;

    if (headerPrinted == 0) {
        fprintf(invalidFlightsFile, "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes\n");
        headerPrinted = 1;
    }


    Flight *newFlight = malloc(sizeof(Flight));
    if (newFlight == NULL) {
        return NULL; 
    }

    *newFlight = (Flight){0};
    newFlight->id = tokens[0] && strcmp(tokens[0], "") != 0 ? strdup(tokens[0]) : NULL;
    newFlight->airline = tokens[1] && strcmp(tokens[1], "") != 0 ? strdup(tokens[1]) : NULL;
    newFlight->modelo = tokens[2] && strcmp(tokens[2], "") != 0 ? strdup(tokens[2]) : NULL;
    newFlight->origin = tokens[4] && strcmp(tokens[4], "") != 0 ? strdup(tokens[4]) : NULL;
    newFlight->destination = tokens[5] && strcmp(tokens[5], "") != 0 ? strdup(tokens[5]) : NULL;
    newFlight->departure_date = tokens[6] && strcmp(tokens[6], "") != 0 ? strdup(tokens[6]) : NULL;
    newFlight->arrival_date = tokens[7] && strcmp(tokens[7], "") != 0 ? strdup(tokens[7]) : NULL;
    newFlight->real_departure_date = tokens[8] && strcmp(tokens[8], "") != 0 ? strdup(tokens[8]) : NULL;
    newFlight->real_arrival_date = tokens[9] && strcmp(tokens[9], "") != 0 ? strdup(tokens[9]) : NULL;
    newFlight->pilot = tokens[10] && strcmp(tokens[10], "") != 0 ? strdup(tokens[10]) : NULL;
    newFlight->copilot = tokens[11] && strcmp(tokens[11], "") != 0 ? strdup(tokens[11]) : NULL;
    newFlight->notes = tokens[12] && strcmp(tokens[12], "") != 0 ? strdup(tokens[12]) : NULL;

    int seats;
    // Validações
    if (!newFlight->id || !newFlight->airline || !newFlight->modelo ||
        !newFlight->origin || !isValidAirportCode(newFlight->origin) ||
        !newFlight->destination || !isValidAirportCode(newFlight->destination) ||
        !newFlight->departure_date || !dataCriarValida(newFlight->departure_date) ||
        !newFlight->arrival_date || !dataCriarValida(newFlight->arrival_date) ||
        !dataCriarValida(newFlight->real_arrival_date) || !dataCriarValida(newFlight->real_departure_date) ||
        !isDepartureBeforeArrival(newFlight->departure_date, newFlight->arrival_date) ||
        !isDepartureBeforeArrival(newFlight->real_departure_date, newFlight->real_arrival_date) ||
        !newFlight->pilot || !newFlight->copilot || !testaInteiro(tokens[3],&seats)) {

        // Registrar o erro no arquivo
        fprintf(invalidFlightsFile, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
            tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], tokens[6],
            tokens[7], tokens[8], tokens[9], tokens[10], tokens[11], tokens[12]);
        
        freeFlights(newFlight);
        return NULL;
    }

    newFlight -> seats = seats;
    return newFlight;
}

// 'getFlightId'- Retorna o ID do voo.
const char* getFlightId(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->id;  
}

//'getFlightAirline'- Retorna a companhia aérea.
const char* getFlightAirline(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->airline;  
}

//'getFlightModel'- Retorna o modelo do avião.
const char* getFlightModel(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->modelo;  
}

//'getFlightOrigin'- Retorna o aeroporto de origem.
const char* getFlightOrigin(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->origin;  
}

//'getFlightDest'- Retorna o aeroporto de destino.
const char* getFlightDest(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->destination;  
}

//'getFlightPartida'- Retorna a data/hora programada de partida.
const char* getFlightPartida(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->departure_date;  
}

//'getFlightRealPartida'- Retorna a data/hora real de partida.
const char* getFlightRealPartida(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->real_departure_date;  
}

//'getFlightChegada'- Retorna a data/hora programada de chegada.
const char* getFlightChegada(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->arrival_date;  
}

//'getFlightPlane'- Retorna novamente o modelo do avião (possível duplicação).
const char* getFlightPlane(const Flight *flight) {
    if (flight == NULL) return NULL;
    return flight->modelo;  
}

/*
A função 'anoCoincide()' verifica se o ano da data de partida de um voo coincide com um ano especificado.
A mesma extrai o ano da data de partida de um voo e compara-a com o ano fornecido. 
Retorna true se coincidir, false caso contrário ou se houver um erro na extração da data.
*/
bool  anoCoincide(const Flight *flight, int year) {
    if (flight == NULL || flight->departure_date == NULL) {
        return false; 
    }

    int flightYear, month, day, hour, minute, second;
    if (extractComponents(flight->departure_date, &flightYear, &month, &day, &hour, &minute, &second) != 6) {
        return false; 
    }

    return flightYear == year;
}
