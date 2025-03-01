#include "query3.h"

/*
Nesta função 'query3()' temos como objetivo calcular a avaliação média de um hotel com base nas reservas do mesmo.
Recebemos como input o id de um hotel.
Percorremos os dados guardados sobre as reservas relativas ao hotel em questão e vamos somando as avaliações do hotel.
No final, fazemos uma média das avaliações e imprimimos a mesma num ficheiro de texto.
*/


void query3(bool formatFlag, char *args, int commandLineNum,  ReservationHashTable* reservationTable, bool interativo) {

    FILE *outputFile;
    char outputFileName[1024];

    if (interativo) {
        outputFile = stdout; 
    } else {
        sprintf(outputFileName, "Resultados/command%d_output.txt", commandLineNum);
        outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }
    }

    if (args == NULL || reservationTable == NULL) {
        fclose(outputFile);
        return;
    }

    char *hotelId = trim(args);

    double totalRating = 0.0;
    int numRatings = 0;

    ReservationIterator *iterador = initReservationIterator(reservationTable);
    if (iterador == NULL) {
        fclose(outputFile);
        return;
    }   

    Reservation *reserva;

    while ((reserva = getNextReservation(iterador)) != NULL) {
        const char* reservationHotelId = getReservationHotelId(reserva);
        int reservationRate = getReservationRate(reserva);

        if (reservationHotelId != NULL && strcmp(reservationHotelId, hotelId) == 0) {
                totalRating += reservationRate;
                numRatings++;
        }
    }

    free(iterador);

    double averageRating = (numRatings > 0) ? totalRating / numRatings : 0.0;

//    escreve(formatFlag,3, interativo, ' ', 0, 0, 0,NULL,NULL,NULL,NULL,NULL, NULL, averageRating, outputFile);

    if (interativo) {
        char output[1024];
        if (formatFlag) {
            sprintf(output, "--- 1 ---\nrating: %.3lf", averageRating);
        } else {
            sprintf(output, "%.3lf", averageRating);
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
            fprintf(outputFile, "--- 1 ---\nrating: %.3lf\n", averageRating);
        } else {
            fprintf(outputFile, "%.3lf\n", averageRating);
        }
    }

    if (interativo) {
    } else {
        fclose(outputFile);
    }


}