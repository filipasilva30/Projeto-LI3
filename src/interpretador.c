#include "interpretador.h"

/*
A função 'interpretador' atua como um interpretador de comandos que lê um arquivo de entrada, analisa os comandos e chama funções correspondentes para executar ações específicas com base nos comandos encontrados.
Ela é usada para processar uma série de operações em um sistema que envolvem as hashtables, os catálogos e as outras estruturas de dados referidas.
*/



int interpretador(char *caminho_input, UserHashTable *userHashTable, FlightHashTable *flightHashTable, ReservationHashTable *reservationHashTable, PassengerCatalog *passengerCatalog) {
    FILE *file = fopen(caminho_input, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de comandos");
        return 0;
    }

    char line[1024];
    int commandLineNum = 1;

    while (fgets(line, sizeof(line), file)) {
        bool formatFlag = false;
        char *token;
        char *rest = line;

        // Obtém o ID da query
        token = strtok_r(rest, " ", &rest);
        if (!token) {
            printf("Linha de comando vazia ou inválida\n");
            continue;
        }

        char queryId = token[0];

        // Verifica se há um 'F' após o ID da query
        if (strlen(token) > 1 && token[1] == 'F') {
            formatFlag = true;
        }

        switch (queryId) {
            case '1':
                query1(formatFlag, rest, commandLineNum, userHashTable, flightHashTable, passengerCatalog, reservationHashTable, false);
                break;   
            case '2':
                query2 (formatFlag, rest, commandLineNum, userHashTable, flightHashTable, reservationHashTable, passengerCatalog, false);
                break;         
            case '3':
                query3(formatFlag, rest, commandLineNum, reservationHashTable, false);
                break;
            case '4':
                query4(formatFlag, rest, commandLineNum, reservationHashTable, false);
                break;
            case '5':
                query5(formatFlag,rest, commandLineNum, flightHashTable, false);
                break;
   //         case '6':
   //             query6(formatFlag,rest, commandLineNum, flightHashTable, passengerCatalog, false);
   //             break;
            case '7':
                query7(formatFlag, rest, commandLineNum, flightHashTable, false);
                break;
   //         case '8':
   //             query8(formatFlag,rest, commandLineNum, reservationHashTable, false);
                break;
            case '9':
                query9(formatFlag, rest, commandLineNum, userHashTable, false);
                break;
            default:
                printf("Query ID não reconhecido: %c\n", queryId);
        }
        commandLineNum++;
    }

    fclose(file);
    return commandLineNum;
}

