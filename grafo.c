#include "grafo.h"

Grafo *grafo_criar(int num_vertices, int representacao) {
    Grafo *g = (Grafo *)calloc(1, sizeof(Grafo));
    g->num_vertices  = num_vertices;
    g->num_arestas   = 0;
    g->representacao = representacao;

    if (representacao == REP_MATRIZ) {
        g->matriz = (int **)malloc((num_vertices + 1) * sizeof(int *));
        for (int i = 0; i <= num_vertices; i++)
            g->matriz[i] = (int *)calloc(num_vertices + 1, sizeof(int));
    } else {
        g->lista = (AdjNo **)calloc(num_vertices + 1, sizeof(AdjNo *));
    }
    return g;
}

void grafo_destruir(Grafo *g) {
    if (!g) return;
    if (g->representacao == REP_MATRIZ && g->matriz) {
        for (int i = 0; i <= g->num_vertices; i++) free(g->matriz[i]);
        free(g->matriz);
    } else if (g->lista) {
        for (int i = 1; i <= g->num_vertices; i++) {
            AdjNo *cur = g->lista[i];
            while (cur) { AdjNo *tmp = cur->prox; free(cur); cur = tmp; }
        }
        free(g->lista);
    }
    free(g);
}

void grafo_adicionar_aresta(Grafo *g, int u, int v) {
    if (g->representacao == REP_MATRIZ) {
        if (g->matriz[u][v]) return;
        g->matriz[u][v] = g->matriz[v][u] = 1;
    } else {
        AdjNo *nu = (AdjNo *)malloc(sizeof(AdjNo));
        AdjNo *nv = (AdjNo *)malloc(sizeof(AdjNo));
        nu->vertice = v; nu->prox = g->lista[u]; g->lista[u] = nu;
        nv->vertice = u; nv->prox = g->lista[v]; g->lista[v] = nv;
    }
    g->num_arestas++;
}

int grafo_tem_aresta(Grafo *g, int u, int v) {
    if (g->representacao == REP_MATRIZ)
        return g->matriz[u][v];
    for (AdjNo *n = g->lista[u]; n; n = n->prox)
        if (n->vertice == v) return 1;
    return 0;
}

/* lê grafo: primeira linha = num_vertices, demais = "u v" */
Grafo *grafo_ler(const char *arquivo, int representacao) {
    FILE *f = fopen(arquivo, "r");
    if (!f) { perror("grafo_ler"); return NULL; }

    int n;
    if (fscanf(f, "%d", &n) != 1) { fclose(f); return NULL; }

    Grafo *g = grafo_criar(n, representacao);

    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        if (u < 1 || u > n || v < 1 || v > n) continue;
        grafo_adicionar_aresta(g, u, v);
    }
    fclose(f);
    return g;
}

void grafo_escrever_info(Grafo *g, const char *arquivo) {
    FILE *f = fopen(arquivo, "w");
    if (!f) { perror("grafo_escrever_info"); return; }
    //precisa fazer as funcoes depois
    //fprintf(f, "Vertices:        %d\n", g->num_vertices);
    //fprintf(f, "Arestas:         %d\n", g->num_arestas);
    //fprintf(f, "Grau minimo:     %d\n", grau_minimo(g));
    //fprintf(f, "Grau maximo:     %d\n", grau_maximo(g));
    //fprintf(f, "Grau medio:      %.4f\n", grau_medio(g));
    //fprintf(f, "Mediana de grau: %.1f\n", grau_mediana(g));

    //ResultComponentes *rc = componentes_conexas(g);
    //fprintf(f, "Componentes conexas: %d\n", rc->num_componentes);
    //for (int i = 0; i < rc->num_componentes; i++)
    //    fprintf(f, "  Componente %d: %d vertices\n", i + 1, rc->componentes[i].tamanho);
    //componentes_destruir(rc);

    fclose(f);
}
