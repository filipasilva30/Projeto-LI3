#include "query1.h"


/*
Se o Id for de uma reserva, como começa por Book, a função retorna 1.
Se o Id for de um Voo (ou seja, só tem números), a função retorna 2.
Se não cumprir nenhuma dessas premissas podemos afirmar que se trata de um id de um User, retornando 3.
*/


int determinarID (const char* id) {
    if (strncmp (id, "Book", 4) == 0) {
        return 1;
    }
    
    if (atoi(id) != 0) return 2;

    
    return 3;
}


/*
De acordo com o return da determinarID, sabemos de que Id se trata.
Nesta função 'query1()' apenas utilizamos funções já definidas no utilities.h para calcular os dados que nos são pedidos.
Caso não seja encontrado o id que nos foi pedido, a função retorna imediatamente não imprimindo nada no ficheiro de texto.
Para além disso, se for um identificador de um usuário, confirmamos também se está ativo ou inativo.
No final, como em todas as queries, é impresso o resultado num ficheiro de texto.
*/


void query1 (bool formatFlag, char* args, int commandLineNum,const UserHashTable* userHashTable,const FlightHashTable* flightHashTable, const PassengerCatalog* passengerCatalog, ReservationHashTable* reservationHashTable, bool interativo) {
    // limpa espaços extras dos argumentos
    char* argumentos = trim(args);

    FILE *outputFile;
    char outputFileName[1024];

    //define a saída como terminal ou arquivo
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
    //processa com base no tipo ID
    if (determinarID (argumentos) == 1) {
        Reservation *reserva = findReservationById(reservationHashTable, argumentos);
        if (reserva == NULL) {
            fclose(outputFile);
            return;
        }

        const char* id = getReservationUserId(reserva);
        User *usuario = findUserById(userHashTable, id);
        
        if(strcasecmp(getUserStatus(usuario), "inactive")==0) {
            fclose(outputFile);
            return;
        } 

        const char *hotel_id = getReservationHotelId(reserva);
        const char *begin_date = getReservationDataIni(reserva);
        const char *end_date = getReservationDataFim(reserva);
        int price = getReservationPrice(reserva);
        int city_tax = getReservationTax(reserva);
        const char *breakfast = getReservationBreakfast(reserva);
        const char *hotel_name = getReservationHotel_Name(reserva);
        int hotel_stars = getReservationStars(reserva);
        int noites = calcularNoite(begin_date, end_date);
        double precoTotal = calcularPrecoTotal(price, city_tax, begin_date, end_date);

        char* breakfastAux = "False";
        if (strcasecmp(breakfast, "true") == 0 || strcasecmp(breakfast, "1") == 0) breakfastAux = "True";

        if (interativo) {
        char output[1024];
        if (formatFlag) {
            sprintf(output,
                "--- 1 ---\n"
                "hotel_id: %s\n"
                "hotel_name: %s\n"
                "hotel_stars: %d\n"
                "begin_date: %s\n"
                "end_date: %s\n"
                "includes_breakfast: %s\n"
                "nights: %d\n"
                "total_price: %.3f\n",
                hotel_id, hotel_name, hotel_stars, begin_date, end_date, 
                breakfastAux, noites, precoTotal);
        } else {
            sprintf(output, "%s;%s;%d;%s;%s;%s;%d;%.3f\n", 
                hotel_id, hotel_name, hotel_stars, begin_date, end_date, 
                breakfastAux, noites, precoTotal);
        }
        int numLines = formatFlag ? 2 : 1; 
        int startRow = LINES / 2 - numLines / 2; 

       const char* line = strtok(output, "\n");
        while (line != NULL) {
            mvprintw(startRow++, (COLS - strlen(line)) / 2, "%s", line);
            line = strtok(NULL, "\n");
        }
    } else {
        if (formatFlag) {
            fprintf(outputFile, "--- 1 ---\n");
            fprintf(outputFile, "hotel_id: %s\n", hotel_id);
            fprintf(outputFile, "hotel_name: %s\n", hotel_name);
            fprintf(outputFile, "hotel_stars: %d\n", hotel_stars);
            fprintf(outputFile, "begin_date: %s\n", begin_date);
            fprintf(outputFile, "end_date: %s\n", end_date);
            fprintf(outputFile, "includes_breakfast: %s\n", breakfastAux);
            fprintf(outputFile, "nights: %d\n", noites);
            fprintf(outputFile, "total_price: %.3f\n", precoTotal);
        } else {
            fprintf(outputFile, "%s;%s;%d;%s;%s;%s;%d;%.3f\n", 
                hotel_id, hotel_name, hotel_stars, begin_date, end_date, 
                breakfastAux, noites, precoTotal);
        }
    }

    if (interativo) {
    } else {
        fclose(outputFile);
    }


//        escreve(formatFlag, 11, interativo, ' ', hotel_stars, noites, 0, hotel_id, hotel_name, begin_date, end_date, breakfastAux, NULL,precoTotal, outputFile);
    
    }       

    if (determinarID (argumentos) == 2) {

    Flight *voo = findFlightById(flightHashTable, argumentos);

        if (voo == NULL) {
            fclose(outputFile);
            return; // Voo não encontrado
        }

        const char* airline = getFlightAirline(voo);
        const char* model = getFlightModel(voo);
        const char* origin = getFlightOrigin(voo);
        const char* destination = getFlightDest(voo);
        const char* hora_partida = getFlightPartida(voo);
        const char* horareal_partida = getFlightRealPartida(voo);
        const char* hora_chegada = getFlightChegada(voo);

        int passageiros = 0;
        int j = getPassengerCatalogNum(passengerCatalog);

        for(int i=0; i<j;i++){
           const char* aux = getPassengerFlight(getPassenger(passengerCatalog,i));
            if(strcmp(aux,argumentos)==0) passageiros++;
        }

        char* aux_hora_partida = strdup(hora_partida);
        char* aux_hora_real_partida = strdup(horareal_partida);

        int atraso = delay(aux_hora_partida, aux_hora_real_partida);

        free(aux_hora_partida);
        free(aux_hora_real_partida);

        if (interativo) {
        char output[1024];
        if (formatFlag) {
            sprintf(output,
                "--- 1 ---\n"
                "airline: %s\n"
                "plane_model: %s\n"
                "origin: %s\n"
                "destination: %s\n"
                "schedule_departure_date: %s\n"
                "schedule_arrival_date: %s\n"
                "passengers: %d\n"
                "delay: %d\n", // Aqui, cada linha termina com "\", exceto a última
                airline, model, origin, destination, hora_partida, 
                hora_chegada, passageiros, atraso);
        } else {
            sprintf(output, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
                airline, model, origin, destination, hora_partida, 
                hora_chegada, passageiros, atraso);
        }
        int numLines = formatFlag ? 2 : 1; 
        int startRow = LINES / 2 - numLines / 2; 

       const char* line = strtok(output, "\n");
        while (line != NULL) {
            mvprintw(startRow++, (COLS - strlen(line)) / 2, "%s", line);
            line = strtok(NULL, "\n");
        }
    } else {
        if (formatFlag) {
            fprintf(outputFile, "--- 1 ---\n");
            fprintf(outputFile, "airline: %s\n", airline);
            fprintf(outputFile, "plane_model: %s\n", model);
            fprintf(outputFile, "origin: %s\n", origin);
            fprintf(outputFile, "destination: %s\n", destination);
            fprintf(outputFile, "schedule_departure_date: %s\n", hora_partida);
            fprintf(outputFile, "schedule_arrival_date: %s\n", hora_chegada);
            fprintf(outputFile, "passengers: %d\n", passageiros);
            fprintf(outputFile, "delay: %d\n", atraso);
        } else {
            fprintf(outputFile, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
                airline, model, origin, destination, hora_partida, 
                hora_chegada, passageiros, atraso);
        }
    }

    if (interativo) {
    } else {
        fclose(outputFile);
    }

//        escreve(formatFlag, 12,interativo, ' ', passageiros, atraso,0, airline, model, origin, destination, hora_partida, hora_chegada, 0, outputFile);
    }

    if (determinarID (argumentos) == 3) {

    User *usuario = findUserById(userHashTable, argumentos);

        if (usuario == NULL) {
            fclose(outputFile);
            return; // Usuário não encontrado
        }

        if(strcasecmp(getUserStatus(usuario), "inactive")==0) {
            fclose(outputFile);
            return;
        }

        const char* id = getUserId(usuario);
        const char* name = getUserName(usuario);
        const char sex = getUserSex(usuario);
        char* data = strdup(getUserData(usuario));
        int age = calcularIdade(data);
        free(data);
        const char* country = getUserCc(usuario);
        const char* passport = getUserPassport(usuario);

        int voos = 0;
        int j=getPassengerCatalogNum(passengerCatalog);

        for (int i=0; i<j; i++) {
            if(strcmp(id,getPassengerId(getPassenger(passengerCatalog,i))) == 0) voos ++;
    }

        int n_reservas = 0;
        double gasto = 0;


    ReservationIterator *iterator = initReservationIterator(reservationHashTable);
    if (iterator == NULL) {
        fclose(outputFile);
        return;
    }

    Reservation *reserva;
    while ((reserva = getNextReservation(iterator)) != NULL) {
        if (strcasecmp(getReservationUserId(reserva), id) == 0) {
            n_reservas++;
            gasto += calcularPrecoTotal(getReservationPrice(reserva), getReservationTax(reserva), getReservationDataIni(reserva), getReservationDataFim(reserva));
        }
    }

    free(iterator);
    
    if (interativo) {
        char output[1024];
        if (formatFlag) {
            sprintf(output,
                "--- 1 ---\n"
                "name: %s\n"
                "sex: %c\n"
                "age: %d\n"
                "country_code: %s\n"
                "passport: %s\n"
                "number_of_flights: %d\n"
                "number_of_reservations: %d\n"
                "total_spent: %.3f\n", // Aqui, cada linha termina com "\", exceto a última
                name, sex, age, country, passport, 
                voos, n_reservas, gasto);
        } else {
            sprintf(output, "%s;%c;%d;%s;%s;%d;%d;%.3f\n", 
                name, sex, age, country, passport, 
                voos, n_reservas, gasto);
        }
        int numLines = formatFlag ? 2 : 1; 
        int startRow = LINES / 2 - numLines / 2; 

       const char* line = strtok(output, "\n");
        while (line != NULL) {
            mvprintw(startRow++, (COLS - strlen(line)) / 2, "%s", line);
            line = strtok(NULL, "\n");
        }
    } else {
        if (formatFlag) {
            fprintf(outputFile, "--- 1 ---\n");
            fprintf(outputFile, "name: %s\n",name);
            fprintf(outputFile, "sex: %c\n",sex);
            fprintf(outputFile, "age: %d\n",age);
            fprintf(outputFile, "country_code: %s\n",country);
            fprintf(outputFile, "passport: %s\n",passport);
            fprintf(outputFile, "number_of_flights: %d\n",voos);
            fprintf(outputFile, "number_of_reservations: %d\n",n_reservas);
            fprintf(outputFile, "total_spent: %.3f\n",gasto);
        } else {
            fprintf(outputFile,"%s;%c;%d;%s;%s;%d;%d;%.3f\n", 
            name, sex, age, country, passport, voos, n_reservas, gasto);
        }
    }



//    escreve (formatFlag, 13, interativo, sex, age, voos, n_reservas, name, country, passport, NULL, NULL, NULL, gasto, outputFile);

    if (interativo) {
    } else {
        fclose(outputFile);
    }

    }    
}