#include "modulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

FILE* abrirArquivoCidades() {
    FILE* arquivo = fopen("cidades.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de cidades para gravação.\n");
        exit(1);
    }
    return arquivo;
}

FILE* abrirArquivoViagem() {
    FILE* arquivo = fopen("viagem.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de viagem para gravação.\n");
        exit(1);
    }
    return arquivo;
}

void fecharArquivo(FILE* arquivo) {
    fclose(arquivo);
}

int cidadeExiste(Cidade cidades[], int numero_total_cidades, int x, int y) {
    for (int i = 0; i < numero_total_cidades; i++) {
        if (cidades[i].x == x && cidades[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int gerarNumeroAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void gerarCidade(Cidade cidades[], int numero_total_cidades, int min, int max) {
    int x, y;
    for (int i = 0; i < numero_total_cidades; i++) {
        do {
            x = gerarNumeroAleatorio(min, max);
            y = gerarNumeroAleatorio(min, max);
        } while (cidadeExiste(cidades, numero_total_cidades, x, y));
        cidades[i].x = x;
        cidades[i].y = y;
    }
}

double calcularDistancia(Cidade cidades[], int cidadeA, int cidadeB) {
    int x = cidades[cidadeA].x - cidades[cidadeB].x;
    int y = cidades[cidadeA].y - cidades[cidadeB].y;
    return sqrt((x * x) + (y * y));
}

void imprimeCidades(Cidade cidades[], int n) {
    FILE* arquivo = abrirArquivoCidades();
    for (int i = 0; i < n; i++) {
        fprintf(arquivo, "Cidade %d - (%d,%d)\n", i + 1, cidades[i].x, cidades[i].y);
    }
    fecharArquivo(arquivo);
}

double calcularDistanciaTotal(Cidade cidades[], int caminho[], int numero_total_cidades) {
    double distancia_total = 0.0;
    for (int i = 0; i < numero_total_cidades - 1; i++) {
        distancia_total += calcularDistancia(cidades, caminho[i], caminho[i + 1]);
    }
    distancia_total += calcularDistancia(cidades, caminho[numero_total_cidades - 1], caminho[0]);
    return distancia_total;
}

// Função auxiliar para trocar dois elementos de um array
void trocar(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para gerar permutações e calcular a Distancia de cada caminho
void permutarECalcular(Cidade cidades[], int caminho[], int l, int r, int numero_total_cidades, double* melhorDistancia, int melhorCaminho[], FILE* arquivo) {
    if (l == r) {
        double distanciaAtual = calcularDistanciaTotal(cidades, caminho, numero_total_cidades);
        fprintf(arquivo, "Caminho: ");
        for (int i = 0; i < numero_total_cidades; i++) {
            fprintf(arquivo, "%d -> ", caminho[i] + 1);
        }
        fprintf(arquivo, "%d | Distancia total: %.2f km\n", caminho[0] + 1, distanciaAtual);

        if (distanciaAtual < *melhorDistancia) {
            *melhorDistancia = distanciaAtual;
            for (int i = 0; i < numero_total_cidades; i++) {
                melhorCaminho[i] = caminho[i];
            }
        }
    } else {
        for (int i = l; i <= r; i++) {
            trocar(&caminho[l], &caminho[i]);
            permutarECalcular(cidades, caminho, l + 1, r, numero_total_cidades, melhorDistancia, melhorCaminho, arquivo);
            trocar(&caminho[l], &caminho[i]); 
        }
    }
}

void mostrarDistanciasEntreCidades(Cidade cidades[], int numero_total_cidades, FILE* arquivo) {
    // Para cada cidade, exibir as distâncias para todas as outras
    for (int i = 0; i < numero_total_cidades; i++) {
        int cidade_atual = i;
        fprintf(arquivo, "\nDistâncias da Cidade %d para todas as outras:\n", cidade_atual + 1);

        for (int j = 0; j < numero_total_cidades; j++) {
            if (i != j) {  // Evita calcular a distância de uma cidade para ela mesma
                double distancia = calcularDistancia(cidades, cidade_atual, j);
                fprintf(arquivo, "Distância de Cidade %d para Cidade %d: %.2f km\n", 
                        cidade_atual + 1, j + 1, distancia);
            }
        }
    }
}

void salvarDistancias(Cidade cidades[], int numero_total_cidades) {
    FILE* arquivo = fopen("distancias.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de distâncias para gravação.\n");
        exit(1);
    }

    mostrarDistanciasEntreCidades(cidades, numero_total_cidades, arquivo);

    fclose(arquivo);
}

void mostrarTodosCaminhos(Cidade cidades[], int numero_total_cidades) {
    FILE* arquivo = abrirArquivoViagem();
    int caminho[numero_total_cidades];
    for (int i = 0; i < numero_total_cidades; i++) {
        caminho[i] = i;
    }

    double melhorDistancia = DBL_MAX;
    int melhorCaminho[numero_total_cidades];

    fprintf(arquivo, "\nTodos os caminhos e suas distâncias:\n");

    // Gerar todas as permutações e calcular a Distancia
    permutarECalcular(cidades, caminho, 0, numero_total_cidades - 1, numero_total_cidades, &melhorDistancia, melhorCaminho, arquivo);

    // Salvar o melhor caminho no arquivo
    fprintf(arquivo, "\nMelhor caminho encontrado:\n");
    for (int i = 0; i < numero_total_cidades; i++) {
        fprintf(arquivo, "%d -> ", melhorCaminho[i] + 1);
    }
    fprintf(arquivo, "%d\n", melhorCaminho[0] + 1);
    fprintf(arquivo, "Distancia total: %.2f km\n", melhorDistancia);

    fecharArquivo(arquivo);
}

void encontrarMelhorCaminhoVizinhoMaisProximo(Cidade cidades[], int numero_total_cidades) {
    FILE* arquivo = abrirArquivoViagem();
    int visitado[numero_total_cidades];
    int caminho[numero_total_cidades];
    double distancia_total = 0.0;

    // Inicializar todas as cidades como não visitadas
    for (int i = 0; i < numero_total_cidades; i++) {
        visitado[i] = 0;
    }

    // Começar da primeira cidade (cidade 0)
    int cidade_atual = 0;
    visitado[cidade_atual] = 1;
    caminho[0] = cidade_atual;

    fprintf(arquivo, "Cidade inicial: %d\n\n", cidade_atual + 1);

    // Iterar sobre todas as cidades
    for (int i = 1; i < numero_total_cidades; i++) {
        int proxima_cidade = -1;
        double menor_distancia = DBL_MAX;

        // Encontrar a cidade não visitada mais próxima
        for (int j = 0; j < numero_total_cidades; j++) {
            if (!visitado[j]) {
                double distancia = calcularDistancia(cidades, cidade_atual, j);
                if (distancia < menor_distancia) {
                    menor_distancia = distancia;
                    proxima_cidade = j;
                }
            }
        }

        // Ir para a próxima cidade
        cidade_atual = proxima_cidade;
        visitado[cidade_atual] = 1;
        caminho[i] = cidade_atual;
        distancia_total += menor_distancia;

        fprintf(arquivo, "Visita: Cidade %d -> Cidade %d (Distancia: %.2f km)\n", caminho[i - 1] + 1, cidade_atual + 1, menor_distancia);
    }

    // Voltar à cidade inicial para fechar o ciclo
    double distancia_retornar = calcularDistancia(cidades, cidade_atual, caminho[0]);
    distancia_total += distancia_retornar;
    fprintf(arquivo, "Retorno: Cidade %d -> Cidade %d (Distancia: %.2f km)\n", cidade_atual + 1, caminho[0] + 1, distancia_retornar);

    // Exibir o caminho total percorrido
    fprintf(arquivo, "\nCaminho encontrado (vizinho mais próximo):\n");
    for (int i = 0; i < numero_total_cidades; i++) {
        fprintf(arquivo, "%d -> ", caminho[i] + 1);
    }
    fprintf(arquivo, "%d\n", caminho[0] + 1);
    fprintf(arquivo, "Distancia total percorrida: %.2f km\n", distancia_total);

    fecharArquivo(arquivo);
}