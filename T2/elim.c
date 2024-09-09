#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elim.h"

int seq_equal(char *ptr, char *seq) {  // Retorna verdadero (1) si encuentra una coincidencia del patron, falso (0) en aso contrario
    while((*ptr == *seq)&&(*ptr!=0)) {
        ptr++;
        seq++;   
    }
    if(*seq!=0) {
        return 0;
    }
    return 1; 
}

void eliminar(char *str, char *pat) {  
    int pat_len = strlen(pat);
    char *j = str;
    if(pat_len==0){ // Caso trivial en el que el patron es string vacío (no se hace nada)
        return;
    }
    while(*j) {
        while((*j==*pat)&&seq_equal(j, pat)) {  // Se chequea primer el digito coincide con el primero del patron, para no llamar innecesariamente a seq_equal
            j+=pat_len;
        }
        *str=*j;
        if(*j!=0) {
            str++;
            j++;
        }
    }
    *str=*j;
}


char *eliminados(char *str, char *pat) { 
    char *str_copy = malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    eliminar(str_copy, pat);
    char *answer = malloc(strlen(str_copy) +1);
    strcpy(answer, str_copy);
    free(str_copy);
    return answer;
}