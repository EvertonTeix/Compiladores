#include <float.h>

typedef struct cidade{
    int x;
    int y;
}Cidade;

/*Verifica se a cidade já existe*/
int cidadeExiste(Cidade cidades[], int numero_total_cidades, int x, int y);

/*Gera todas as cidades aleatoriamente*/
void gerarCidade(Cidade cidades[], int numero_total_cidades, int min, int max);

/*Calcula a distância entre duas cidades*/
double calcularDistancia(Cidade cidades[], int cidadeA, int cidadeB);

/*Imrime as cidades*/
void imprimeCidades(Cidade cidades[], int n);



void encontrarMelhorCaminhoVizinhoMaisProximo(Cidade cidades[], int numero_total_cidades);

void mostrarTodosCaminhos(Cidade cidades[], int numero_total_cidades);