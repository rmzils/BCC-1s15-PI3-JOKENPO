#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

game *inicializa_game(){
	game *g = malloc(sizeof(game));

	g->p = inicializa_player();
	g->npc = inicializa_player();
	g->rodada = 0;
}

void game_loop(){
	game *g = inicializa_game();
	srand(time(NULL));

	//carrega o mapa;

	while(g->rodada < 10){
		//atualiza mapa;
		game_turn(g);
	}

	if(g->p->vitoria > g->npc->vitoria)
		printf("Você venceu!\n");
	else if(g->p->vitoria == g->npc->vitoria)
		printf("Empatou!\n");
	else
		printf("Voce perdeu!\n");
}

void game_turn(game *g){
	int escolha;

	printf("Rodada: %d\n\n", g->rodada + 1);

	printf("Jogador = %d | d = NPC\n\n", g->p->vitoria, g->npc->vitoria);

	printf("Escolha uma jogada:\n 1 - Pedra;\n 2 - Papel;\n 3 - Tesoura.\n\n");
	scanf("%d", &escolha);

	adiciona_jogada(g->p, g->rodada, escolha);
	adiciona_jogada(g->npc, g->rodada, (rand() % 3) + 1);

	adiciona_resultado(g, vitoria_jogador(g));

	if(g->resultado[g->rodada] == 1)
		adiciona_vitoria(g->p);
	else if(g->resultado[g->rodada] == 0)
		adiciona_vitoria(g->npc);

	printf("\nSua jogada = ");
	imprime_jogada(g->p, g->rodada);
	printf("Jogada do npc = ");
	imprime_jogada(g->npc, g->rodada);
	printf("\n");

	adiciona_rodada(g);

	printf("-----------------------------------------------------\n");
}

int vitoria_jogador(game *g){
	if(g->p->jogada[g->rodada] == 1){
		if(g->npc->jogada[g->rodada] == 3)
			return 1;
		else if (g->npc->jogada[g->rodada] == 2)
			return 0;
		else
			return 2;
	}

	if(g->p->jogada[g->rodada] == 2){
		if(g->npc->jogada[g->rodada] == 1)
			return 1;
		else if (g->npc->jogada[g->rodada] == 3)
			return 0;
		else
			return 2;
	}

	if(g->p->jogada[g->rodada] == 3){
		if(g->npc->jogada[g->rodada] == 2)
			return 1;
		else if (g->npc->jogada[g->rodada] == 1)
			return 0;
		else
			return 2;
	}
}

void adiciona_resultado(game *g, int vitoria){
	g->resultado[g->rodada] = vitoria;
}

void adiciona_rodada(game *g){
	g->rodada++;
}