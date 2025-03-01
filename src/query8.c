#include "query8.h"

/*
Esta função tem como objetivo comparar duas datas no formato AAAA/MM/DD.
A mesma desmembra as datas em componentes de ano, mês e dia e as compara sequencialmente.
*/
int compareDates2(const char *date1, const char *date2) {
    int year1, month1, day1;
    int year2, month2, day2;

    // Lê apenas as partes de data das strings
    if (sscanf(date1, "%d/%d/%d", &year1, &month1, &day1) != 3 ||
        sscanf(date2, "%d/%d/%d", &year2, &month2, &day2) != 3) {
        return 0; // Se as datas não puderem ser lidas corretamente, retornamos 0
    }

    // Compara ano, mês e dia sequencialmente
    if (year1 < year2) return -1;
    if (year1 > year2) return 1;
    if (month1 < month2) return -1;
    if (month1 > month2) return 1;
    if (day1 < day2) return -1;
    if (day1 > day2) return 1;

    return 0; // As datas são iguais
}

/*
Esta função tem como objetivo determinar qual das duas datas é mais recente.
A mesma compara as datas desmembrando-as em ano, mês e dia, e retorna um valor indicativo da comparação.
*/
int comparar_datas(const char *data1_str, const char *data2_str) {
    int ano1, mes1, dia1;
    int ano2, mes2, dia2;

    sscanf(data1_str, "%d/%d/%d", &ano1, &mes1, &dia1);
    sscanf(data2_str, "%d/%d/%d", &ano2, &mes2, &dia2);

    if (ano1 < ano2 || (ano1 == ano2 && mes1 < mes2) || (ano1 == ano2 && mes1 == mes2 && dia1 < dia2)) {
        return 2; // data2 é mais recente
    } else if (ano1 == ano2 && mes1 == mes2 && dia1 == dia2) {
        return 0; // datas são iguais
    } else {
        return 1; // data1 é mais recente
    }
}

/*
A função 'query8()'tem como objetivo apresentar a receita total de um hotel específico durante um período definido.
Começa por extrair o ID do hotel e as datas de início e fim do args.
Inicializa um iterador para a tabela de reservas.
Itera sobre as reservas, calculando a receita para aquelas que estão no intervalo de datas e pertencem ao hotel especificado.
Soma a receita de todas as reservas relevantes.
Exibe o total da receita, formatado ou não, no terminal ou em um arquivo.
*/
void query8(bool formatFlag, char* args, int commandLineNum, ReservationHashTable *reservationHashTable, bool interativo) {
    char hotelId[30], begin_date[11], end_date[11];
    
    char *token = strtok(args, " ");
    if (token == NULL) {
        printf("Argumentos inválidos para a query8\n");
        return;
    }
    strncpy(hotelId, token, 29);
    hotelId[29] = '\0'; // Garantir a terminação da string

    token = strtok(NULL, " \""); // Lendo a próxima parte (data de início)
    if (token == NULL) {
        printf("Argumentos inválidos para a query8\n");
        return;
    }
    strncpy(begin_date, token, 10);
    begin_date[10] = '\0';

    token = strtok(NULL, "\""); // Lendo a próxima parte (data de fim)
    if (token == NULL) {
        printf("Argumentos inválidos para a query8\n");
        return;
    }
    strncpy(end_date, token, 10);
    end_date[10] = '\0';

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

    int totalRevenue = 0;
    ReservationIterator *iterador = initReservationIterator(reservationHashTable);
    if (iterador == NULL) {
        fprintf(stderr, "Erro ao inicializar iterador de reservas.\n");
        fclose(outputFile);
        return;
    }

    Reservation *reserva;
    while ((reserva = getNextReservation(iterador)) != NULL) {
        if (strcasecmp(getReservationHotelId(reserva), hotelId) == 0) {
            char *dataInicial = strdup(getReservationDataIni(reserva));
            char *dataFinal = strdup(getReservationDataFim(reserva));
            
            // Determinar intervalo efetivo da reserva
            char *dataInicioEfetiva = compareDates2(dataInicial, begin_date) >= 0 ? strdup(dataInicial) : strdup(begin_date);
            char *dataFimEfetiva = compareDates2(dataFinal, end_date) <= 0 ? strdup(dataFinal) : strdup(end_date);

            // Calcular receita se reserva estiver no intervalo
            if (compareDates2(dataInicioEfetiva, dataFimEfetiva) <= 0) {
                int preco = getReservationPrice(reserva);
                int taxa = getReservationTax(reserva);
                double receitaReserva = calcularPrecoTotal(preco, taxa, dataInicioEfetiva, dataFimEfetiva);
                totalRevenue = totalRevenue + (int)receitaReserva;
            }

            free(dataInicial);
            free(dataFinal);
            free(dataInicioEfetiva);
            free(dataFimEfetiva);
        }
    }

    free(iterador);
  
    if (interativo) {
        char output[1024];
        if (formatFlag) {
            sprintf(output, "--- 1 ---\nrevenue: %d", totalRevenue);
        } else {
            sprintf(output, "%d", totalRevenue);
        }
        int numLines = formatFlag ? 2 : 1; 
        int startRow = LINES / 2 - numLines / 2; 

        char* line = strtok(output, "\n");
        while (line != NULL) {
            mvprintw(startRow++, (COLS - strlen(line)) / 2, "%s", line);
            line = strtok(NULL, "\n");
        }
    } else {
        if (formatFlag) {
            fprintf(outputFile, "--- 1 ---\nrevenue: %d\n", totalRevenue);
        } else {
            fprintf(outputFile, "%d\n", totalRevenue);
        }
        fclose(outputFile);
    }

}   