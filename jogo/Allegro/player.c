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

void imprime_jogada_jogador(player *p, int rodada, ALLEGRO_FONT *fonte, const int LARGURA_TELA, const int ALTURA_TELA){
	if(p->jogada[rodada] == 1)
		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
				(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Pedra");
	else if(p->jogada[rodada] == 2)
		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
				(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Papel");
	else
		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
				(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Tesoura");
}

void imprime_jogada_NPC(player *p, int rodada, ALLEGRO_FONT *fonte, const int LARGURA_TELA, const int ALTURA_TELA){
	if(p->jogada[rodada] == 1)
		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
				(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Pedra");
	else if(p->jogada[rodada] == 2)
		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
				(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Papel");
	else
		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
				(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Tesoura");
}