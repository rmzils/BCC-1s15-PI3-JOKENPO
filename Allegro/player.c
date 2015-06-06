#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

player *inicializa_player(){
	player *p = malloc(sizeof(player));

	for(int i = 0; i < MAX_PLAYS; i++)
		p->jogada[i] = -1;

	p->vitoria = 0;

	return p;
}

void adiciona_jogada(player *p, int rodada, int jogada){
	p->jogada[rodada] = jogada;
}

void adiciona_vitoria(player *p){
	p->vitoria++;
}

void libera_player(player *p){
	free(p);
}

int imprime_jogada_jogador(player *p, int rodada)
{
	if (p->jogada[rodada] == 1)
		return 1;
	else if (p->jogada[rodada] == 2)
		return 2;
	else if (p->jogada[rodada] == 3)
		return 3;
	else
		return 4;
}

int imprime_jogada_NPC(player *p, int rodada)
{
	if (p->jogada[rodada] == 1)
		return 1;
	else if (p->jogada[rodada] == 2)
		return 2;
	else if (p->jogada[rodada] == 3)
		return 3;
	else
		return 4;
}