#ifndef PLAYER_H
#define PLAYER_H

#define MAX_PLAYS 120

typedef struct{
	long int jogada[MAX_PLAYS];
	int vitoria;
}player;

player *inicializa_player();
void adiciona_jogada(player *p, int rodada, int jogada);
void adiciona_vitoria(player *p);
void libera_player(player *p);
int imprime_jogada_jogador(player *p, int rodada);
int imprime_jogada_NPC(player *p, int rodada);

#endif