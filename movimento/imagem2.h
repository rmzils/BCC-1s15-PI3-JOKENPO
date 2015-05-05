#ifndef IMAGEM_H
#define IMAGEM_H

#include <stdlib.h>
#include <stdio.h>

#include "movimento.h"

void copia_quadro(unsigned char ***quadro1, unsigned char ***quadro2, int altura, int largura);

void black_white_hand(rastreador *r, parametros *p, unsigned char ***matriz);
void black_white_finger(rastreador *r, parametros *p, unsigned char ***matriz);
void rgb_hsv(rastreador *rast, unsigned char ***matriz, int **matiz, int **saturacao);

void abertura(rastreador *r, unsigned char ***matriz, int n);
void fechamento(rastreador *r, unsigned char ***matriz, int n);
void dilatacao(rastreador *r, unsigned char ***matriz, int n);
void erosao(rastreador *r, unsigned char ***matriz, int n);

void dilatacao_int(rastreador *r, unsigned char ***matriz);
void erosao_int(rastreador *r, unsigned char ***matriz);

#endif
