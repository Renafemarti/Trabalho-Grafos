# Trabalho 1 - Grafos

Biblioteca em C para manipulação de grafos não-direcionados, desenvolvida para a
disciplina Estrutura de Dados II. Implementa representação por matriz e por
lista de adjacência, além dos algoritmos de busca em largura (BFS), busca em
profundidade (DFS), cálculo de distância, diâmetro (exato e aproximado) e
componentes conexas.

## Requisitos

- Linux (ou WSL)
- `gcc`
- `make`

## Estrutura do projeto

```
.
├── grafo.h            # interface da biblioteca (TAD Grafo)
├── grafo.c            # implementação da biblioteca
├── main.c             # programa interativo que usa a biblioteca
├── Makefile
├── grafo_1.txt         # grafos de exemplo usados no estudo de caso
├── grafo_2.txt
├── grafo_3.txt
├── grafo_4.txt
├── grafo_5.txt
└── grafo_6.txt
```

## Como compilar

Na raiz do projeto, rode:

```bash
make
```

Isso vai gerar o executável `grafo` na mesma pasta.

Para limpar o executável gerado:

```bash
make clean
```

## Como executar

Basta rodar o executável, sem passar nenhum argumento:

```bash
./grafo
```

O programa é interativo e vai perguntar, em sequência:

1. **Qual grafo usar** (1 a 6) — corresponde aos arquivos `grafo_1.txt` até
   `grafo_6.txt`, que devem estar na mesma pasta do executável.
2. **Qual representação usar**:
   - `1` para matriz de adjacência
   - `2` para lista de adjacência
3. **Vértice de origem** para gerar a árvore de busca (BFS/DFS). Se você
   apenas apertar Enter, o padrão é o vértice `1`.

### Exemplo de execução

```
$ ./grafo
Escolha o grafo (1-6): 1
Escolha a representacao (1=matriz, 2=lista): 2
Vertice de origem para a arvore de busca [padrao 1]: 1
```

> **Observação sobre desempenho:** a representação por **matriz** usa
> memória O(V²) e buscas O(V²), então para grafos com muitos vértices ela é
> visivelmente mais lenta e consome muito mais memória do que a
> representação por **lista** (O(V+E)). Isso é esperado e é justamente o
> tipo de comparação pedida no estudo de caso do trabalho.

## Arquivos gerados na execução

Depois de rodar, o programa cria e atualiza estes arquivos na pasta atual:

| Arquivo                    | Conteúdo                                                          |
|-----------------------------|--------------------------------------------------------------------|
| `saida_info.txt`            | Nº de vértices/arestas, grau mín/máx/médio/mediana, componentes conexas e diâmetro |
| `saida_bfs.txt`              | Árvore de busca (pai e nível de cada vértice) gerada pela BFS       |
| `saida_dfs.txt`              | Árvore de busca (pai e nível de cada vértice) gerada pela DFS       |
| `saida_componentes.txt`      | Lista das componentes conexas, em ordem decrescente de tamanho     |

Além disso, o próprio terminal mostra durante a execução: memória usada após
carregar o grafo, tempo médio de 100 execuções de BFS e de DFS, pai dos
vértices 10/20/30 (buscas a partir de 1/2/3), distância entre pares de
vértices e o diâmetro (exato e aproximado).