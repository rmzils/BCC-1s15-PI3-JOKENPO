#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
 
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL, *botao_sair = NULL, *botao_tutorial = NULL, *botao_jogar = NULL;
ALLEGRO_FONT *fonte = NULL;


game *inicializa_game(){
	game *g = malloc(sizeof(game));

	g->p = inicializa_player();
	g->npc = inicializa_player();
	g->rodada = 0;
}

bool inicializar()
{
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }
 
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }
 
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }
 
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    	{
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
 
    al_set_window_title(janela, "Utilizando o Teclado");
 
    fonte = al_load_font("comic.ttf", 40, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar \"fonte comic.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }
 
    fundo = al_load_bitmap("bg.jpg");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return false;
    }
 
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return false;
    }
 
    botao_jogar = al_load_bitmap("jogar.png");
    if (!botao_jogar)
    {
        fprintf(stderr, "Falha ao criar botão jogar.\n");
        al_destroy_display(janela);
        return false;
    }
 
    botao_sair = al_load_bitmap("sair.png");
    if (!botao_sair)
    {
        fprintf(stderr, "Falha ao criar botão de saída.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_display(janela);
        return false;
    }

     botao_tutorial = al_load_bitmap("tutorial.jpg");
    if (!botao_tutorial)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_display(janela);
        return false;
    }
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    return true;
}

void game_loop(){
	game *g = inicializa_game();
	//srand(time(NULL));

    // Flag que condicionará nosso looping
    bool sair = false, sair2 = false;

    if (!inicializar())
    {
        return;
    }

    while (!sair)
    {
        // Verificamos se há eventos na fila
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            // Se o evento foi de movimentação do mouse
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.y >= ALTURA_TELA / 3 - 50 - al_get_bitmap_height(botao_jogar) / 2 &&
                    evento.mouse.y <= ALTURA_TELA / 3 - 50 + al_get_bitmap_height(botao_jogar) / 2)
                {
            		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 &&
                    evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 &&
                    evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2)
                {
            		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) + 100 + al_get_bitmap_height(botao_sair) / 2)
                {
            		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else
                	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
            }
            // Ou se o evento foi um clique do mouse
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.y >= ALTURA_TELA / 3 - 50 - al_get_bitmap_height(botao_jogar) / 2 &&
                    evento.mouse.y <= ALTURA_TELA / 3 - 50 + al_get_bitmap_height(botao_jogar) / 2)
                {
                    game_turn(g);
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 &&
                    evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 &&
                    evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2)
                {
                	while(!sair2)
                	{
        				while (!al_is_event_queue_empty(fila_eventos))
        				{
				            ALLEGRO_EVENT evento1;
				            al_wait_for_event(fila_eventos, &evento1);
 
				            // Se o evento foi de movimentação do mouse
				            if (evento1.type == ALLEGRO_EVENT_MOUSE_AXES)
				            {
				                if (evento1.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 &&
				                    evento1.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 &&
				                    evento1.mouse.y >= (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2 &&
				                    evento1.mouse.y <= (ALTURA_TELA / 2) + 100 + al_get_bitmap_height(botao_sair) / 2)
				                {
				            		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
				                }
				                else
				                	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
				            }
				            // Ou se o evento foi um clique do mouse
				            else if (evento1.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            					{
                					

        							if (evento1.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 &&
                    					evento1.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 &&
                    					evento1.mouse.y >= (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2 &&
                    					evento1.mouse.y <= (ALTURA_TELA / 2) + 100 + al_get_bitmap_height(botao_sair) / 2)
                					{
                						sair2 = true;
                					}
        						}                	
                		}

                		al_draw_bitmap(fundo, 0, 0, 0);

                		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) - 50, 
							(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Tutorial");

        				al_draw_bitmap(botao_sair, (LARGURA_TELA / 2) - al_get_bitmap_width(botao_sair) / 2,
                       (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2, 0);

                		al_flip_display();
                	}

                	sair2 = false;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) + 100 + al_get_bitmap_height(botao_sair) / 2)
                {
                	sair = true;
                }
            }
        }

        al_draw_bitmap(fundo, 0, 0, 0);
        
        al_set_target_bitmap(al_get_backbuffer(janela));
        al_draw_bitmap(botao_jogar, LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2,
                       (ALTURA_TELA / 3) - 50 - al_get_bitmap_height(botao_jogar) / 2, 0);
        al_draw_bitmap(botao_tutorial, LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2,
                       ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2, 0);
        al_draw_bitmap(botao_sair, (LARGURA_TELA / 2) - al_get_bitmap_width(botao_sair) / 2,
                       (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2, 0);
 		
        // Atualiza a tela
        al_flip_display();
    }
 
    // Desaloca os recursos utilizados na aplicação
    al_destroy_bitmap(botao_sair);
    al_destroy_bitmap(botao_jogar);
    al_destroy_bitmap(botao_tutorial);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);






	//carrega o mapa;

	//while(g->rodada < 10){
		//atualiza mapa;
		//game_turn(g);
	//}

	//if(g->p->vitoria > g->npc->vitoria)
		//printf("Você venceu!\n");
	//else if(g->p->vitoria == g->npc->vitoria)
		//printf("Empatou!\n");
	//else
		//printf("Voce perdeu!\n");
}//

void game_turn(game *g){

    bool sair = false;
    int tecla = 0;
 
    /*if (!inicializar())
    {
        return;
    }*/
 
    al_draw_bitmap(fundo, 0, 0, 0);

    while (!sair)
    {
        while(!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                	case ALLEGRO_KEY_1:
                    	tecla = 1;
                    	break;
                	case ALLEGRO_KEY_2:
                    	tecla = 2;
                    	break;
                	case ALLEGRO_KEY_3:
                    	tecla = 3;
                    	break;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        }

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 
                	ALLEGRO_ALIGN_LEFT, "Rodada: %d", g->rodada + 1);

		al_draw_textf(fonte,al_map_rgb(255, 255, 255), 620, 10, 
                	ALLEGRO_ALIGN_RIGHT, "Jogador: %d", g->p->vitoria);

		al_draw_textf(fonte, al_map_rgb(255, 255, 255), 620, 50, 
                	ALLEGRO_ALIGN_RIGHT, "NPC: %d", g->npc->vitoria);
 
        if (tecla)
        {
            al_draw_bitmap(fundo, 0, 0, 0);

            
 
            switch (tecla)
            {
            	case 1:
                	adiciona_jogada(g->p, g->rodada, tecla);
					adiciona_jogada(g->npc, g->rodada, (rand() % 3) + 1);
                	break;
            	case 2:
               	 	adiciona_jogada(g->p, g->rodada, tecla);
					adiciona_jogada(g->npc, g->rodada, (rand() % 3) + 1);
                	break;
            	case 3:
                	adiciona_jogada(g->p, g->rodada, tecla);
					adiciona_jogada(g->npc, g->rodada, (rand() % 3) + 1);
                	break;
            }

            adiciona_resultado(g, vitoria_jogador(g));

            if(g->resultado[g->rodada] == 1)
				adiciona_vitoria(g->p);
			else if(g->resultado[g->rodada] == 0)
				adiciona_vitoria(g->npc);

			al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) - 50, 
				(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Sua Jogada = ");
			imprime_jogada_jogador(g->p, g->rodada, fonte, LARGURA_TELA, ALTURA_TELA);
			al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) - 50, 
				(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Jogada do NPC = ");
			imprime_jogada_NPC(g->npc, g->rodada, fonte, LARGURA_TELA, ALTURA_TELA);

			adiciona_rodada(g);

			if(g->rodada == 10)
			{
				if(g->p->vitoria > g->npc->vitoria)
				{
					al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, 
						ALTURA_TELA - 100, ALLEGRO_ALIGN_CENTRE, "Você Ganhou!");
					al_flip_display();
					al_rest(5.0);
					sair = true;
				}
				else if(g->p->vitoria == g->npc->vitoria)
					{
						al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, 
						ALTURA_TELA - 100, ALLEGRO_ALIGN_CENTRE, "Empatou!");
						al_flip_display();
						al_rest(5.0);
						sair = true;
					}
				else
					{
						al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, 
						ALTURA_TELA - 100, ALLEGRO_ALIGN_CENTRE, "Você Perdeu!");
						al_flip_display();
						al_rest(5.0);
						sair = true;
					}
			}

            tecla = 0;
        }
 
        al_flip_display();
    }
 
//---------------------------------------------------------------------------------------------

	//int escolha;

	//printf("Rodada: %d\n\n", g->rodada + 1);

	//printf("Jogador = %d | NPC = %d\n\n", g->p->vitoria, g->npc->vitoria);

	//printf("Escolha uma jogada:\n 1 - Pedra;\n 2 - Papel;\n 3 - Tesoura.\n\n");
	//scanf("%d", &escolha);

	//adiciona_jogada(g->p, g->rodada, escolha);
	//adiciona_jogada(g->npc, g->rodada, (rand() % 3) + 1);

	//adiciona_resultado(g, vitoria_jogador(g));

	//if(g->resultado[g->rodada] == 1)
	//	adiciona_vitoria(g->p);
	//else if(g->resultado[g->rodada] == 0)
	//	adiciona_vitoria(g->npc);

	//printf("\nSua jogada = ");
	//imprime_jogada(g->p, g->rodada);
	//printf("Jogada do npc = ");
	//imprime_jogada(g->npc, g->rodada);
	//printf("\n");

	//adiciona_rodada(g);
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