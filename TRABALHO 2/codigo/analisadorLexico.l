%{
#include <stdio.h>
#include <stdlib.h>

int decimal = 0;
int octal = 0;
int hexadecimal = 0;
int real = 0;
int caracteres_invalidos = 0;

%}

digito         [0-9]
decimais       [1-9][0-9]*
octais         0[0-7]*
hexadecimais   0x[0-9A-Fa-f]+
reais          ({digito}*)\.({digito})*

%%

{decimais}      {decimal++;}
{octais}        {octal++;}
{hexadecimais}  {hexadecimal++;}
{reais}         {if((yytext[0] == '.') && (yytext[1] == '\0')){
                    caracteres_invalidos++;
                    }else{
                        real++;
                    }}
[ \t\n]         ;
.               {caracteres_invalidos++;}

%%

int main() {
    yylex();
    
    // Imprimir contagem de ocorrências
    printf("Total de decimais: %d\n", decimal);
    printf("Total de octais: %d\n", octal);
    printf("Total de hexadecimais: %d\n", hexadecimal);
    printf("Total de reais: %d\n", real);
    printf("Total de caracteres inválidos: %d\n", caracteres_invalidos);
    
    return 0;
}


