#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "grafo.h"

static int ler_opcao(const char *prompt, int min, int max) {
    char linha[64];
    int valor;
    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(linha, sizeof(linha), stdin)) { printf("\nEntrada encerrada.\n"); exit(1); }
        if (sscanf(linha, "%d", &valor) == 1 && valor >= min && valor <= max) {
            return valor;
        }
        printf("Opcao invalida, tente novamente.\n");
    }
}

int main(void) {


    printf("Escolha o grafo:\n");
    printf("  1) grafo_1.txt\n");
    printf("  2) grafo_2.txt\n");
    printf("  3) grafo_3.txt\n");
    printf("  4) grafo_4.txt\n");
    printf("  5) grafo_5.txt\n");
    printf("  6) grafo_6.txt\n");
    int escolha_grafo = ler_opcao("Digite o numero do grafo (1-6): ", 1, 6);

    char nome_arquivo[32];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "grafo_%d.txt", escolha_grafo);

    printf("\nEscolha a representacao:\n");
    printf("  1) Matriz de adjacencia\n");
    printf("  2) Lista de adjacencia\n");
    int escolha_rep = ler_opcao("Digite a opcao (1-2): ", 1, 2);
    int rep = (escolha_rep == 1) ? REP_MATRIZ : REP_LISTA;

    printf("\nVertice de origem para a arvore de busca (BFS/DFS) [padrao 1]: ");
    fflush(stdout);
    char linha_origem[64];
    int origem_usuario = 1;
    if (fgets(linha_origem, sizeof(linha_origem), stdin)) {
        int tmp;
        if (sscanf(linha_origem, "%d", &tmp) == 1) origem_usuario = tmp;
    }

    printf("\nCarregando %s (representacao: %s)...\n\n",
           nome_arquivo, rep == REP_MATRIZ ? "matriz" : "lista");

    Grafo *g = grafo_ler(nome_arquivo, rep);
    if (!g) {
        printf("Erro ao ler o arquivo '%s'. Verifique se ele existe nesta pasta.\n", nome_arquivo);
        return 1;
    }

    printf("=== %s (representacao: %s) ===\n", nome_arquivo, rep == REP_MATRIZ ? "matriz" : "lista");
    printf("Vertices: %d | Arestas: %d\n\n", g->num_vertices, g->num_arestas);

    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    double mem_mb = uso.ru_maxrss / 1024.0;
    printf("--- Item 1: memoria usada apos carregar o grafo ---\n");
    printf("  Memoria residente (RSS): %.2f MB\n\n", mem_mb);

    grafo_escrever_info(g, "saida_info.txt");
    printf("Info geral escrita em saida_info.txt\n\n");

    int n = g->num_vertices;

    if (origem_usuario >= 1 && origem_usuario <= n) {
        ResultBusca *rb_arv = bfs(g, origem_usuario);
        ResultBusca *rd_arv = dfs(g, origem_usuario);
        busca_escrever(g, rb_arv, origem_usuario, "BFS", "saida_bfs.txt");
        busca_escrever(g, rd_arv, origem_usuario, "DFS", "saida_dfs.txt");
        busca_resultado_destruir(rb_arv);
        busca_resultado_destruir(rd_arv);
        printf("Arvore de busca (pai/nivel) a partir do vertice %d escrita em saida_bfs.txt e saida_dfs.txt\n\n",
               origem_usuario);
    } else {
        printf("Vertice de origem %d invalido (grafo tem %d vertices); pulando geracao da arvore de busca.\n\n",
               origem_usuario, n);
    }

    printf("--- Item 4: pai de 10, 20, 30 (BFS e DFS a partir de 1, 2, 3) ---\n");
    int origens[3] = {1, 2, 3};
    int alvos[3] = {10, 20, 30};

    for (int i = 0; i < 3; i++) {
        int o = origens[i];
        if (o > n) continue;

        ResultBusca *rb = bfs(g, o);
        ResultBusca *rd = dfs(g, o);

        printf("Origem %d:\n", o);
        for (int j = 0; j < 3; j++) {
            int v = alvos[j];
            if (v > n) continue;
            printf("  vertice %2d -> pai(BFS)=%-6d pai(DFS)=%-6d\n",
                   v, rb->pai[v], rd->pai[v]);
        }

        busca_resultado_destruir(rb);
        busca_resultado_destruir(rd);
    }
    printf("\n");

    printf("--- Item 5: distancias entre pares ---\n");
    int pares[3][2] = {{10, 20}, {10, 30}, {20, 30}};
    for (int i = 0; i < 3; i++) {
        int u = pares[i][0], v = pares[i][1];
        if (u > n || v > n) continue;
        int d = distancia(g, u, v);
        printf("  distancia(%d, %d) = %d\n", u, v, d);
    }
    printf("\n");

    int k = 100;
    if (k > n) k = n;

    printf("--- Itens 2 e 3: tempo medio de %d BFS e %d DFS ---\n", k, k);

    clock_t ini, fim;
    double soma_bfs = 0.0, soma_dfs = 0.0;

    for (int i = 0; i < k; i++) {
        int origem = 1 + (i * (n / k));
        if (origem > n) origem = n;

        ini = clock();
        ResultBusca *rb = bfs(g, origem);
        fim = clock();
        soma_bfs += (double)(fim - ini) / CLOCKS_PER_SEC;
        busca_resultado_destruir(rb);

        ini = clock();
        ResultBusca *rd = dfs(g, origem);
        fim = clock();
        soma_dfs += (double)(fim - ini) / CLOCKS_PER_SEC;
        busca_resultado_destruir(rd);
    }

    printf("  Tempo medio BFS: %.6f s\n", soma_bfs / k);
    printf("  Tempo medio DFS: %.6f s\n\n", soma_dfs / k);

    printf("--- Item 6: componentes conexas ---\n");
    ResultComponentes *rc = componentes_conexas(g);
    printf("  Numero de componentes: %d\n", rc->num_componentes);
    printf("  Maior componente: %d vertices\n", rc->componentes[0].tamanho);
    printf("  Menor componente: %d vertices\n", rc->componentes[rc->num_componentes - 1].tamanho);
    componentes_escrever(rc, "saida_componentes.txt");
    componentes_destruir(rc);
    printf("\n");

    printf("--- Item 7: diametro ---\n");

    ini = clock();
    int diam_aprox = diametro_aproximado(g);
    fim = clock();
    printf("  Diametro aproximado: %d (calculado em %.6f s)\n",
           diam_aprox, (double)(fim - ini) / CLOCKS_PER_SEC);

    if (n <= 5000) {
        ini = clock();
        int diam = diametro(g);
        fim = clock();
        printf("  Diametro exato: %d (calculado em %.6f s)\n",
               diam, (double)(fim - ini) / CLOCKS_PER_SEC);
    } else {
        printf("  Diametro exato NAO calculado (grafo grande demais, %d vertices)\n", n);
    }

    grafo_destruir(g);
    printf("\nConcluido.\n");
    return 0;
}