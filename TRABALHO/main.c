#include <stdio.h>
#include <stdlib.h>
#include "compiladores.h"

int main(){

    FILE *file;
    char palavra[10000];

    file = fopen("compiladores.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    while(fscanf(file, "%s", palavra) == 1){

        int lexema = lexer(palavra);

        switch (lexema) {
            case 1:
                printf("TOKEN: [ID] | LEXEMA: %s\n", palavra);
                break;
            case 2:
                printf("TOKEN: [OPERADOR DE SOMA] | LEXEMA: %s\n", palavra);
                break;
            case 3:
                printf("TOKEN: [OPERADOR DE SUBTRACAO] | LEXEMA: %s\n", palavra);
                break;
            case 4:
                printf("TOKEN: [OPERADOR DE DIVISAO] | LEXEMA: %s\n", palavra);
                break;
            case 5:
                printf("TOKEN: [OPERADOR DE MULTIPLICACAO] | LEXEMA: %s\n", palavra);
                break;
            case 6:
                printf("TOKEN: [INTEIRO] | LEXEMA: %s\n", palavra);
                break;
            case 7:
                printf("TOKEN: [FLOAT] | LEXEMA: %s\n", palavra);
                break;
            default:
                printf("TOKEN: [ERRO] | LEXEMA: %s\n", palavra);
                break;
        }

    }
    
    fclose(file);

    return 0;
}