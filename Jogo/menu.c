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
			   *tutorial = NULL,
               *jogadas = NULL,
               *regras = NULL;
ALLEGRO_FONT *fonte = NULL;

void inicializar(const int LARGURA_TELA, const int ALTURA_TELA)
{
	if (!al_init())
        erro("nao foi possivel inicializar o allegro");
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon())
        erro("Falha ao inicializar add-on allegro_ttf.");

    if (!al_init_image_addon())
        erro("Falha ao inicializar add-on allegro_image.");

    if(!al_init_primitives_addon())
        erro("nao foi possivel inicializar adicional de primitivas");
 
    if (!al_install_keyboard())
        erro("Falha ao inicializar o teclado.");

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
        erro("Falha ao criar janela.");

    al_set_window_title(janela, "Utilizando o Teclado");

    fonte = al_load_font("Fonts/stormfaze.ttf", 40, 0);
    if (!fonte)
        erro("Falha ao carregar fonte.");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
        erro("Falha ao criar fila de eventos.");
 
    fundo = al_load_bitmap("Bitmaps/fundo.png");
    if (!fundo)
        erro("Falha ao carregar imagem de fundo.");

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse())
        erro("Falha ao inicializar o mouse.");
 
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
        erro("Falha ao atribuir ponteiro do mouse.");
 
    botao_jogar = al_load_bitmap("Bitmaps/jogar.png");
    if (!botao_jogar)
        erro("Falha ao criar botão jogar.");
 
    botao_sair = al_load_bitmap("Bitmaps/sair.png");
    if (!botao_sair)
        erro("Falha ao criar botão de saída.");

    botao_tutorial = al_load_bitmap("Bitmaps/tutorial.png");
    if (!botao_tutorial)
        erro("Falha ao criar botão do tutorial.");

    botao_voltar = al_load_bitmap("Bitmaps/voltar.png");
    if (!botao_tutorial)
        erro("Falha ao criar botão do tutorial.");

    tutorial = al_load_bitmap("Bitmaps/fundotutorial.png");
    if (!tutorial)
        erro("Falha ao criar botão do tutorial.");

    jogadas = al_load_bitmap("Bitmaps/Jogadas.png");
    if (!jogadas)
        erro("Falha ao criar botão do tutorial.");

    regras = al_load_bitmap("Bitmaps/Regras.png");
    if (!regras)
        erro("Falha ao criar botão do tutorial.");

    tesoura = al_load_bitmap("Bitmaps/scissors.png");
    if (!tesoura)
        erro("Falha ao criar botão do tutorial.");

    spock = al_load_bitmap("Bitmaps/spock.png");
    if (!spock)
        erro("Falha ao criar botão do tutorial.");

    papel = al_load_bitmap("Bitmaps/paper.png");
    if (!papel)
        erro("Falha ao criar botão do tutorial.");

    lagarto = al_load_bitmap("Bitmaps/lizard.png");
    if (!lagarto)
        erro("Falha ao criar botão do tutorial.");

    pedra = al_load_bitmap("Bitmaps/rock.png");
    if (!pedra)
        erro("Falha ao criar botão do tutorial.");

    if(!al_install_audio())
        erro("nao foi possivel instalar audio");

    if(!al_init_acodec_addon())
        erro("nao foi possivel inicializar adicional de formato de som");

    if(!al_reserve_samples(1))
        erro("nao foi possivel inicializar canais de áudio");   
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
}

void menu()
{
	const int LARGURA_TELA = 1240;
	const int ALTURA_TELA = 768;
 
    bool sair = false;
    int flag;

    inicializar(LARGURA_TELA, ALTURA_TELA);

    while (!sair)
    {
        // Verificamos se há eventos na fila
        if (!al_is_event_queue_empty(fila_eventos))
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
                    flag = game_turn(janela, LARGURA_TELA, ALTURA_TELA, fonte);
                    if(flag == -1)
                        sair = true;
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(botao_tutorial) / 2 &&
                    	 evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(botao_tutorial) / 2 &&
                    	 evento.mouse.y >= (ALTURA_TELA / 2) - 40 - al_get_bitmap_height(botao_tutorial) / 2 &&
                    	 evento.mouse.y <= (ALTURA_TELA / 2) - 40 + al_get_bitmap_height(botao_tutorial) / 2)
                {
                	flag = game_tutorial(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, tutorial, janela, 
                        tesoura, spock, papel, lagarto, pedra, jogadas, regras, fonte);
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
        al_draw_text(fonte, al_map_rgb(0, 0, 255), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "RAINBOW-PO");
        
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
    al_destroy_font(fonte);
}