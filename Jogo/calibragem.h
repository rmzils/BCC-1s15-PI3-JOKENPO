#ifndef CALIBRAGEM_H
#define CALIBRAGEM_H

#include <stdlib.h>

typedef struct{
	int matiz_dedo_min, matiz_dedo_max, saturacao_dedo;
	int matiz_mao_min, matiz_mao_max, saturacao_mao;
	int massa_min;
	int k;
}parametros;

#include "movimento.h"
#include "camera.h"

#include <math.h>

parametros *aloca_parametros();
//void calibragem(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam);
//void calibra_mao(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam);
//void calibra_dedos(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam);
void libera_parametros(parametros *p);

#endif