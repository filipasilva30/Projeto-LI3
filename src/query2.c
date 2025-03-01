#include "query2.h"


/*
A estrutura 'CombinedInfo()' armazena informações combinadas de voos ou reservas, incluindo um id (identificador do voo ou reserva), uma date (data no formato AAAA/MM/DD), e um type que especifica se é um "flight" ou "reservation".
*/
struct CombinedInfo {
    char id[20]; // ID do voo ou reserva
    char date[11]; // Data formatada (YYYY/MM/DD)
    char type[12]; // "flight" ou "reservation"
};

/*
A função 'compareDates()' compara duas datas dadas como strings.
Extrai os componentes de ano, mês e dia das datas e compara-os.
Retorna um valor negativo, zero ou positivo, dependendo da comparação das datas.
*/
int compareDates(const char *date1, const char *date2) {
    int year1, month1, day1;
    int year2, month2, day2;

    // Lê apenas as partes de data das strings
    if (sscanf(date1, "%d/%d/%d", &year1, &month1, &day1) != 3 ||
        sscanf(date2, "%d/%d/%d", &year2, &month2, &day2) != 3) {
        return 0; // Se as datas não puderem ser lidas corretamente, retornamos 0
    }

    // Compara ano, mês e dia
    if (year1 != year2) return year1 - year2;
    if (month1 != month2) return month1 - month2;
    return day1 - day2;
}

/*
A função 'compareCombinedInfo()' é utilizada para comparar dois registros de CombinedInfo.
Primeiro compara as datas utilizando a função compareDates.
Se as datas forem iguais, compara os IDs para determinar a ordem.
*/
int compareCombinedInfo(const void *a, const void *b) {
    const struct CombinedInfo *infoA = (const struct CombinedInfo *)a;
    const struct CombinedInfo *infoB = (const struct CombinedInfo *)b;

    // Comparar as datas (YYYY/MM/DD)
    int dateComparison = compareDates(infoA->date, infoB->date);
    if (dateComparison != 0) {
        return -dateComparison; // Inverte para ordenar do mais recente para o mais antigo
    }

    // Em caso de empate nas datas, ordenar por ID
    return strcmp(infoA->id, infoB->id);
}

/*
A função principal 'query2()' prepara inicialmente o arquivo de saída dependendo do modo.
De seguida, processa os argumentos para obter o ID do user e o tipo de comando.
Busca e compila informações de voos e/ou reservas associadas ao usuário em uma lista de 'CombinedInfo'.
Ordena esta lista com qsort usando a função 'compareCombinedInfo'.
Por fim, imprime os resultados formatados na saída escolhida (arquivo ou terminal).
*/
void query2 (bool formatFlag, char *args, int commandLineNum, const UserHashTable *userHashTable,const FlightHashTable *flightHashTable,ReservationHashTable *reservationHashTable, const PassengerCatalog *passengerCatalog, bool interativo) {
    
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

    if (args == NULL || (flightHashTable == NULL && reservationHashTable == NULL)) {
        fprintf(outputFile, "Argumentos inválidos para a query2\n");
        fclose(outputFile);
        return;
    }

    // Dividir args em userId e commandType
    char userId[20], commandType[15] = "";
    int conta = 1;
    int aux = 0;

    sscanf(args, "%s %s", userId, commandType);

    User *user = findUserById(userHashTable, userId);
    if (user == NULL || strcasecmp(getUserStatus(user), "inactive") == 0) {
        fclose(outputFile);
        return; // Usuário não encontrado ou inativo
    }

    if (strcmp(commandType, "flights")==0) aux = 1;
    if (strcmp(commandType, "reservations")==0) aux = 2;

    struct CombinedInfo *combinedList = malloc((getFlightTableCount(flightHashTable) + getReservationTableCount(reservationHashTable)) * sizeof(struct CombinedInfo));
    int combinedCount = 0;

    if (aux == 1 || aux == 0) {
        for (int i = 0; i < getPassengerCatalogNum(passengerCatalog); i++) {
            const Passenger *passenger = getPassenger(passengerCatalog, i);
            if (strcmp(getPassengerId(passenger), userId) == 0) {
                // Encontrar o voo associado no FlightCatalog
                const Flight *flight = findFlightById(flightHashTable, getPassengerFlight(passenger));
                if (flight) {
                    strncpy(combinedList[combinedCount].id, getFlightId(flight), sizeof(combinedList[combinedCount].id));
                    strncpy(combinedList[combinedCount].date, getFlightPartida(flight), 10); // Copia apenas a parte da data
                    combinedList[combinedCount].date[10] = '\0'; // Garantir terminação nula
                    strcpy(combinedList[combinedCount].type, "flight");
                    combinedCount++;
                }
            }
        }
    }

    if (aux == 0 || aux == 2) {

        ReservationIterator *iterador = initReservationIterator(reservationHashTable);
        if (iterador == NULL) {
            return;
        }

        Reservation *reserva;

        while ((reserva = getNextReservation(iterador)) != NULL) {
        
            if (reserva != NULL && strcmp(getReservationUserId(reserva), userId) == 0) {
                
                strncpy(combinedList[combinedCount].id, getReservationId(reserva), sizeof(combinedList[combinedCount].id) - 1);
                combinedList[combinedCount].id[sizeof(combinedList[combinedCount].id) - 1] = '\0'; // Garantir terminação nula

                strncpy(combinedList[combinedCount].date, getReservationDataIni(reserva), 10); // Copia apenas a parte da data
                combinedList[combinedCount].date[10] = '\0'; // Garantir terminação nula

                strcpy(combinedList[combinedCount].type, "reservation");
                combinedCount++;

            }
        }

        free(iterador);
    }           
        qsort(combinedList, combinedCount, sizeof(struct CombinedInfo), compareCombinedInfo);

int offset = 0;

for (int i = 0; i < combinedCount; i++) {
    char lineBuffer[256]; 
    int lineLength;   

    if (interativo) {
        // Limpamos o buffer de linha para cada nova entrada
        memset(lineBuffer, 0, sizeof(lineBuffer));

        if (formatFlag) {
            lineLength = snprintf(lineBuffer, sizeof(lineBuffer), 
                                  "--- %d ---\nid: %s\ndate:%s\n", 
                                  conta, combinedList[i].id, combinedList[i].date);
            if (aux == 0) {
                // Se aux é 0, adicionamos o tipo ao buffer de linha
                lineLength += snprintf(lineBuffer + lineLength, sizeof(lineBuffer) - lineLength, 
                                       "type: %s\n", combinedList[i].type);
            }
        } else {
            if (aux == 0) {
                lineLength = snprintf(lineBuffer, sizeof(lineBuffer), 
                                      "%s;%s;%s\n", combinedList[i].id, combinedList[i].date, combinedList[i].type);
            } else {
                lineLength = snprintf(lineBuffer, sizeof(lineBuffer), 
                                      "%s;%s\n", combinedList[i].id, combinedList[i].date);
            }
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
        if (formatFlag) {
                fprintf(outputFile, "--- %d ---\n", conta);
                fprintf(outputFile, "id: %s\n", combinedList[i].id);
                fprintf(outputFile, "date: %s\n", combinedList[i].date);
                if (aux==0) {
                    // Imprime o tipo apenas se ambos voos e reservas estão sendo listados
                    fprintf(outputFile, "type: %s\n", combinedList[i].type);
                }
                if (i < combinedCount - 1) fprintf(outputFile, "\n");
                conta++;
        } else {
            if (aux == 0) {
                // Imprime o tipo apenas se ambos voos e reservas estão sendo listados
                fprintf(outputFile, "%s;%s;%s\n", combinedList[i].id, combinedList[i].date, combinedList[i].type);
            } 
        
            if (aux == 1 || aux == 2) {
                fprintf(outputFile, "%s;%s\n", combinedList[i].id, combinedList[i].date);
            }
        }
    }
}
    if (interativo) fclose(outputFile);

    free(combinedList); 
    
}
