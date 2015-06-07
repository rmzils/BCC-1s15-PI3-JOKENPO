#ifndef GAME_H
#define GAME_H

#define PI 3.14159

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
}game;

void game_turn(ALLEGRO_DISPLAY *janela, const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_FONT *fonte, 
	ALLEGRO_BITMAP *fundo, ALLEGRO_TIMER *timer_jogo, ALLEGRO_EVENT_QUEUE *fila_eventos);

int ganhade(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
	ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *ganha1, ALLEGRO_BITMAP *ganha2, ALLEGRO_BITMAP *perde1, 
	ALLEGRO_BITMAP *perde2, ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte);

int game_tutorial(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
	ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *tutorial, ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *tesoura,
	ALLEGRO_BITMAP *spock, ALLEGRO_BITMAP *papel, ALLEGRO_BITMAP *lagarto, ALLEGRO_BITMAP *pedra, ALLEGRO_FONT *fonte);

void erro(char *mensagem);

#endif