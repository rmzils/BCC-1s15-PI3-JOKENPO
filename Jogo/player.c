#include <stdio.h>
#include <stdlib.h>

#include "player.h"

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

void imprime_jogada(int jogada){
  if(jogada == 0)
    printf("PEDRA\n");
  else if(jogada == 1)
    printf("PAPEL\n");
  else if(jogada == 2)
    printf("TESOURA\n");
  else if(jogada == 3)
    printf("LAGARTO\n");
  else if(jogada == 4)
    printf("SPOCK\n");
}