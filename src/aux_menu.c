#include "aux_menu.h"

/*
A função 'printCentered()'recebe uma string como entrada e imprime essa mesma string centralizada na tela. 
Ela calcula a posição vertical e horizontal para centralizar o texto e usa a função 'mvprintw' da biblioteca ncurses para exibir o texto na tela.
*/
void printCentered(const char* str) {
    int len = strlen(str);
    int row = LINES / 2; // Centraliza verticalmente
    int col = (COLS - len) / 2; // Centraliza horizontalmente
    mvprintw(row, col, "%s", str);
}

/*
Esta função ('waitForEnter()') espera até que o usuário pressione a tecla 'Enter' (ou 'Return') para continuar. 
A mesma usa um loop do-while para ler o código de tecla de entrada até que o código correspondente à tecla 'Enter' seja detectado. 
Isto será útil para aguardar a entrada do usuário antes de continuar.
*/
void waitForEnter() {
    refresh(); 

    int c;
    do {
        c = getch();
    } while (c != '\n');

    clear();
    refresh(); 
}

/*
A função 'printInMiddle()' imprime uma string text centralizada em uma largura especificada 'width', com um espaço de preenchimento 'padding' à esquerda e à direita.
A mesma calcula a quantidade de espaços necessários para centralizar o texto e, em seguida, imprime o texto com o preenchimento apropriado.
*/
void printInMiddle(const char* text, int width, int padding) {
    int len = strlen(text);
    int spaces = (width - len) / 2;
    for (int i = 0; i < spaces; i++) printf(" "); // Padding esquerdo
    printf("%*s\n", len + padding, text);
    for (int i = 0; i < spaces; i++) printf(" "); // Padding direito
}


/*
A função 'displayMenu()' é usada para exibir um menu de opções centralizado na tela. 
Ela utiliza a biblioteca ncurses para definir cores e para formatar o texto. 
A função 'mvprintw' é usada para exibir o texto do menu com as opções numeradas.
O utilizador pode escolher uma opção se digitar o número correspondente.
*/
void displayMenu() {
    int row, col;
    getmaxyx(stdscr, row, col); // Obtém as dimensões do terminal

    clear(); // Limpa a tela
    start_color(); // Inicia o modo de cores
    init_pair(1, COLOR_WHITE, COLOR_YELLOW); // Define um par de cores

    attron(COLOR_PAIR(1)); // Ativa o par de cores
    mvprintw(row / 2 - 12, (col - strlen("Menu Interativo")) / 2, "Menu Interativo");
    attroff(COLOR_PAIR(1)); // Desativa o par de cores
    attron(A_BOLD);
    mvprintw(row / 2 - 8, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 - 7, (col - strlen("Premir 1 --> Query 1")) / 2, "Premir 1 --> Query 1");
    mvprintw(row / 2 - 6, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 - 5, (col - strlen("Premir 2 --> Query 2")) / 2, "Premir 2 --> Query 2");
    mvprintw(row / 2 - 4, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 - 3, (col - strlen("Premir 3 --> Query 3")) / 2, "Premir 3 --> Query 3");
    mvprintw(row / 2 - 2, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 - 1, (col - strlen("Premir 4 --> Query 4")) / 2, "Premir 4 --> Query 4");
    mvprintw(row / 2 , (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 + 1, (col - strlen("Premir 5 --> Query 5")) / 2, "Premir 5 --> Query 5");
    mvprintw(row / 2 + 2, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 + 3 , (col - strlen("Premir 6 --> Query 6")) / 2, "Premir 6 --> Query 6");
    mvprintw(row / 2 + 4, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 + 5, (col - strlen("Premir 7 --> Query 7")) / 2, "Premir 7 --> Query 7");
    mvprintw(row / 2 + 6, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 + 7, (col - strlen("Premir 8 --> Query 8")) / 2, "Premir 8 --> Query 8");
    mvprintw(row / 2 + 8, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 + 9, (col - strlen("Premir 9 --> Query 9")) / 2, "Premir 9 --> Query 9");
    mvprintw(row / 2 + 10, (col - strlen("+----------------------+")) / 2, "+----------------------+");
    mvprintw(row / 2 + 12, (col - strlen("Premir '0' --> Sair")) / 2, "Premir '0' --> Sair");
    // Repetir para outros comandos
    attroff(A_BOLD);

    attron(COLOR_PAIR(1)); // Ativa o par de cores
    mvprintw(row/2 + 16, (col - strlen("Escolha um comando: ")) / 2, "Escolha um comando: ");
    attroff(COLOR_PAIR(1)); // Desativa o par de cores

    refresh(); 
}
