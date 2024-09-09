#include "modulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));
    int numero_total, minimo, maximo;

    // Ajustar o intervalo correto para o número de cidades (entre 5 e 10)
    do {
        printf("\nDigite o numero total de cidades que voce deseja entre 5 e 10: ");
        scanf("%d", &numero_total);

        if (numero_total < 5 || numero_total > 10) {
            printf("Erro: O numero total de cidades deve estar entre 5 e 10.\n");
        }
    } while (numero_total < 5 || numero_total > 10);

    // Obter valores mínimos e máximos para as coordenadas
    do {
        printf("Digite o valor minimo para as coordenadas: ");
        scanf("%d", &minimo);

        printf("Digite o valor maximo para as coordenadas: ");
        scanf("%d", &maximo);

        if (minimo >= maximo) {
            printf("Erro: O valor minimo deve ser menor que o valor máximo.\n");
        }
    } while (minimo >= maximo);

    // Gerar as cidades com base nas entradas do usuário
    Cidade cidades[numero_total];
    gerarCidade(cidades, numero_total, minimo, maximo);

    // Exibir as cidades geradas
    imprimeCidades(cidades, numero_total);

    // Exibir o melhor caminho usando o método do vizinho mais próximo
    printf("\nMelhor caminho usando vizinho mais proximo:\n");
    encontrarMelhorCaminhoVizinhoMaisProximo(cidades, numero_total);

    // Exibir todos os caminhos e distâncias
    printf("\nTodos os caminhos e distâncias:\n");
    mostrarTodosCaminhos(cidades, numero_total);

    return 0;
}
