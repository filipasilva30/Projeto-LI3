#include "interativo.h"

/*
Esta função 'modoInterativo()' é responsável por executar o modo interativo de um programa que utiliza a biblioteca ncurses em C para criar uma interface de usuário em um terminal de texto. 
A mesma cria um ambiente interativo em que o usuário pode escolher várias ações para interagir com os dados processados pelo programa, usando uma interface de menu e teclado.
*/
void modoInterativo(char* caminho_data) {
    
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    UserHashTable *userHashTable = processarUsers(caminho_data);
    FlightHashTable *flightHashTable = processarFlights(caminho_data);
    ReservationHashTable *reservationHashTable = processarReservations(caminho_data, userHashTable);
    PassengerCatalog *passengerCatalog = processarPassengers(caminho_data, userHashTable, flightHashTable);

    int opcao;

    while (1) {

        bool formatFlag = false;
        displayMenu();
        refresh();
        opcao = getch(); // Lê a escolha do usuário
        int opcaoNumero = opcao - '0';

        if (opcao >= '1' && opcao <= '9') {
            clear();
            const char *msg_formato = "Pressione a tecla 'F' se quiser que o resultado saia formatado:";
            attron(A_BOLD);    
            mvprintw(LINES / 2, (COLS - strlen(msg_formato)) / 2, "%s", msg_formato);
            attroff(A_BOLD);
            refresh();

            int formatChoice = getch();
            if (formatChoice == 'F' || formatChoice == 'f') formatFlag = true;
            refresh();

        switch (opcaoNumero) {
         case 1:{
                // Fazer o caso de se receber algo inválido, não passar
                clear();
                char arg1[256];
                const char *msg1Exp = "A query1 lista o resumo de um utilizador,voo ou reserva à sua escolha.";
                mvprintw((LINES-10) / 2, (COLS - strlen(msg1Exp)+10) / 2, "%s", msg1Exp);
                const char *msg1 = "Insira o ID do usuário, da reserva, ou do voo que pretende obter informações:";
                mvprintw(LINES / 2, (COLS - strlen(msg1)+10) / 2, "%s", msg1);
                echo();
                getstr(arg1);
                noecho();
                char* arg1Trim = trim(arg1);
                clear();
                mvprintw((LINES - 10) / 2, (COLS-45) / 2, "A entidade associado ao ID '%s' é:",arg1Trim);
                query1(formatFlag, arg1Trim, 0, userHashTable, flightHashTable, passengerCatalog, reservationHashTable, true); 
                refresh();
                mvprintw((LINES + 25) / 2, (COLS-48) / 2, "Pressione qualquer botão para voltar ao menu inicial..");
                refresh();
                break; 
            }
            case 2:{
                clear();
                char arg2[256];
                const char *msg2Exp = "A query2 lista os voos ou as reservas de um utilizador.";
                const char *msg2Exp_2 = "Para tal deve indicar o id desejado seguido de flights/reservations";
                const char* msg2Exp2 = "Caso não seja indicada nenhuma especificação, serão imprimidos tanto reservas como voos.";
                mvprintw((LINES-10) / 2, (COLS - strlen(msg2Exp)+10) / 2, "%s", msg2Exp);
                mvprintw((LINES-9) / 2, (COLS - strlen(msg2Exp_2)+10) / 2, "%s", msg2Exp_2);
                mvprintw((LINES-8) / 2, (COLS - strlen(msg2Exp2)+10) / 2, "%s", msg2Exp2);
                const char *msg2 = "Insira os argumentos para a query2 (userId commandType):";
                mvprintw(LINES / 2, (COLS - strlen(msg2)+10) / 2, "%s", msg2);
                echo();
                getstr(arg2);
                noecho();
                clear();
                mvprintw((LINES - 25) / 2, (COLS-45) / 2, "As informações pedidas para o id selecionado são:");
                query2(formatFlag, arg2, 0, userHashTable, flightHashTable, reservationHashTable, passengerCatalog, true);
                refresh();
                mvprintw((LINES + 25) / 2, (COLS-48) / 2, "Pressione qualquer botão para voltar ao menu inicial..");
                refresh();
                break; 
            }
            case 3: {
                clear();
                char arg3[256];
                const char *msg3Exp = "A query3 calcula a classificação média de um hotel à sua escolha.";
                mvprintw((LINES-10) / 2, (COLS - strlen(msg3Exp)+10) / 2, "%s", msg3Exp);
                const char *msg3 = "Insira o ID do hotel desejado:";
                mvprintw(LINES / 2, (COLS - strlen(msg3))-10 / 2, "%s", msg3);
                echo();
                getstr(arg3);
                noecho();
                char* arg3Trim = trim(arg3);
                clear();
                mvprintw((LINES - 10) / 2, (COLS-45) / 2, "A média do hotel associado ao ID '%s' é:",arg3);
                query3(formatFlag, arg3Trim, 0, reservationHashTable, true);
                refresh();
                mvprintw((LINES + 15) / 2, (COLS-48) / 2, "Pressione qualquer botão para voltar ao menu inicial..");
                refresh();
                break; 
            }
            case 4: { 
                clear();
                char arg4[256];
                mvprintw(0, 0, "Insira o ID do hotel (por exemplo, 'HTL1001'):");
                echo();
                getstr(arg4);
                noecho();
                char* arg4Trim = trim(arg4);
                query4(formatFlag, arg4Trim, 0, reservationHashTable, true);
                waitForEnter();
                break; 
            }
            case 5:{ 
                clear();
                char arg5[256];
                mvprintw(0, 0, "Insira os argumentos para a query5 (name, begin_date, end_date):");
                echo();
                getstr(arg5);
                noecho();
                query5(formatFlag, arg5, 0, flightHashTable, true);
                waitForEnter();
                break; 
            }    
            case 6:{
                clear();
                char arg6[256];
                mvprintw(0, 0, "Insira os argumentos para a query6 (ano,N): ");
                echo();
                getstr(arg6);
                noecho();
                query6(formatFlag, arg6, 0, flightHashTable, passengerCatalog, true);
                waitForEnter();
                break;     
            }            
            case 7: {
                clear();
                char arg7[256];
                const char *msg7Exp = "A query7 lista os 'N' aeroportos com uma maior mediana de atrasos.";
                mvprintw((LINES-10) / 2, (COLS - strlen(msg7Exp)) / 2, "%s", msg7Exp);
                const char *msg7 = "Insira o nº de aeroporto que pretende 'ranquear':";
                mvprintw(LINES / 2, (COLS - strlen(msg7))-70 / 2, "%s", msg7);
                echo();
                getstr(arg7);
                noecho();
                char* arg7Trim = trim(arg7);
                clear();
                mvprintw((LINES - 20) / 2, (COLS-45) / 2, "Os '%s' aeroportos com maior mediana de atrasos são:",arg7);
                query7(formatFlag, arg7Trim, 0, flightHashTable, true);
                refresh();
                mvprintw((LINES + 25) / 2, (COLS-48) / 2, "Pressione qualquer botão para voltar ao menu inicial..");
                refresh();
                break; 
            }
            case 8:{
                clear();
                char arg8[256];
                const char *msg8Exp = "A query8 apresenta a receita de um hotel entre duas datas.";
                const char *msg8Exp_8 = "Para tal deve indicar o id desejado seguido de um intervalo de duas datas.";
                const char* msg8Exp8 = "Exemplo de inserção: 'HTL1001 2010/10/10 2010/10/18.";
                mvprintw((LINES-10) / 2, (COLS - strlen(msg8Exp)+10) / 2, "%s", msg8Exp);
                mvprintw((LINES-9) / 2, (COLS - strlen(msg8Exp_8)+10) / 2, "%s", msg8Exp_8);
                mvprintw((LINES-8) / 2, (COLS - strlen(msg8Exp8)+10) / 2, "%s", msg8Exp8);
                const char *msg8 = "Insira os argumentos para a query8:";
                mvprintw(LINES / 2, (COLS - strlen(msg8)+10) / 2, "%s", msg8);
                echo();
                getstr(arg8);
                noecho();
                clear();
                mvprintw((LINES - 10) / 2, (COLS-60) / 2, "A receita total do hotel entre as duas datas introduzidas é de:");
                query8(formatFlag, arg8, 0, reservationHashTable, true);
                refresh();
                mvprintw((LINES + 25) / 2, (COLS-52) / 2, "Pressione qualquer botão para voltar ao menu inicial..");
                refresh();
                break; 
            }    
            case 9: {
                clear();
                char arg9[256];
                mvprintw(0, 0, "Insira o prefixo a ser procurado:  ");
                echo();
                getstr(arg9);
                noecho();
                query9(formatFlag, arg9, 0, userHashTable, true);
                waitForEnter();
                break; 
            }
    
            mvprintw(LINES - 1, (COLS - strlen("Pressione Enter para retornar ao menu...")) / 2, "Pressione Enter para retornar ao menu...");
            refresh();
            while (getch() != '\n'); 
        }

           waitForEnter(); 
           clear();
        }else if (opcaoNumero == 0) { 
            break;
         } else { 
            mvprintw(LINES - 1, (COLS - strlen("Comando inválido, pressione qualquer tecla para continuar...")) / 2, "Comando inválido, pressione qualquer tecla para continuar...");
            getch(); // Aguarda o usuário pressionar uma tecla
            clear();
         }
         clear();
        
    }         
        
        
    freeUHashTable(userHashTable);
    freeFHashTable(flightHashTable);
    freeRHashTable(reservationHashTable);
    freePassengerCatalog(passengerCatalog);

    endwin();
}