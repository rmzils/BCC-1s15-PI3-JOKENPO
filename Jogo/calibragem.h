#ifndef CALIBRAGEM_H
#define CALIBRAGEM_H

#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef struct{
	int matiz_dedo_min, matiz_dedo_max, saturacao_dedo;
	int matiz_mao_min, matiz_mao_max, saturacao_mao;
	int massa_min;
	int k;
}parametros;

#include "movimento.h"
#include "camera.h"
#include "disjoint.h"
#include "connected.h"

#include <math.h>

parametros *aloca_parametros();
int calibragem(parametros *p, rastreador *r, hsv **h, camera *cam, ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, int altura, int largura);
//void calibra_mao(parametros *p, rastreador *r, hsv **h, camera *cam);
//void calibra_dedos(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam);
void libera_parametros(parametros *p);

#endif