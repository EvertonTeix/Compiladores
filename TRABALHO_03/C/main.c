#include "modulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>       // Para medir uso de memória
#include <psapi.h>         // Para usar GetProcessMemoryInfo

// Função para medir o uso de memória em MB
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter))) {
        double memoryUsedMB = memCounter.WorkingSetSize / (1024.0 * 1024.0);  // Converter para MB
        printf("Uso de memoria: %.2f MB\n", memoryUsedMB);
    } else {
        printf("Erro ao obter informações de memória.\n");
    }
}

int main() {
    // Medir o tempo de início do programa
    clock_t start, end;
    double cpu_time_used;

    // Iniciar contagem do tempo
    start = clock();

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

    // Salvar as distâncias entre as cidades em um arquivo
    salvarDistancias(cidades, numero_total);

    // Exibir o melhor caminho usando o método do vizinho mais próximo
    printf("\nMelhor caminho usando vizinho mais proximo:\n");
    encontrarMelhorCaminhoVizinhoMaisProximo(cidades, numero_total);

    // Medir o tempo de fim do programa
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %f segundos\n", cpu_time_used);

    // Medir o uso de memória em MB
    printMemoryUsage();

    return 0;
}
