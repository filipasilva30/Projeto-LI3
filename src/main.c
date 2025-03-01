#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <time.h>
#include <sys/resource.h>
#include "interativo.h"
#include "interpretador.h"
#include "user_catalogs.h"
#include "reservations_catalog.h"
#include "flight_catalogs.h"
#include "passenger_catalogs.h"
#include "tests.h"


/*
A função 'main()' controla o fluxo do programa com base nos argumentos passados e coordena a execução dos modos interativo e batch.
A mesma processa os dados do dataset e executa os comandos conforme necessário.
*/

int main(int args, char *arg[]) {
    
    if (args == 1) {
        char caminho_data[256];
        printf("Insira o caminho do dataset: ");
        fgets(caminho_data, 256, stdin);
        caminho_data[strcspn(caminho_data, "\n")] = 0;
        modoInterativo(caminho_data);
    }

    else if (args == 3) {
        char* caminho_data = arg[1];
        char* caminho_input = arg[2];

        UserHashTable *userHashTable = processarUsers(caminho_data); //processar users é definida na  user_catalogos
        FlightHashTable *flightHashTable = processarFlights(caminho_data);
        ReservationHashTable *reservationHashTable = processarReservations(caminho_data, userHashTable);
        PassengerCatalog *passengerCatalog = processarPassengers(caminho_data, userHashTable, flightHashTable);

        interpretador(caminho_input,userHashTable, flightHashTable, reservationHashTable, passengerCatalog); // invoca o interpretador para ler as queries e passa-lhe a data

        freeUHashTable(userHashTable);
        freeFHashTable(flightHashTable);
        freeRHashTable(reservationHashTable);
        freePassengerCatalog(passengerCatalog);
        
    }
    else if (args == 4) {

        struct timespec start, end;
        struct rusage usageStart, usageEnd;

        clock_gettime(CLOCK_REALTIME, &start);
        getrusage(RUSAGE_SELF, &usageStart);


        char* caminho_data = arg[1];
        char* caminho_input = arg[2];
        char* pasta_output_esperado = arg[3];
        
        UserHashTable *userHashTable = processarUsers(caminho_data); //processarUsers é definida na  user_catalogos
        FlightHashTable *flightHashTable = processarFlights(caminho_data);
        ReservationHashTable *reservationHashTable = processarReservations(caminho_data, userHashTable);
        PassengerCatalog *passengerCatalog = processarPassengers(caminho_data, userHashTable, flightHashTable);

        int x = interpretador(caminho_input,userHashTable, flightHashTable, reservationHashTable, passengerCatalog);

        freeUHashTable(userHashTable);
        freeFHashTable(flightHashTable);
        freeRHashTable(reservationHashTable);
        freePassengerCatalog(passengerCatalog);

        executeTests(x, arg[3], "Resultados");
        clock_gettime(CLOCK_REALTIME, &end);
        getrusage(RUSAGE_SELF, &usageEnd);

        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        long memoryUsed = usageEnd.ru_maxrss - usageStart.ru_maxrss; // Em kilobytes

        printf("Tempo total de execução (modo batch + teste): %.6f segundos\n", elapsed);
        printf("Uso total de memória: %ld KB\n", memoryUsed);

     }else {
        printf("Não existe nenhum modo com esse nº de argumentos\n");
        return 1;
        }
    return 0;
}