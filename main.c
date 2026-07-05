#include <stdio.h>
#include "grafo.h"

int main(int argc, char **argv) {
    Grafo *g = grafo_ler("grafo_2.txt", REP_LISTA);
    if (!g) {
        printf("Erro ao ler o grafo.\n");
        return 1;
    }

    grafo_escrever_info(g, "saida_info.txt");
    printf("tudo ok\n");

    grafo_destruir(g);
    return 0;
}