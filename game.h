#ifndef GAME_H
#define GAME_H

#include "player.h"

typedef struct{
	player *p, *npc;
	int rodada;
	int resultado[MAX_PLAYS];
}game;

void game_loop();
void game_turn(game *g);
int vitoria_jogador(game *g);
void adiciona_resultado(game *g, int vitoria);
void adiciona_rodada(game *g);

#endif