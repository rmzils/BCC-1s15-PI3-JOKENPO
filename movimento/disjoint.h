#ifndef DISJOINT_H
#define DISJOINT_H

#include <stdlib.h>
#include <stdio.h>

struct _arco;
struct _conjunto;
struct _disjoint;

typedef struct _arco arco;
typedef struct _conjunto conjunto;
typedef struct _disjoint disjoint;

struct _arco{
	conjunto *w;
	arco *prox;
};

struct _conjunto{
	int massa, num;
	int visitado;
	arco *comeco, *a;
};

struct _disjoint{
	int n, max;
	conjunto **conjuntos;
};

disjoint *aloca_disjoint(int max);
conjunto *aloca_conjunto(int num);
void insere_conjunto(disjoint *d);

int parentesco(conjunto *c, conjunto *s);
void insere_parente(conjunto *c, conjunto *s);

void adiciona_massa(conjunto *c);

void reseta_disjoint(disjoint *d);
void libera_disjoint(disjoint *d);

#endif