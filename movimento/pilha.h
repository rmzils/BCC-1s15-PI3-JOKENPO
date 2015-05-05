#ifndef PILHA_H
#define PILHA_H


#include "disjoint.h"


typedef struct _elemento{
  conjunto *c;
  struct _elemento *prox;
}elemento;

typedef struct{
  elemento *comeco;
}pilha;


pilha *pilha_aloca();
void pilha_libera(pilha *p);

int pilha_vazia(pilha *p);
conjunto *pilha_topo(pilha *p);

void pilha_insere(pilha *p, conjunto *c);
void pilha_retira(pilha *);


#endif
