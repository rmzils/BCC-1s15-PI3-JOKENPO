#ifndef IMAGEM_H
#define IMAGEM_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct{
	int matiz, saturacao, iluminacao;
}hsv;

#include "movimento.h"

void copia_quadro(unsigned char ***quadro1, unsigned char ***quadro2, int altura, int largura);

hsv **hsv_aloca(int altura, int largura);

void rgb_hsv(rastreador *rast, unsigned char ***matriz, hsv **h);

void black_white(rastreador *r, parametros *p, hsv **h, unsigned char ***matriz);
void black_white_finger(rastreador *r, parametros *p, hsv **h, unsigned char ***matriz);

int pixel_dedo(parametros *p, hsv *h);
int pixel_mao(parametros *p, hsv *h);

void dilatacao_dedo(rastreador *r, parametros *p, hsv **h, int n, int k);
void dilatacao_mao(rastreador *r, parametros *p, hsv **h, int n, int k);
void erosao_mao(rastreador *r, parametros *p, hsv **h, int n, int k);

void dilatacao_dedo_int(rastreador *r, parametros *p, hsv **h, int k);
void dilatacao_mao_int(rastreador *r, parametros *p, hsv **h, int k);
void erosao_mao_int(rastreador *r, parametros *p, hsv **h, int k);

void blur_simples(rastreador *r, unsigned char ***matriz, int k);

void fritacao(rastreador *r, unsigned char ***matriz, int k);

void hsv_libera(rastreador *r, hsv **h);

#endif