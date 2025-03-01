#include "query4.h"


/*
A função 'compareReservations()' é uma função auxiliar para comparar as datas de ínicio das reservas.
Em caso de empate usa os IDs das reservas para desempate.
As datas são ordenadas em ordem decrescente.
*/
int compareReservations(const void *a, const void *b) {
    const Reservation *resA = *(const Reservation **)a;
    const Reservation *resB = *(const Reservation **)b;

    int dateCompare = strcmp(getReservationDataIni(resA), getReservationDataIni(resB));
    if (dateCompare == 0) {
        // Usar strcmp para comparar os IDs corretamente
        return strcmp(getReservationId(resA), getReservationId(resB));
    }
    return -dateCompare; // Negativo para ordem decrescente
}

/*
A função 'query4()'tem como objetivo listar as reservas de um hotel, ordenadas por data de início (da mais recente para a mais
antiga).  
A mesma é usada para filtrar, ordenar e exibir informações detalhadas sobre as reservas de um hotel específico.
A função gerencia dinamicamente a memória para armazenar as reservas relevantes e oferece opções flexíveis para a formatação e exibição da saída.
*/
void query4(bool formatFlag, char *args, int commandLineNum, ReservationHashTable *reservationHashTable, bool interativo) {
    
    //Verifica se os argumentos fornecidos (args) ou a tabela hash das reservas (reservationHashTable) são nulos.
    if (args == NULL || reservationHashTable == NULL) {
        printf("Argumentos inválidos para a query4\n");
        return;
    }

    int conta = 1;
    char* hotelId = trim(args);

    int n_reservas = getReservationTableCount(reservationHashTable);
    int numFiltradas = 0;
    
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

    Reservation **reservasFiltradas = malloc(n_reservas* sizeof(Reservation*));
    if (reservasFiltradas == NULL) {
        return;
    }

    ReservationIterator *iterador = initReservationIterator(reservationHashTable);
    if (iterador == NULL) {
        return;
    }   

    Reservation *reserva;

    while ((reserva = getNextReservation(iterador)) != NULL) {
        if (strcasecmp(getReservationHotelId(reserva), hotelId) == 0) {
            if (numFiltradas == n_reservas) {
                n_reservas *= 2;
                Reservation **temp = realloc(reservasFiltradas, n_reservas * sizeof(Reservation*));
                if (temp == NULL) {
                    // Tratamento do erro, a realocação falhou
                    free(reservasFiltradas);
                    return;
                }
                reservasFiltradas = temp;
            }
            reservasFiltradas[numFiltradas++] = reserva;    
        }
    }

    free(iterador);

    qsort(reservasFiltradas, numFiltradas, sizeof(Reservation*), compareReservations);

    for (int i = 0; i < numFiltradas; i++) {
        
        //Aloca memória para armazenar ponteiros para as reservas filtradas, usa um iterador para percorrer todas as reservas na tabela hash.
        //Filtra as reservas que correspondem ao ID do hotel fornecido, armazenando-as em 'reservasFiltradas'.
        Reservation *reservation = reservasFiltradas[i];
        double totalPrice = calcularPrecoTotal(getReservationPrice(reservation),
                                              getReservationTax(reservation),
                                              getReservationDataIni(reservation),
                                              getReservationDataFim(reservation));

    if (formatFlag){

        fprintf(outputFile, "--- %d ---\n", conta);
        fprintf(outputFile, "id: %s\n", getReservationId(reservation));
        fprintf(outputFile, "begin_date: %s\n", getReservationDataIni(reservation));
        fprintf(outputFile, "end_date: %s\n", getReservationDataFim(reservation));
        fprintf(outputFile, "user_id: %s\n", getReservationUserId(reservation));
        fprintf(outputFile, "rating: %d\n", getReservationRate(reservation));
        fprintf(outputFile, "total_price: %.3lf\n", totalPrice);

        if(i<numFiltradas-1) fprintf(outputFile,"\n");

        conta++;

    }

    else {

       fprintf(outputFile, "%s;%s;%s;%s;%d;%.3lf\n",
                getReservationId(reservation),
                getReservationDataIni(reservation),
                getReservationDataFim(reservation),
                getReservationUserId(reservation),
                getReservationRate(reservation),
                totalPrice);
        }

        
    }

    if (interativo) {
    } else {
        fclose(outputFile);
    }
    
    free(reservasFiltradas);
}