#ifndef CONNECTED_H
#define CONNECTED_H

#include <stdlib.h>

#include "disjoint.h"
#include "movimento.h"
#include "pilha.h"

typedef struct{
	conjunto *representante;
}pixel;

void connected_components(rastreador *r, unsigned char ***matriz, disjoint *d);
void une_conjuntos(disjoint *d);
void une_parentes(disjoint *d, conjunto *c);
conjunto *find(pixel *p);
int conta_componentes(disjoint *d);
void consiste_massa(rastreador *r, parametros *p, disjoint *d);
void imprime_parentes(conjunto *c);


#endif