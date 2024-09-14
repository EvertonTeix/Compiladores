package Java;

import java.io.*;
import java.util.*;

public class VizinhoMaisProximo {
    public static void main(String[] args) {
        // Medir o tempo de início
        long startTime = System.currentTimeMillis();
        
        // Medir a memória usada no início
        Runtime runtime = Runtime.getRuntime();
        long startMemory = runtime.totalMemory() - runtime.freeMemory();

        List<Cidade> cidades = lerCidadesDeArquivo("C/cidades.txt");

        if (cidades == null || cidades.isEmpty()) {
            System.out.println("Nenhuma cidade carregada.");
            return;
        }

        encontrarMelhorCaminhoPermutacao(cidades, "Java/viagemJava.txt");
        salvarDistancias(cidades, "Java/distanciasJava.txt");
        
        // Medir o tempo e a memória ao final
        long endTime = System.currentTimeMillis();
        long endMemory = runtime.totalMemory() - runtime.freeMemory();

        // Calcular tempo e memória usados
        long elapsedTime = endTime - startTime;
        long usedMemory = endMemory - startMemory;

        // Exibir resultados
        System.out.println("Tempo de execucao: " + (elapsedTime / 1000.0) + " segundos");
        System.out.println("Memoria usada: " + (usedMemory / 1024.0 / 1024.0) + " MB");
    }

    public static List<Cidade> lerCidadesDeArquivo(String nomeArquivo) {
        List<Cidade> cidades = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(nomeArquivo))) {
            String linha;
            while ((linha = br.readLine()) != null) {
                String[] partes = linha.split("[(),]");
                int x = Integer.parseInt(partes[1].trim());
                int y = Integer.parseInt(partes[2].trim());
                cidades.add(new Cidade(x, y));
            }
        } catch (IOException e) {
            System.out.println("Erro ao ler o arquivo de cidades: " + e.getMessage());
            return null;
        }

        return cidades;
    }

    public static void encontrarMelhorCaminhoPermutacao(List<Cidade> cidades, String arquivoSaida) {
        int numeroCidades = cidades.size();
        double melhorDistancia = Double.MAX_VALUE;
        int[] melhorCaminho = new int[numeroCidades];
        List<int[]> permutacoes = gerarPermutacoes(numeroCidades);

        try (BufferedWriter bw = new BufferedWriter(new FileWriter(arquivoSaida))) {
            for (int[] caminho : permutacoes) {
                double distanciaAtual = calcularDistanciaTotal(cidades, caminho);
                bw.write("Caminho: ");
                for (int i = 0; i < numeroCidades; i++) {
                    bw.write((caminho[i] + 1) + " -> ");
                }
                bw.write((caminho[0] + 1) + " | Distancia total: " + String.format(Locale.US, "%.2f", distanciaAtual) + " km\n");

                if (distanciaAtual < melhorDistancia) {
                    melhorDistancia = distanciaAtual;
                    melhorCaminho = Arrays.copyOf(caminho, caminho.length);
                }
            }

            bw.write("\nMelhor caminho encontrado:\n");
            for (int i = 0; i < numeroCidades; i++) {
                bw.write((melhorCaminho[i] + 1) + " -> ");
            }
            bw.write(melhorCaminho[0] + 1 + "\n");
            bw.write("Distancia total percorrida: " + String.format(Locale.US, "%.2f", melhorDistancia) + " km\n");
        } catch (IOException e) {
            System.out.println("Erro ao escrever o arquivo de viagem: " + e.getMessage());
        }
    }

    private static List<int[]> gerarPermutacoes(int numeroCidades) {
        List<int[]> permutacoes = new ArrayList<>();
        int[] caminho = new int[numeroCidades];
        for (int i = 0; i < numeroCidades; i++) {
            caminho[i] = i;
        }
        permutar(caminho, 0, numeroCidades - 1, permutacoes);
        return permutacoes;
    }

    private static void permutar(int[] caminho, int l, int r, List<int[]> permutacoes) {
        if (l == r) {
            permutacoes.add(caminho.clone());
        } else {
            for (int i = l; i <= r; i++) {
                trocar(caminho, l, i);
                permutar(caminho, l + 1, r, permutacoes);
                trocar(caminho, l, i);
            }
        }
    }

    private static void trocar(int[] caminho, int i, int j) {
        int temp = caminho[i];
        caminho[i] = caminho[j];
        caminho[j] = temp;
    }

    private static double calcularDistanciaTotal(List<Cidade> cidades, int[] caminho) {
        double distanciaTotal = 0.0;
        for (int i = 0; i < caminho.length - 1; i++) {
            distanciaTotal += cidades.get(caminho[i]).calcularDistancia(cidades.get(caminho[i + 1]));
        }
        // Voltar à cidade inicial para fechar o ciclo
        distanciaTotal += cidades.get(caminho[caminho.length - 1]).calcularDistancia(cidades.get(caminho[0]));
        return distanciaTotal;
    }

    public static void salvarDistancias(List<Cidade> cidades, String arquivo) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(arquivo))) {
            bw.write("\nArquivo de distancias no paradigma OO");
            for (int i = 0; i < cidades.size(); i++) {
                bw.write("\nDistancias da Cidade " + (i + 1) + " para todas as outras:\n");
                for (int j = 0; j < cidades.size(); j++) {
                    if (i != j) {
                        double distancia = cidades.get(i).calcularDistancia(cidades.get(j));
                        bw.write("Distancia de Cidade " + (i + 1) + " para Cidade " + (j + 1) + ": " + String.format("%.2f", distancia) + " km\n");
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Erro ao salvar as distancias: " + e.getMessage());
        }
    }
}
