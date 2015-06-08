#ifndef CONNECTED_H
#define CONNECTED_H

#include <stdlib.h>

#include "disjoint.h"
#include "imagem.h"
#include "movimento.h"
#include "pilha.h"

typedef struct{
	conjunto *representante;
}pixel;

void connected_components(rastreador *r, parametros *p, hsv **h, disjoint *d);
void connected_components_mao(rastreador *r, parametros *p, hsv **h, disjoint *d);
void une_conjuntos(disjoint *d);
void une_parentes(disjoint *d, conjunto *c);
conjunto *find(pixel *p);
int conta_componentes(disjoint *d);
void consiste_dedo(disjoint *d);

#endif