#ifndef GRAFO_H
#define GRAFO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REP_MATRIZ 0
#define REP_LISTA  1

typedef struct AdjNo {
    int vertice;
    struct AdjNo *prox;
} AdjNo;

typedef struct {
    int num_vertices;
    int num_arestas;
    int representacao;
    int **matriz;
    AdjNo **lista;
} Grafo;

typedef struct {
    int *pai;
    int *nivel;
    int *ordem;
    int  n_ordem;
} ResultBusca;

typedef struct {
    int *vertices;
    int  tamanho;
} Componente;
typedef struct {
    Componente *componentes;
    int         num_componentes;
} ResultComponentes;



Grafo *grafo_criar(int num_vertices, int representacao);
void   grafo_destruir(Grafo *g);

Grafo *grafo_ler(const char *arquivo, int representacao);
void   grafo_escrever_info(Grafo *g, const char *arquivo);

void grafo_adicionar_aresta(Grafo *g, int u, int v);
int  grafo_tem_aresta(Grafo *g, int u, int v);

int grau_vertice(Grafo *g, int v);
int grau_minimo(Grafo *g);
int grau_maximo(Grafo *g);
double grau_medio(Grafo *g);
double grau_mediana(Grafo *g);

ResultBusca *bfs(Grafo *g, int origem);
ResultBusca *dfs(Grafo *g, int origem);
void busca_resultado_destruir(ResultBusca *res);
void busca_escrever(Grafo *g, ResultBusca *res, int origem, const char *tipo, const char *arquivo);

int distancia(Grafo *g, int u, int v);
int diametro(Grafo *g);
int diametro_aproximado(Grafo *g);

ResultComponentes *componentes_conexas(Grafo *g);
void componentes_destruir(ResultComponentes *res);
void componentes_escrever(ResultComponentes *res, const char *arquivo);

#endif