#include "erros_flights.h"

// A função'isValidAirportCode' irá verificar se uma string é composta por exatamente três caracteres.
bool isValidAirportCode(const char *str) { 
    if (strlen(str) != 3){
        return false;
        
    }
    return true;
    
}

/*

Esta função chamada 'isDepartureBeforeArrival', é usada para comparar duas datas e horas, uma de partida (departure) e outra de chegada (arrival).
Faz isto para verificar se a data e hora de partida acontecem antes da data e hora de chegada.
*/
bool isDepartureBeforeArrival(const char *departure, const char *arrival) {
    int depYear, depMonth, depDay, depHours, depMinutes, depSeconds;
    int arrYear, arrMonth, arrDay, arrHours, arrMinutes, arrSeconds;

    if (sscanf(departure, "%d/%d/%d %d:%d:%d", &depYear, &depMonth, &depDay, &depHours, &depMinutes, &depSeconds) != 6 ||
        sscanf(arrival, "%d/%d/%d %d:%d:%d", &arrYear, &arrMonth, &arrDay, &arrHours, &arrMinutes, &arrSeconds) != 6) {
        return false;
    }

    // Comparar ano, mês e dia primeiro
    if (depYear > arrYear ||
        (depYear == arrYear && depMonth > arrMonth) ||
        (depYear == arrYear && depMonth == arrMonth && depDay > arrDay)) {
        return false;
    }

    // Se a data for a mesma, compara a hora
    if (depYear == arrYear && depMonth == arrMonth && depDay == arrDay) {
        if (depHours > arrHours || 
            (depHours == arrHours && depMinutes > arrMinutes) || 
            (depHours == arrHours && depMinutes == arrMinutes && depSeconds >= arrSeconds)) {
            return false;
        }
    }

    return true;
}