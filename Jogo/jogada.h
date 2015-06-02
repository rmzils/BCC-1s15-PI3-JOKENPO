#ifndef JOGADA_H
#define JOGADA_H

#include <stdlib.h>

#include "imagem.h"
#include "movimento.h"
#include "disjoint.h"
#include "connected.h"

int analiza_jogada(rastreador *r, parametros *p, hsv **h);

#endif