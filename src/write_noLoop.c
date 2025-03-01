#include "write_noLoop.h"

/*
void escreve1 (bool formatFlag, int query, bool interativo, char c, int x1, int x2, int x3,const char* s1,const char* s2,const char* s3,const char* s4,const char* s5,const char* s6, double d, FILE* outputFile) {

    if (query==1) {
        
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
                s1, s2, x1, s3, s4, 
                s5, x2, d);
        } else {
            sprintf(output, "%s;%s;%d;%s;%s;%s;%d;%.3f\n", 
                s1, s2, x1, s3, s4, 
                s5, x2, d);
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
            fprintf(outputFile, "hotel_id: %s\n", s1);
            fprintf(outputFile, "hotel_name: %s\n", s2);
            fprintf(outputFile, "hotel_stars: %d\n", x1);
            fprintf(outputFile, "begin_date: %s\n", s3);
            fprintf(outputFile, "end_date: %s\n", s4);
            fprintf(outputFile, "includes_breakfast: %s\n", s5);
            fprintf(outputFile, "nights: %d\n", x2);
            fprintf(outputFile, "total_price: %.3f\n", d);
        } else {
            fprintf(outputFile, "%s;%s;%d;%s;%s;%s;%d;%.3f\n", 
                s1, s2, x1, s3, s4, 
                s5, x2, d);
        }
    }
    }

    if (query==2){

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
                s1, s2, s3, s4, s5, 
                s6, x1, x2);
        } else {
            sprintf(output, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
                s1, s2, s3, s4, s5, 
                s6, x1, x2);
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
            fprintf(outputFile, "airline: %s\n", s1);
            fprintf(outputFile, "plane_model: %s\n", s2);
            fprintf(outputFile, "origin: %s\n", s3);
            fprintf(outputFile, "destination: %s\n", s4);
            fprintf(outputFile, "schedule_departure_date: %s\n", s5);
            fprintf(outputFile, "schedule_arrival_date: %s\n", s6);
            fprintf(outputFile, "passengers: %d\n", x1);
            fprintf(outputFile, "delay: %d\n", x2);
        } else {
            fprintf(outputFile, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
                s1, s2, s3, s4, s5, 
                s6, x1, x2);
        }
    }

    }

    if (query==3){

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
                s1, c, x1, s2, s3, 
                x2, x3, d);
        } else {
            sprintf(output, "%s;%c;%d;%s;%s;%d;%d;%.3f\n", 
                s1, c, x1, s2, s3, 
                x2, x3, d);
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
            fprintf(outputFile, "name: %s\n",s1);
            fprintf(outputFile, "sex: %c\n",c);
            fprintf(outputFile, "age: %d\n",x1);
            fprintf(outputFile, "country_code: %s\n",s2);
            fprintf(outputFile, "passport: %s\n",s3);
            fprintf(outputFile, "number_of_flights: %d\n",x2);
            fprintf(outputFile, "number_of_reservations: %d\n",x3);
            fprintf(outputFile, "total_spent: %.3f\n",d);
        } else {
            fprintf(outputFile,"%s;%c;%d;%s;%s;%d;%d;%.3f\n", 
            s1, c, x1, s2, s3, x2, x3, d);
        }
    }
    }

}


*/
/*
void escreve3 (bool formatFlag, double media, bool interativo, FILE* outputFile) {

    if (interativo) {
        char output[1024];
        if (formatFlag) {
            sprintf(output, "--- 1 ---\nrating: %.3lf", media);
        } else {
            sprintf(output, "%.3lf", media);
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
            fprintf(outputFile, "--- 1 ---\nrating: %.3lf\n", media);
        } else {
            fprintf(outputFile, "%.3lf\n", media);
        }
    }

}




void escreve (bool formatFlag, int query, bool interativo, char c, int x1, int x2, int x3,const char* s1,const char* s2,const char* s3,const char* s4,const char* s5,const char* s6, double d, FILE* outputFile) {


    if (query==11) escreve1(formatFlag, 1, interativo,c, x1, x2, x3, s1, s2, s3, s4, s5, s6, d, outputFile);
    if (query==12) escreve1(formatFlag, 2, interativo,c, x1, x2, x3, s1, s2, s3, s4, s5, s6, d, outputFile);
    if (query==13) escreve1(formatFlag, 3, interativo,c, x1, x2, x3, s1, s2, s3, s4, s5, s6, d, outputFile);

    if (query==3) escreve3(formatFlag, d, interativo, outputFile);


    if (interativo) {
    } else {
        fclose(outputFile);
    }

}

*/