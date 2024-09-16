#include "modulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

// Funções para manipulação de arquivos
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

FILE* abrirArquivoDistancias() {
    FILE* arquivo = fopen("distancias.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de distâncias para gravação.\n");
        exit(1);
    }
    return arquivo;
}

void fecharArquivo(FILE* arquivo) {
    fclose(arquivo);
}

// Funções auxiliares
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

void imprimeCidades(Cidade cidades[], int numero_total_cidades) {
    FILE* arquivo = abrirArquivoCidades();
    for (int i = 0; i < numero_total_cidades; i++) {
        fprintf(arquivo, "Cidade %d - (%d,%d)\n", i + 1, cidades[i].x, cidades[i].y);
    }
    fecharArquivo(arquivo);
}

void salvarDistancias(Cidade cidades[], int numero_total_cidades) {
    FILE* arquivo = abrirArquivoDistancias();
    fprintf(arquivo, "Distancia das cidades em C\n\n");
    for (int i = 0; i < numero_total_cidades; i++) {
        for (int j = 0; j < numero_total_cidades; j++) {
            if (i != j) {
                double distancia = calcularDistancia(cidades, i, j);
                fprintf(arquivo, "Distância entre Cidade %d e Cidade %d: %.2f km\n", i + 1, j + 1, distancia);
            }
        }
        fprintf(arquivo, "\n");  // Linha em branco para separar distâncias entre cidades
    }
    fecharArquivo(arquivo);
}

// Função para encontrar o melhor caminho a partir de cada cidade
void encontrarMelhorCaminhoVizinhoMaisProximo(Cidade cidades[], int numero_total_cidades) {
    FILE* arquivo = abrirArquivoViagem();
    double melhor_distancia_total = DBL_MAX;
    int melhor_caminho[numero_total_cidades];
    
    // Iterar sobre cada cidade como ponto de partida
    for (int inicio = 0; inicio < numero_total_cidades; inicio++) {
        int visitado[numero_total_cidades];
        int caminho[numero_total_cidades];
        double distancia_total = 0.0;
        
        // Inicializar todas as cidades como não visitadas
        for (int i = 0; i < numero_total_cidades; i++) {
            visitado[i] = 0;
        }
        
        // Começar da cidade atual
        int cidade_atual = inicio;
        visitado[cidade_atual] = 1;
        caminho[0] = cidade_atual;
        
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
        }
        
        // Voltar à cidade inicial para fechar o ciclo
        double distancia_retornar = calcularDistancia(cidades, cidade_atual, caminho[0]);
        distancia_total += distancia_retornar;
        
        // Exibir o caminho total percorrido no formato desejado
        fprintf(arquivo, "Caminho: ");
        for (int i = 0; i < numero_total_cidades; i++) {
            fprintf(arquivo, "%d -> ", caminho[i] + 1);
        }
        fprintf(arquivo, "%d | Distancia total: %.2f km\n", caminho[0] + 1, distancia_total);
        
        // Verificar se este é o melhor caminho encontrado até agora
        if (distancia_total < melhor_distancia_total) {
            melhor_distancia_total = distancia_total;
            for (int i = 0; i < numero_total_cidades; i++) {
                melhor_caminho[i] = caminho[i];
            }
        }
    }
    
    // Escrever o melhor caminho encontrado
    fprintf(arquivo, "\nMelhor caminho encontrado C:\n");
    fprintf(arquivo, "Caminho: ");
    for (int i = 0; i < numero_total_cidades; i++) {
        fprintf(arquivo, "%d -> ", melhor_caminho[i] + 1);
    }
    fprintf(arquivo, "%d | Distancia total: %.2f km\n", melhor_caminho[0] + 1, melhor_distancia_total);
    
    fecharArquivo(arquivo);
}
