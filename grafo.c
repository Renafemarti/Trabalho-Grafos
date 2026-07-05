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
    fprintf(f, "Vertices:        %d\n", g->num_vertices);
    fprintf(f, "Arestas:         %d\n", g->num_arestas);
    fprintf(f, "Grau minimo:     %d\n", grau_minimo(g));
    fprintf(f, "Grau maximo:     %d\n", grau_maximo(g));
    fprintf(f, "Grau medio:      %.4f\n", grau_medio(g));
    fprintf(f, "Mediana de grau: %.1f\n", grau_mediana(g));

    ResultComponentes *rc = componentes_conexas(g);
    fprintf(f, "Componentes conexas: %d\n", rc->num_componentes);
    for (int i = 0; i < rc->num_componentes; i++)
        fprintf(f, "  Componente %d: %d vertices\n", i + 1, rc->componentes[i].tamanho);
    componentes_destruir(rc);

    fclose(f);
}

// Função auxiliar para calcular quantos vizinhos um vértice possui, dependendo da representação do grafo
// Serve também para não repetir código if matriz else lista, DRY
int grau_vertice(Grafo *g, int v) {
    if(g->representacao == REP_MATRIZ) {
        int grau = 0;
        for(int i = 1; i <= g->num_vertices; i++) {
            if(g->matriz[v][i]) grau++;
        }
        return grau;
    }
    int grau = 0;
    for(AdjNo *n = g->lista[v]; n; n = n->prox) grau++;
    return grau;
}

// Funação que busca o grau mínimo de todos os vértices do grafo, utilizando a função grau_vertice
int grau_minimo(Grafo *g) {
    int min = grau_vertice(g, 1);
    for(int v = 2; v <= g->num_vertices; v++) {
        int grau = grau_vertice(g, v);
        if(grau < min) min = grau;
    }
    return min;
}

//Função que busca o grau máximo de todos os vértices do grafo, utilizando a função grau_vertice
int grau_maximo(Grafo *g) {
    int max = grau_vertice(g, 1);
    for(int v = 2; v <= g->num_vertices; v++) {
        int grau = grau_vertice(g, v);
        if(grau > max) max = grau;
    }
    return max;
}

// Função que calcula a soma de todos os graus divida pela qtd de vértices
double grau_medio(Grafo *g) {
    long soma = 0;
    for(int v = 1; v <=g->num_vertices; v++) {
        soma += grau_vertice(g, v);
    }
    return (double)soma / g->num_vertices;
}

//Função para auxiliar o qsort a ordenar os graus dos vértices, para calcular a mediana
static int cmp_int(const void *a, const void *b) {
    return (*(const int *)a) - (*(const int *)b);
}

//Função que usa o qsort para ordenar os graus dos vértices e calcular a mediana, utilizando a função grau_vertice
double grau_mediana(Grafo *g) {
    int n = g->num_vertices;
    int *graus = (int *)malloc(n * sizeof(int));
    for(int v = 1; v <= n; v++) {
        graus[v-1] = grau_vertice(g, v);
    }
    qsort(graus, n, sizeof(int), cmp_int);

    double mediana;
    if(n % 2 == 1) {
        mediana = graus[n / 2];
    } else {
        mediana = (graus[n/2 - 1] + graus[n/2]) / 2.0;
    }
    free(graus);
    return mediana;
}

// Função que realiza a busca em largura (BFS) em um grafo, retornando um ponteiro para uma estrutura ResultBusca contendo informações sobre a busca
ResultBusca *bfs(Grafo *g, int origem) {
    int n = g->num_vertices;

    ResultBusca *res = (ResultBusca *)malloc(sizeof(ResultBusca));
    res->pai = (int *)malloc((n + 1) * sizeof(int));
    res->nivel = (int *)malloc((n + 1) * sizeof(int));  
    res->ordem = (int *)malloc((n + 1) * sizeof(int));
    res->n_ordem = 0;

    for(int i = 0; i <=n; i++) {
        res->pai[i] = -1;
        res->nivel[i] = -1;
    }

    int *fila = (int *)malloc((n + 1) * sizeof(int));
    int inicio = 0, fim = 0;

    res->pai[origem] = 0;
    res->nivel[origem] = 0;
    fila[fim++] = origem;
    res->ordem[res->n_ordem++] = origem;

    while(inicio < fim) {
        int u = fila[inicio++];

        if(g->representacao == REP_MATRIZ) {
            for(int v = 1; v <=n; v++) {
                if(g->matriz[u][v] && res->nivel[v] == -1) {
                    res->pai[v] = u;
                    res->nivel[v] = res->nivel[u] + 1;
                    fila[fim++] = v;
                    res->ordem[res->n_ordem++] = v;
                }
            }
        } else {
        for(AdjNo *no = g->lista[u]; no; no = no->prox) {
                int v = no->vertice;
                if(res->nivel[v] == -1) {
                    res->pai[v] = u;
                    res->nivel[v] = res->nivel[u] + 1;
                    fila[fim++] = v;
                    res->ordem[res->n_ordem++] = v;
                }
            }
        }
    }
    free(fila);
    return res;
}

//Função que realiza a busca em profundidade (DFS) em um grafo, retornando um ponteiro para uma estrutura ResultBusca contendo informações sobre a busca
ResultBusca *dfs(Grafo *g, int origem) {
    int n = g->num_vertices;

    ResultBusca *res = (ResultBusca *)malloc(sizeof(ResultBusca));
    res->pai = (int *)malloc((n + 1) * sizeof(int));
    res->nivel = (int *)malloc((n + 1) * sizeof(int));
    res->ordem = (int *)malloc((n + 1) * sizeof(int));
    res->n_ordem = 0;

    for (int i = 0; i <= n; i++) {
        res->pai[i] = -1;
        res->nivel[i] = -1;
    }

    int *pilha = (int *)malloc((n + 1) * sizeof(int));
    int topo = 0;

    res->pai[origem] = 0;
    res->nivel[origem] = 0;
    pilha[topo++] = origem;
    res->ordem[res->n_ordem++] = origem;

    while (topo > 0) {
        int u = pilha[--topo];

        if (g->representacao == REP_MATRIZ) {
            for (int v = 1; v <= n; v++) {
                if (g->matriz[u][v] && res->nivel[v] == -1) {
                    res->pai[v] = u;
                    res->nivel[v] = res->nivel[u] + 1;
                    pilha[topo++] = v;
                    res->ordem[res->n_ordem++] = v;
                }
            }
        } else {
            for (AdjNo *no = g->lista[u]; no; no = no->prox) {
                int v = no->vertice;
                if (res->nivel[v] == -1) {
                    res->pai[v] = u;
                    res->nivel[v] = res->nivel[u] + 1;
                    pilha[topo++] = v;
                    res->ordem[res->n_ordem++] = v;
                }
            }
        }
    }
    free(pilha);
    return res;
}
 
//funcao pra liberar a memoria da bfs e dfs
//usar na distancia, diamentro, diametro_aprox e resultcomponentes
void busca_resultado_destruir(ResultBusca *res) {
    if (!res) return;
    free(res->pai);
    free(res->nivel);
    free(res->ordem);
    free(res);
}
 
//usar no main
//funcao pra liberar a memoria do componentes_conexo
void componentes_destruir(ResultComponentes *res) {
    if (!res) return;
    for (int i = 0; i < res->num_componentes; i++)
        free(res->componentes[i].vertices);
    free(res->componentes);
    free(res);
}
//funcao pra ordenar do os componentes em orden crescente
static int cmp_componente_desc(const void *a, const void *b) {
    const Componente *ca = (const Componente *)a;
    const Componente *cb = (const Componente *)b;
    return cb->tamanho - ca->tamanho;
}


//funcao que usa a bfs pra os conexos apartir de cada vertice que nao foi visitado e ordem crescente
ResultComponentes *componentes_conexas(Grafo *g) {
    int n = g->num_vertices;
    int *visitado = (int *)calloc(n + 1, sizeof(int));
 
    Componente *comps = (Componente *)malloc(n * sizeof(Componente));
    int num_comps = 0;
 
    for (int v = 1; v <= n; v++) {
        if (visitado[v]) continue;
 
        ResultBusca *res = bfs(g, v);
        int tamanho = res->n_ordem;
        int *vertices = (int *)malloc(tamanho * sizeof(int));
        for (int i = 0; i < tamanho; i++) {
            vertices[i] = res->ordem[i];
            visitado[res->ordem[i]] = 1;
        }
        comps[num_comps].vertices = vertices;
        comps[num_comps].tamanho = tamanho;
        num_comps++;
 
        busca_resultado_destruir(res);
    }
 
    qsort(comps, num_comps, sizeof(Componente), cmp_componente_desc);
 
    ResultComponentes *rc = (ResultComponentes *)malloc(sizeof(ResultComponentes));
    rc->componentes = comps;
    rc->num_componentes = num_comps;
 
    free(visitado);
    return rc;
}