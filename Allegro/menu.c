#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "menu.h"
#include "game.h"
#include "player.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL, 
			   *botao_sair = NULL, 
			   *botao_tutorial = NULL, 
			   *botao_jogar = NULL,
			   *botao_voltar = NULL,
			   *tesoura = NULL,
			   *spock = NULL,
			   *papel = NULL,
			   *lagarto = NULL,
			   *pedra = NULL,
			   *tutorial = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_TIMER *timer_jogo = NULL;

bool inicializar(const int LARGURA_TELA, const int ALTURA_TELA)
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
        al_destroy_font(fonte);
        return false;
    }
 
    fundo = al_load_bitmap("fundo.png");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_font(fonte);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        al_destroy_font(fonte);
        al_destroy_event_queue(fila_eventos);
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

    botao_tutorial = al_load_bitmap("tutorial.png");
    if (!botao_tutorial)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_display(janela);
        return false;
    }

    botao_voltar = al_load_bitmap("voltar.png");
    if (!botao_tutorial)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

    tutorial = al_load_bitmap("fundotutorial.png");
    if (!tutorial)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

    tesoura = al_load_bitmap("scissors.png");
    if (!tesoura)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

    spock = al_load_bitmap("spock.png");
    if (!spock)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

    papel = al_load_bitmap("paper.png");
    if (!papel)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

    lagarto = al_load_bitmap("lizard.png");
    if (!lagarto)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

    pedra = al_load_bitmap("rock.png");
    if (!pedra)
    {
        fprintf(stderr, "Falha ao criar botão do tutorial.\n");
        al_destroy_bitmap(botao_jogar);
        al_destroy_bitmap(botao_sair);
        al_destroy_bitmap(botao_tutorial);
        al_destroy_display(janela);
        return false;
    }

   timer_jogo = al_create_timer(1.0);
   if(!timer_jogo) {
      fprintf(stderr, "failed to create timer!\n");
      return false;
   }
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer_jogo));

    return true;
}

void menu()
{
	const int LARGURA_TELA = 1240;
	const int ALTURA_TELA = 768;
 
    bool sair = false;
    int flag;

    if (!inicializar(LARGURA_TELA, ALTURA_TELA))
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
                if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) - 50 - al_get_bitmap_height(botao_jogar) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) - 50 + al_get_bitmap_height(botao_jogar) / 2)
                {
            		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_tutorial) / 2 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_tutorial) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 40 - al_get_bitmap_height(botao_tutorial) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 40 + al_get_bitmap_height(botao_tutorial) / 2)
                {
            		al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_sair) / 2 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_sair) / 2 &&
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
                if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_jogar) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) - 50 - al_get_bitmap_height(botao_jogar) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) - 50 + al_get_bitmap_height(botao_jogar) / 2)
                {
                    game_turn(LARGURA_TELA, ALTURA_TELA, fonte, fundo, timer_jogo, fila_eventos);
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_tutorial) / 2 &&
                    	 evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_tutorial) / 2 &&
                    	 evento.mouse.y >= (ALTURA_TELA / 2) - 40 - al_get_bitmap_height(botao_tutorial) / 2 &&
                    	 evento.mouse.y <= (ALTURA_TELA / 2) - 40 + al_get_bitmap_height(botao_tutorial) / 2)
                {
                	flag = game_tutorial(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, tutorial, janela, 
                		tesoura, spock, papel, lagarto, pedra, fonte);
                	if (flag == -1)
            		{
            			sair = true;
            		}
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_sair) / 2 &&
                    	 evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_sair) / 2 &&
                    	 evento.mouse.y >= (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2 &&
                    	 evento.mouse.y <= (ALTURA_TELA / 2) + 100 + al_get_bitmap_height(botao_sair) / 2)
                {
                	sair = true;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        }

        al_draw_bitmap(fundo, 0, 0, 0);
        
        al_set_target_bitmap(al_get_backbuffer(janela));
        al_draw_bitmap(botao_jogar, (LARGURA_TELA / 2) - al_get_bitmap_width(botao_jogar) / 2,
                       (ALTURA_TELA / 3) - 50 - al_get_bitmap_height(botao_jogar) / 2, 0);
        al_draw_bitmap(botao_tutorial, (LARGURA_TELA / 2) - al_get_bitmap_width(botao_tutorial) / 2,
                       (ALTURA_TELA / 2) - 40 - al_get_bitmap_height(botao_tutorial) / 2, 0);
        al_draw_bitmap(botao_sair, (LARGURA_TELA / 2) - al_get_bitmap_width(botao_sair) / 2,
                       (ALTURA_TELA / 2) + 100 - al_get_bitmap_height(botao_sair) / 2, 0);
 		
        // Atualiza a tela
        al_flip_display();
    }
 
    // Desaloca os recursos utilizados na aplicação
    al_destroy_bitmap(botao_sair);
    al_destroy_bitmap(botao_jogar);
    al_destroy_bitmap(botao_tutorial);
    al_destroy_bitmap(tutorial);
    al_destroy_bitmap(fundo);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_font(fonte);
    al_destroy_timer(timer_jogo);
}