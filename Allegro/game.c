#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

game *inicializa_game(){
	game *g = malloc(sizeof(game));

	g->p = inicializa_player();
	g->npc = inicializa_player();
	g->rodada = 0;

	return g;
}

void game_turn(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_FONT *fonte, 
	ALLEGRO_BITMAP *fundo, ALLEGRO_TIMER *timer_jogo, ALLEGRO_EVENT_QUEUE *fila_eventos)
{
	game *g = inicializa_game();

    bool sair = false;
    int tecla = 0, 
    	min_jogo = 2,
		seg_jogo = 0,
		seg_rodada = 5,
		resultado_jogador = 0,
		resultado_NPC = 0;

    al_start_timer(timer_jogo);

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
                return;
            }

            if (evento.type == ALLEGRO_EVENT_TIMER)
            {
            	if (evento.timer.source == timer_jogo)
            	{
            		if (min_jogo == 0 && seg_jogo <= 1) 
        			{
        				sair = true;
        			}

            		seg_jogo--;
            		if (seg_jogo <= 0)
            		{
            			min_jogo--;
            			seg_jogo = 59;
            		}
       
            		seg_rodada--;
            		if (seg_rodada == 0)
            		{
            			seg_rodada = 5;
            			adiciona_rodada(g);
            		}
            	}
            }
        }

        if (tecla)
        {
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

            resultado_jogador = imprime_jogada_jogador(g->p, g->rodada);
        	resultado_NPC = imprime_jogada_NPC(g->npc, g->rodada);

            adiciona_resultado(g, vitoria_jogador(g));

            if(g->resultado[g->rodada] == 1)
				adiciona_vitoria(g->p);
			else if(g->resultado[g->rodada] == 0)
				adiciona_vitoria(g->npc);
			
				seg_rodada = 5;
				adiciona_rodada(g);

            	tecla = 0;
        }

        al_draw_bitmap(fundo, 0, 0, 0);

        switch (resultado_jogador)
        {
        	case 1:
        		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
					(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Pedra");
        	case 2:
        		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
					(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Papel");
        	case 3:
        		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
					(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Tesoura");
        	case 4:
        		al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 140, 
				(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "");
        }

		switch (resultado_NPC)
		{
			case 1:
				al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
					(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Pedra");
			case 2:
				al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
					(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Papel");
			case 3:
				al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
					(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Tesoura");
			case 4:
				al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) + 180, 
				(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "");
		}

        al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) - 50, 
			(ALTURA_TELA / 2) - 20, ALLEGRO_ALIGN_CENTRE, "Sua Jogada = ");
        al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2) - 50, 
			(ALTURA_TELA / 2) + 20, ALLEGRO_ALIGN_CENTRE, "Jogada do NPC = ");

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2), 15, 
				ALLEGRO_ALIGN_CENTRE, "%dm %ds", min_jogo, seg_jogo);
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2), 55, 
				ALLEGRO_ALIGN_CENTRE, "%ds", seg_rodada);

        al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 
                	ALLEGRO_ALIGN_LEFT, "Rodada: %d", g->rodada + 1);

		al_draw_textf(fonte,al_map_rgb(255, 255, 255), LARGURA_TELA - 30, 10, 
                	ALLEGRO_ALIGN_RIGHT, "Jogador: %d", g->p->vitoria);

		al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA - 30, 50, 
                	ALLEGRO_ALIGN_RIGHT, "NPC: %d", g->npc->vitoria);
 
        al_flip_display();
    }
}

int game_tutorial(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
	ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *tutorial, ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *tesoura,
	ALLEGRO_BITMAP *spock, ALLEGRO_BITMAP *papel, ALLEGRO_BITMAP *lagarto, ALLEGRO_BITMAP *pedra, ALLEGRO_FONT *fonte)
{
	bool sair = false;
	int flag;

	while(!sair)
    {
    	while (!al_is_event_queue_empty(fila_eventos))
    	{
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
 
			// Se o evento foi de movimentação do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
			{
				   if (evento.mouse.x >= 130 - al_get_bitmap_width(botao_voltar) / 2 &&
				       evento.mouse.x <= 130 + al_get_bitmap_width(botao_voltar) / 2 &&
				       evento.mouse.y >= 50 - al_get_bitmap_height(botao_voltar) / 2 &&
				       evento.mouse.y <= 50 + al_get_bitmap_height(botao_voltar) / 2)
				   {
				   		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
				   }
				   else if (evento.mouse.x >= (LARGURA_TELA / 2) - 130 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) -130 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 400 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 400 + 270)
                	{
            			al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                	}
                	else if (evento.mouse.x >= (LARGURA_TELA / 2) - 450 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) - 450 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 220 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 220 + 270)
                	{
            			al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                	}
                	else if (evento.mouse.x >= (LARGURA_TELA / 2) + 200 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + 200 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 220 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 220 + 270)
                	{
            			al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                	}
                	else if (evento.mouse.x >= (LARGURA_TELA / 2) + 120 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + 120 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) + 95 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) + 95 + 270)
                	{
            			al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                	}
                	else if (evento.mouse.x >= (LARGURA_TELA / 2) - 350 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) - 350 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) + 95 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) + 95 + 270)
                	{
            			al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                	}
				   else
				     	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			}
			// Ou se o evento foi um clique do mouse
			else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
         	{
                if (evento.mouse.x >= 130 - al_get_bitmap_width(botao_voltar) / 2 &&
                    evento.mouse.x <= 130 + al_get_bitmap_width(botao_voltar) / 2 &&
                    evento.mouse.y >= 50 - al_get_bitmap_height(botao_voltar) / 2 &&
                    evento.mouse.y <= 50 + al_get_bitmap_height(botao_voltar) / 2)
                {
                	sair = true;
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - 130 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) -130 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 400 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 400 + 270)
                	{
            			flag = ganhade(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, tesoura, lagarto, papel, spock, janela, fonte);
            			if (flag == -1)
            			{
            				return -1;
            			}
                	}
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - 450 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) - 450 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 220 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 220 + 270)
                	{
            			flag = ganhade(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, pedra, tesoura, papel, lagarto, janela, fonte);
            			if (flag == -1)
            			{
            				return -1;
            			}
                	}
                else if (evento.mouse.x >= (LARGURA_TELA / 2) + 200 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + 200 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 220 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 220 + 270)
                	{
            			flag = ganhade(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, pedra, spock, tesoura, lagarto, janela, fonte);
            			if (flag == -1)
            			{
            				return -1;
            			}
                	}
                else if (evento.mouse.x >= (LARGURA_TELA / 2) + 120 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + 120 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) + 95 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) + 95 + 270)
                	{
            			flag = ganhade(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, papel, lagarto, pedra, spock, janela, fonte);
            			if (flag == -1)
            			{
            				return -1;
            			}
                	}
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - 350 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) - 350 + 220 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) + 95 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) + 95 + 270)
                	{
            			flag = ganhade(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, papel, spock, tesoura, pedra, janela, fonte);
            			if (flag == -1)
            			{
            				return -1;
            			}
                	}
        	}
        	else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                return -1;
            }                	
        }

        al_draw_bitmap(fundo, 0, 0, 0);

        al_draw_bitmap(tutorial, 0, 0, 0);

       	al_draw_bitmap(botao_voltar,130 - al_get_bitmap_width(botao_voltar) / 2,
                50 - al_get_bitmap_height(botao_voltar) / 2, 0);

        al_flip_display();
    }
    return 0;
}

int ganhade(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
	ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *ganha1, ALLEGRO_BITMAP *ganha2, ALLEGRO_BITMAP *perde1, 
	ALLEGRO_BITMAP *perde2, ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte)
{
	bool sair = false;

	while(!sair)
    {
    	while (!al_is_event_queue_empty(fila_eventos))
    	{
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);
 
			// Se o evento foi de movimentação do mouse
			if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
			{
				   if (evento.mouse.x >= 130 - al_get_bitmap_width(botao_voltar) / 2 &&
				       evento.mouse.x <= 130 + al_get_bitmap_width(botao_voltar) / 2 &&
				       evento.mouse.y >= 50 - al_get_bitmap_height(botao_voltar) / 2 &&
				       evento.mouse.y <= 50 + al_get_bitmap_height(botao_voltar) / 2)
				   {
				   		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
				   }
				   else
				     	al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			}
			// Ou se o evento foi um clique do mouse
			else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
         	{
                if (evento.mouse.x >= 130 - al_get_bitmap_width(botao_voltar) / 2 &&
                    evento.mouse.x <= 130 + al_get_bitmap_width(botao_voltar) / 2 &&
                    evento.mouse.y >= 50 - al_get_bitmap_height(botao_voltar) / 2 &&
                    evento.mouse.y <= 50 + al_get_bitmap_height(botao_voltar) / 2)
                {
                	sair = true;
                }
        	} 
        	else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                return -1;
            }               	
        }

        al_draw_bitmap(fundo, 0, 0, 0);

       
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 400, (ALTURA_TELA / 2) - 300, 
               ALLEGRO_ALIGN_CENTRE, "Ganha de:");
        al_draw_bitmap(ganha1, (LARGURA_TELA / 2) - 400,
              (ALTURA_TELA / 2) - 200, 0);
        al_draw_bitmap(ganha2, (LARGURA_TELA / 2) - 400,
              (ALTURA_TELA / 2) + 50, 0); 
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 400, (ALTURA_TELA / 2) - 300, 
               ALLEGRO_ALIGN_CENTRE, "Perde de:");
        al_draw_bitmap(perde1, (LARGURA_TELA / 2) + 400,
              (ALTURA_TELA / 2) - 200, 0);
        al_draw_bitmap(perde2, (LARGURA_TELA / 2) + 400,
              (ALTURA_TELA / 2) + 50, 0); 

        

       	al_draw_bitmap(botao_voltar, 130 - al_get_bitmap_width(botao_voltar) / 2,
                50 - al_get_bitmap_height(botao_voltar) / 2, 0);

        al_flip_display();
    }
    return 0;
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