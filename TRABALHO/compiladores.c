#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "compiladores.h"

int lexer(char *lexema){

    if (isalpha(lexema[0])){

        int i = 1;

        while(lexema[i] != 0){
            if(!isalpha(lexema[i]) && lexema[i] != '_'){
                return 8;
            }

            i++;
        }

        return 1;

    }else if (isdigit(lexema[0])){

        int i = 1;
        int quantas_virgulas = 0;
        


        while (lexema[i] != '\0') {
            if (!isdigit(lexema[i]) && lexema[i] != ',') {
                return 8;
            }

            if (lexema[i] == ',') {
                quantas_virgulas++;
                if (quantas_virgulas > 1) {
                    return 8;
                }
            }

            i++;
        }

        if (quantas_virgulas < 1) {
            return 6;
        } else {
            return 7;
        }

    }else if (lexema[0] == '+'){
        return 2;

    }else if (lexema[0] == '-'){
        return 3;

    }else if (lexema[0] == '/'){
        return 4;

    }else if (lexema[0] == '*'){
        return 5;

    }else if(lexema[0] == ','){
        int quantidade_virgulas = 1;
        int i = 1;

        if(lexema[i] == '\0'){
            return 8;
        }

        while (lexema[i] != '\0')
        {
            if (lexema[i] == ',') {
                quantidade_virgulas++;
                if (quantidade_virgulas > 1) {
                    return 8; 
                }

                return 8;
            }

            i++;
        }

        if (quantidade_virgulas <= 1) {
            return 7; 
    }
        
    }
}