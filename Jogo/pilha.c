#include "pilha.h"

#include <stdlib.h>


pilha *pilha_aloca(){
    pilha *p = malloc(sizeof(pilha));
    p->comeco = NULL;
}


void pilha_libera(pilha *p){
    free(p);
}


int pilha_vazia(pilha *p){
    if(p->comeco == NULL)
        return 1;
    return 0;
}


conjunto *pilha_topo(pilha *p){
    return p->comeco->c;
}


void pilha_insere(pilha *p, conjunto *c){
    elemento *e = malloc(sizeof(conjunto));

    e->c = c;
    e->prox = p->comeco;
    p->comeco = e;
}


void pilha_retira(pilha *p){
    elemento *e = p->comeco;

    p->comeco = e->prox;

    free(e);
}
