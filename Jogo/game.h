#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "game.h"
#include "calibragem.h"
#include "camera.h"
#include "imagem.h"
#include "movimento.h"
#include "jogada.h"
#include "player.h"

typedef struct{
	player *p, *npc;
	int rodada;
	int resultado[MAX_PLAYS];
	int fase;
	int continuar;
}game;

void game_loop();

#endif