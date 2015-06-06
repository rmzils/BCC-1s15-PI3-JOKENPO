#ifndef GAME_H
#define GAME_H

#include "player.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

typedef struct{
	player *p, *npc;
	int rodada;
	int resultado[MAX_PLAYS];
}game;

void game_turn(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_FONT *fonte, 
	ALLEGRO_BITMAP *fundo, ALLEGRO_TIMER *timer_jogo, ALLEGRO_EVENT_QUEUE *fila_eventos);
int game_tutorial(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
	ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *tutorial, ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *tesoura,
	ALLEGRO_BITMAP *spock, ALLEGRO_BITMAP *papel, ALLEGRO_BITMAP *lagarto, ALLEGRO_BITMAP *pedra, ALLEGRO_FONT *fonte);
int ganhade(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
	ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *ganha1, ALLEGRO_BITMAP *ganha2, ALLEGRO_BITMAP *perde1, 
	ALLEGRO_BITMAP *perde2, ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte);
int vitoria_jogador(game *g);
void adiciona_resultado(game *g, int vitoria);
void adiciona_rodada(game *g);

#endif