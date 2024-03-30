#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "compiladores.h"

int lexer(char *lexema)
{
    if (isalpha(lexema[0])){
        return 1;

    }else if (isdigit(lexema[0])){
    int i = 1;
    int quantas_virgulas = 0;

    while (lexema[i] != '\0') {
        if (!isdigit(lexema[i]) && lexema[i] != ',') {
            return 8; // Caractere inválido
        }

        if (lexema[i] == ',') {
            quantas_virgulas++;
            if (quantas_virgulas > 1) {
                return 8; // Mais de uma vírgula encontrada
            }
        }

        i++;
    }

    if (quantas_virgulas <= 1) {
        return 7; // Número float válido
    } else {
        return 6; // Número inteiro válido
    }

    }else if (lexema[0] == '+'){
        return 2;

    }else if (lexema[0] == '-'){
        return 3;

    }else if (lexema[0] == '/'){
        return 4;

    }else if (lexema[0] == '*'){
        return 5;

    }
}