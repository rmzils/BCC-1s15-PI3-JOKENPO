#ifndef JOGADA_H
#define JOGADA_H

#include <stdlib.h>

#include "calibragem.h"
#include "movimento.h"
#include "disjoint.h"
#include "imagem.h"
#include "connected.h"

int analiza_jogada(rastreador *r, parametros *p, unsigned char ***matriz);

#endif