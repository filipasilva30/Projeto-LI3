#include "reservations.h"


 struct Reservation {
    char* id;
    char* user_id;
    char* hotel_id;
    char* hotel_name;
    int hotel_stars;
    int city_tax;
    char* address;
    char* begin_date;
    char* end_date;
    int price;
    char* breakfast;
    char* room_details;
    int rate;
    char* comment;
};

void freeReservation(Reservation *reservation) {
    if (reservation == NULL) return;
    free(reservation->id);
    free(reservation->user_id);
    free(reservation->hotel_id);
    free(reservation->hotel_name);
    free(reservation->address);
    free(reservation->begin_date);
    free(reservation->end_date);
    free(reservation->breakfast);
    free(reservation->room_details);
    free(reservation->comment);
    free(reservation);
}

/*
A função 'criarReservation()' tem como objetivo criar e inicializar uma nova instância da estrutura 'Reservation'.
A mesma verifica se o cabeçalho para reservas inválidas já foi impresso, caso contrário imprime-o.
Aloca memória para uma nova reserva e inicializa os seus campos com valores fornecidos.
Verifica a validade dos dados. Se for inválido, registra no arquivo de reservas inválidas e liberta a memória.
*/
Reservation* criarReservation(char **tokens, FILE *invalidReservationsFile, UserHashTable *userHashTable) {
    
    static int headerPrinted = 0;  // Variável estática para controlar a impressão do cabeçalho

    if (headerPrinted == 0) {
        fprintf(invalidReservationsFile, "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment\n");
        headerPrinted = 1;
    }
    Reservation *newReservation = malloc(sizeof(Reservation));
    if (newReservation == NULL) {
        return NULL; // Falha na alocação de memória
    }

    *newReservation = (Reservation){0};
    newReservation->id = tokens[0] && strcmp(tokens[0], "") != 0 ? strdup(tokens[0]) : NULL;
    newReservation->user_id = tokens[1] && strcmp(tokens[1], "") != 0 ? strdup(tokens[1]) : NULL;
    newReservation->hotel_id = tokens[2] && strcmp(tokens[2], "") != 0 ? strdup(tokens[2]) : NULL;
    newReservation->hotel_name = tokens[3] && strcmp(tokens[3], "") != 0 ? strdup(tokens[3]) : NULL;
    newReservation->address = tokens[6] && strcmp(tokens[6], "") != 0 ? strdup(tokens[6]) : NULL;
    newReservation->begin_date = tokens[7] && strcmp(tokens[7], "") != 0 ? strdup(tokens[7]) : NULL;
    newReservation->end_date = tokens[8] && strcmp(tokens[8], "") != 0 ? strdup(tokens[8]) : NULL;
    newReservation->breakfast = tokens[10] && strcmp(tokens[10], "") != 0 ? strdup(tokens[10]) : NULL;
    newReservation->room_details = tokens[11] && strcmp(tokens[11], "") != 0 ? strdup(tokens[11]) : NULL;
    newReservation->comment = tokens[13] && strcmp(tokens[13], "") != 0 ? strdup(tokens[13]) : NULL;


    int hotel_stars, city_tax, price, rate;
    // Validações
    if ((newReservation->id == NULL || strcmp(newReservation->id, "") == 0) 
                || (newReservation->user_id == NULL) || (strcmp(newReservation->user_id, "") == 0)
                || (existeUser(userHashTable, newReservation->user_id) == -1) 
                || newReservation->hotel_id == NULL || strcmp(newReservation->hotel_id, "") == 0
                || newReservation->hotel_name == NULL || (strcmp(newReservation->hotel_name, "") == 0) 
                || (newReservation->address ==NULL) || (strcmp(newReservation->address, "") == 0)
                || newReservation->begin_date == NULL || (strcmp(newReservation->begin_date, "") == 0)  
                || !dataValida(newReservation->begin_date) || newReservation->end_date == NULL 
                || (strcmp(newReservation->end_date, "") == 0) || !dataValida(newReservation->end_date)
                || (strcmp(newReservation->begin_date, newReservation->end_date) > 0)
                || verificaBreakfast(newReservation->breakfast)==0 ||newReservation->room_details == NULL 
                || (strcmp(newReservation->room_details, "") == 0)
                || !testaInteiro(tokens[4], &hotel_stars) || !testaInteiro(tokens[5], &city_tax) 
                || !testaInteiro(tokens[9], &price) || !testaInteiro(tokens[12], &rate)
                || !isIntegerValueValid(price, 1) || !isIntegerValueValid(city_tax, 0) || !isRatingValid(rate)
                || !isRatingValid(hotel_stars))  {
       
        // Registrar o erro no arquivo
        fprintf(invalidReservationsFile, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
            tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5],
            tokens[6], tokens[7], tokens[8], tokens[9], tokens[10], tokens[11],
            tokens[12], tokens[13]);
        freeReservation(newReservation);
        return NULL;
    }

    newReservation->hotel_stars = hotel_stars;
    newReservation->city_tax = city_tax;
    newReservation->price = price;
    newReservation->rate = rate;

    return newReservation;
}

//'getReservationId'- Retorna o ID da reserva.
const char* getReservationId(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->id;
}

//'getReservationHotelId'- Retorna o ID do hotel.
const char* getReservationHotelId(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->hotel_id;
}

//'getReservationUserId'- Retorna o ID do usuário.
const char* getReservationUserId(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->user_id;
}

//'getReservationRate'- Retorna a avaliação da reserva.
int getReservationRate(const Reservation *reservation) {
    return reservation->rate;
}

//'getReservationDataIni'- Retorna a data de início da reserva.
const char* getReservationDataIni(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->begin_date;
}

//'getReservationDataFim'- Retorna a data de término da reserva.
const char* getReservationDataFim(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->end_date;
}

//'getReservationPrice'- Retorna o preço da reserva.
int getReservationPrice(const Reservation *reservation) {
    return reservation->price;
}

//'getReservationTax'- Retorna a taxa da cidade.
int getReservationTax(const Reservation *reservation) {
    return reservation->city_tax;
}

//'getReservationBreakfast: Retorna informações sobre o pequeno-almoço.
const char* getReservationBreakfast(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->breakfast;
}

//'getReservationHotel_Name'- Retorna o nome do hotel.
const char* getReservationHotel_Name(const Reservation *reservation) {
    if (reservation == NULL) return NULL;
    return reservation->hotel_name;
}

//'getReservationStars'- Retorna a classificação em estrelas do hotel.
int getReservationStars(const Reservation *reservation) {;
    return reservation->hotel_stars;
}

