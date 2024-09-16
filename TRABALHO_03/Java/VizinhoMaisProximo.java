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

        encontrarMelhorCaminhoVizinhoMaisProximo(cidades, "Java/viagemJava.txt");
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

    public static void encontrarMelhorCaminhoVizinhoMaisProximo(List<Cidade> cidades, String arquivoSaida) {
        int numeroCidades = cidades.size();
        double melhorDistancia = Double.MAX_VALUE;
        int[] melhorCaminho = new int[numeroCidades];

        try (BufferedWriter bw = new BufferedWriter(new FileWriter(arquivoSaida))) {
            for (int inicio = 0; inicio < numeroCidades; inicio++) {
                boolean[] visitado = new boolean[numeroCidades];
                int[] caminho = new int[numeroCidades];
                double distanciaTotal = 0.0;
                
                int cidadeAtual = inicio;
                visitado[cidadeAtual] = true;
                caminho[0] = cidadeAtual;
                
                for (int i = 1; i < numeroCidades; i++) {
                    int proximaCidade = -1;
                    double menorDistancia = Double.MAX_VALUE;
                    
                    for (int j = 0; j < numeroCidades; j++) {
                        if (!visitado[j]) {
                            double distancia = cidades.get(cidadeAtual).calcularDistancia(cidades.get(j));
                            if (distancia < menorDistancia) {
                                menorDistancia = distancia;
                                proximaCidade = j;
                            }
                        }
                    }
                    
                    cidadeAtual = proximaCidade;
                    visitado[cidadeAtual] = true;
                    caminho[i] = cidadeAtual;
                    distanciaTotal += menorDistancia;
                }
                
                distanciaTotal += cidades.get(cidadeAtual).calcularDistancia(cidades.get(caminho[0]));
                
                bw.write("Caminho: ");
                for (int i = 0; i < numeroCidades; i++) {
                    bw.write((caminho[i] + 1) + " -> ");
                }
                bw.write((caminho[0] + 1) + " | Distancia total: " + String.format(Locale.US, "%.2f", distanciaTotal) + " km\n");
                
                if (distanciaTotal < melhorDistancia) {
                    melhorDistancia = distanciaTotal;
                    melhorCaminho = Arrays.copyOf(caminho, caminho.length);
                }
            }

            bw.write("\nMelhor caminho encontrado JAVA:\n");
            bw.write("Caminho: ");
            for (int i = 0; i < numeroCidades; i++) {
                bw.write((melhorCaminho[i] + 1) + " -> ");
            }
            bw.write(melhorCaminho[0] + 1 + "\n");
            bw.write("Distancia total percorrida: " + String.format(Locale.US, "%.2f", melhorDistancia) + " km\n");
        } catch (IOException e) {
            System.out.println("Erro ao escrever o arquivo de viagem: " + e.getMessage());
        }
    }

    public static void salvarDistancias(List<Cidade> cidades, String arquivo) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(arquivo))) {
            for (int i = 0; i < cidades.size(); i++) {
                bw.write("Distâncias da Cidade " + (i + 1) + " para todas as outras:\n");
                for (int j = 0; j < cidades.size(); j++) {
                    if (i != j) {
                        double distancia = cidades.get(i).calcularDistancia(cidades.get(j));
                        bw.write("Distância entre Cidade " + (i + 1) + " e Cidade " + (j + 1) + ": " + String.format("%.2f", distancia) + " km\n");
                    }
                }
                bw.write("\n"); // Linha em branco após as distâncias de cada cidade
            }
        } catch (IOException e) {
            System.out.println("Erro ao salvar as distâncias: " + e.getMessage());
        }
    }
}
