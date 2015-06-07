#ifndef MOVIMENTO_H
#define MOVIMENTO_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct{
	int n,s,w,e;
	int altura, largura;
	int count;
	int delimitador;
	int flag_descendo;
	int altura_max, altura_min;
	int angulo;
}rastreador;

#include "calibragem.h"
#include "imagem.h"

rastreador *aloca_rastreador(int altura, int largura);
void atualiza_delimitador(rastreador *r, int deli);
int detecta_movimento(rastreador *r, unsigned char ***quadro1, unsigned char ***quadro2);
void atualiza_rastreador(rastreador *r, parametros *p, hsv **h);
void atualiza_angulo(rastreador *r);
int contador(rastreador *r);
void libera_rastreador(rastreador *r);

#endif