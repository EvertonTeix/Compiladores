import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
from tkinter import ttk

def ler_cidades(arquivo_cidades):
    cidades = []
    with open(arquivo_cidades, 'r') as f:
        for linha in f:
            if 'Cidade' in linha:
                partes = linha.split('-')
                if len(partes) >= 2:
                    coord_str = partes[1].strip()[1:-1]  # Remove '(' e ')'
                    try:
                        x, y = map(float, coord_str.split(','))  # Converte de string para float
                        cidades.append((x, y))
                        print(f"Coordenadas extraídas: {coord_str}")  # Exibe as coordenadas no console
                    except ValueError as e:
                        print(f"Erro ao converter coordenadas: {coord_str} - {e}")
    return cidades

def ler_caminhos(arquivo_viagem):
    caminhos = []
    melhor_caminho = None
    with open(arquivo_viagem, 'r') as f:
        linhas = f.readlines()
    
    for i, linha in enumerate(linhas):
        if 'Caminho:' in linha:
            partes = linha.split('|')
            if len(partes) >= 2:
                caminho = partes[0].replace('Caminho:', '').strip()
                distancia = partes[1].replace('Distancia total:', '').replace('km', '').strip()
                try:
                    distancia = float(distancia)
                    caminhos.append((caminho, distancia))
                except ValueError:
                    print(f"Erro ao converter distância: {distancia}")
        elif 'Melhor caminho encontrado C:' in linha:
            if i + 1 < len(linhas):
                melhor_caminho = linhas[i + 1].strip().replace('Caminho:', '').strip()
    return caminhos, melhor_caminho

import matplotlib.pyplot as plt

import matplotlib.pyplot as plt

def plotar_cidades_e_caminho(cidades, melhor_caminho):
    fig, ax1 = plt.subplots(figsize=(12, 8))  # Aumentar o tamanho do gráfico
    
    # Plotar as cidades
    if cidades:
        xs, ys = zip(*cidades)
        ax1.scatter(xs, ys, color='purple', label='Cidades', zorder=5)  # Mudança de cor para roxo
        
        if melhor_caminho:
            try:
                # Separar o caminho da distância
                caminho_e_distancia = melhor_caminho.split(" | ")
                caminho_partes = caminho_e_distancia[0].replace(' -> ', ' ').strip().split()
                
                caminho_indices = list(map(int, caminho_partes))  # Converter o caminho para uma lista de inteiros
                caminho_indices.append(caminho_indices[0])  # Fechar o ciclo

                if any(idx > len(cidades) or idx < 1 for idx in caminho_indices):
                    print("Erro: Um ou mais índices estão fora do intervalo da lista de cidades.")
                    return
                
                caminho_cidades = [cidades[i-1] for i in caminho_indices]
                cx, cy = zip(*caminho_cidades)
                
                # Desenha setas entre as cidades com nova cor
                for i in range(len(caminho_cidades) - 1):
                    start = caminho_cidades[i]
                    end = caminho_cidades[i + 1]
                    ax1.annotate('', xy=end, xytext=start,
                                arrowprops=dict(facecolor='orange', edgecolor='black', arrowstyle='->', lw=2))
                
                # Marca a cidade inicial (sem texto no gráfico, apenas na legenda)
                start_city = caminho_cidades[0]
                ax1.scatter([start_city[0]], [start_city[1]], color='blue', s=100, zorder=6, label='Início (Cidade {})'.format(caminho_indices[0]))
            
            except ValueError as e:
                print(f"Erro ao processar o melhor caminho: {melhor_caminho} - {e}")
        
        # Exibir números das cidades com nova cor
        for i, (x, y) in enumerate(cidades):
            ax1.text(x, y + 0.5, f'{i+1}', fontsize=12, color='green', ha='right', zorder=6)  # Números em verde
        
        ax1.set_title('Plano 2D das Cidades e Melhor Caminho calculado', fontsize=14)
        ax1.set_xlabel('X')
        ax1.set_ylabel('Y')
        ax1.set_xlim(min(xs) - 5, max(xs) + 5)  # Ajusta o intervalo do eixo X
        ax1.set_ylim(min(ys) - 5, max(ys) + 5)  # Ajusta o intervalo do eixo Y
        ax1.legend()
        ax1.grid(True, which='both', linestyle='--', linewidth=0.5)
    else:
        print("Nenhuma cidade para plotar.")
    
    return fig



def exibir_interface_completa(cidades, melhor_caminho, caminhos):
    root = tk.Tk()
    root.title('Visualização de Cidades e Caminhos')

    # Plotar o gráfico
    fig = plotar_cidades_e_caminho(cidades, melhor_caminho)
    if fig:
        canvas = FigureCanvasTkAgg(fig, master=root)
        canvas_widget = canvas.get_tk_widget()
        canvas_widget.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)
    
    # Adicionar tabela rolável
    tabela_frame = tk.Frame(root)
    tabela_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=1)

    if caminhos:
        df = pd.DataFrame(caminhos, columns=['Caminho', 'Distância'])
        df = df.sort_values(by='Distância')

        # Criação da tabela rolável
        table_frame = tk.Frame(tabela_frame)
        table_frame.pack(fill=tk.BOTH, expand=1)
        
        tree = ttk.Treeview(table_frame, columns=list(df.columns), show='headings')
        for col in df.columns:
            tree.heading(col, text=col)
            tree.column(col, width=150)
        
        for row in df.itertuples(index=False):
            tree.insert('', tk.END, values=row)
        
        tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)

        scrollbar = tk.Scrollbar(table_frame, orient=tk.VERTICAL, command=tree.yview)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        tree.config(yscrollcommand=scrollbar.set)
    
    root.mainloop()

# Arquivos de entrada
arquivo_cidades = 'cidades.txt'
arquivo_viagem = 'viagem.txt'

# Ler dados
cidades = ler_cidades(arquivo_cidades)
caminhos, melhor_caminho = ler_caminhos(arquivo_viagem)

# Exibir a interface gráfica
exibir_interface_completa(cidades, melhor_caminho, caminhos)
