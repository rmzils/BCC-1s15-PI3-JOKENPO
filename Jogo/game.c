#include "game.h"

void erro(char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);

    exit(EXIT_FAILURE);
}

void delay(int milliseconds){
      long pause;
      clock_t now,then;

      pause = milliseconds*(CLOCKS_PER_SEC/1000);
      now = then = clock();
      while((now-then) < pause)
            now = clock();
}

game *inicializa_game(){
	game *g = malloc(sizeof(game));

	g->p = inicializa_player();
	g->npc = inicializa_player();
	g->rodada = 0;
    g->fase = 1;

	return g;
}

int aloca_musicas(ALLEGRO_AUDIO_STREAM *musicas[]){
    if(!(musicas[0] = al_load_audio_stream("musicas/teste.ogg", 4, 1024)))
        return 0;

    return 1;
}

int aloca_samples(ALLEGRO_SAMPLE *samples[]){
    if(!(samples[0] = al_load_sample("Samples/prepare.wav")))
        return 0;

    if(!(samples[1] = al_load_sample("Samples/firstblood.wav")))
        return 0;

    if(!(samples[2] = al_load_sample("Samples/doublekill.wav")))
        return 0;

    if(!(samples[3] = al_load_sample("Samples/killingspree.wav")))
        return 0;

    if(!(samples[4] = al_load_sample("Samples/monsterkill.wav")))
        return 0;

    if(!(samples[5] = al_load_sample("Samples/unstoppable.wav")))
        return 0;

    if(!(samples[6] = al_load_sample("Samples/multikill.wav")))
        return 0;

    if(!(samples[7] = al_load_sample("Samples/ultrakill.wav")))
        return 0;

    if(!(samples[8] = al_load_sample("Samples/godlike.wav")))
        return 0;

    if(!(samples[9] = al_load_sample("Samples/holyshit.wav")))
        return 0;

  return 1;
 }

int vitoria_jogador(int jogada, int jogada_npc){
    if(jogada_npc == 0 && (jogada == 1 || jogada == 4))
        return 1;
    else if(jogada_npc == 1 && (jogada == 2 || jogada == 3))
        return 1;
    else if(jogada_npc == 2 && (jogada == 0 || jogada == 4))
        return 1;
    else if(jogada_npc == 3 && (jogada == 0 || jogada == 2))
        return 1;
    else if(jogada_npc == 4 && (jogada == 1 || jogada == 3))
        return 1;
  
    return 0;
}

void jogada_invalida(game *g){
    int rodada = g->rodada - 1;

    g->resultado[rodada] = 0;

    adiciona_jogada(g->p, rodada, -1);

    adiciona_jogada(g->npc, rodada, -1);
    
    adiciona_vitoria(g->npc);

    g->rodada++;
}

void imprime_mao(rastreador *r, int altura, int largura, ALLEGRO_BITMAP *jogadas[], int jogada){
  int cx = al_get_bitmap_width(jogadas[jogada])/2;
  int cy = al_get_bitmap_height(jogadas[jogada]);

  float angulo = ((r->angulo)*PI)/180;

  al_draw_rotated_bitmap(jogadas[jogada], cx, cy, (largura - cy)/2, altura/2, angulo, 0);
}

void game_turn(ALLEGRO_DISPLAY *janela, const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_FONT *fonte, 
	ALLEGRO_BITMAP *fundo, ALLEGRO_TIMER *timer_jogo, ALLEGRO_EVENT_QUEUE *fila_eventos)
{
	game *g = inicializa_game();

    srand(time(NULL));

    camera *cam = camera_inicializa(0);
    if(!cam)
        erro("nao foi possivel inicializar camera");

    int largura = cam->largura;
    int altura = cam->altura;

    ALLEGRO_SAMPLE *samples[10];
    if(!aloca_samples(samples))
        erro("nao foi possivel carregar samples");

    rastreador *r = aloca_rastreador(altura, largura);

    parametros *p = aloca_parametros();

    ALLEGRO_BITMAP *fundo_jogo = al_load_bitmap("Bitmaps/fundo2.png");

    ALLEGRO_BITMAP *jogadas[5];

    jogadas[0] = al_load_bitmap("Bitmaps/rock.png");
    jogadas[1] = al_load_bitmap("Bitmaps/paper.png");
    jogadas[2] = al_load_bitmap("Bitmaps/scissors.png");
    jogadas[3] = al_load_bitmap("Bitmaps/lizard.png");
    jogadas[4] = al_load_bitmap("Bitmaps/spock.png");

    unsigned char ***matriz = camera_aloca_matriz(cam);

    hsv **h = hsv_aloca(altura, largura);

    int sequencia_sample = 1, ultima_vitoria;

    if(!calibragem(p, r, h, cam, janela, fundo, fonte, ALTURA_TELA, LARGURA_TELA))
        erro("existe muito ruido no local!\n");

    printf("passou calibragem\n");

    al_play_sample(samples[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

    printf("tocou musica\n");

    bool sair = false;
    int tecla = 0; 
    int	min_jogo = 2, seg_jogo = 0, seg_rodada = 5;
    int wait = 0;

    al_start_timer(timer_jogo);

    while (!sair)
    {
        while(!al_is_event_queue_empty(fila_eventos))
        {

            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
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
            			seg_rodada = 10;
            			g->rodada++;
                        r->count = 0;
                        g->fase = 1;
            		}
            	}
            }
        }

        al_draw_bitmap(fundo_jogo, 0, 0, 0);

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

        atualiza_angulo(r);

        if(wait){
            delay(1000);
            wait = 0;
        }

        if(g->fase != 3)
            imprime_mao(r, ALTURA_TELA, LARGURA_TELA, jogadas, 0);
        else
            wait = 1;


        if(g->fase == 1){
            if(!contador(r)){
                camera_atualiza(cam);

                rgb_hsv(r, cam->quadro, h);

                erosao_mao(r, p, h, 3, 3);

                dilatacao_mao(r, p, h, 3, 3);

                atualiza_rastreador(r, p, h);
            }

            else
                g->fase = 2;

        }

        /**********/

        else if(g->fase == 2){
            camera_atualiza(cam);

            copia_quadro(matriz, cam->quadro, r->altura, r->largura);

            camera_atualiza(cam);

            if(detecta_movimento(r, matriz, cam->quadro)){
                copia_quadro(matriz, cam->quadro, r->altura, r->largura);

                rgb_hsv(r, cam->quadro, h);

                erosao_mao(r, p, h, 3, 3);

                dilatacao_mao(r, p, h, 3, 3);

                atualiza_rastreador(r, p, h);

                camera_atualiza(cam);

                if(r->flag_descendo){
                    printf("Era pra ter jogado e sua mão voltou a subir!\n");
                    //buzina
                    jogada_invalida(g);
                    g->fase = 1;
                }    
            }

            else{
                g->fase = 3;
            }
        }

            /**********/

        else if(g->fase == 3){

            //inicio do RTA

            hsv **h_aux = hsv_aloca(altura, largura);

            camera_atualiza(cam);

            rgb_hsv(r, cam->quadro, h);

            camera_atualiza(cam);

            rgb_hsv(r, cam->quadro, h_aux);

            merge(r, p, h, h_aux);

            camera_atualiza(cam);

            rgb_hsv(r, cam->quadro, h_aux);

            merge(r, p, h, h_aux);

            hsv_libera(h_aux, altura);

            int rodada = g->rodada - 1;

            int jogada_player = analiza_jogada(r, p, h);

            adiciona_jogada(g->p, rodada, jogada_player);

            if(jogada_player == -1){
                printf("jogada invalida!\n");
                //buzina
                jogada_invalida(g);
                imprime_mao(r, ALTURA_TELA, LARGURA_TELA, jogadas, 0);
            }

            else{
                printf("Você jogou: ");

                imprime_jogada(jogada_player);

                int jogada_npc = 0; //rand() % 5;

                adiciona_jogada(g->npc, rodada, jogada_npc);

                printf("jogada do npc: ", jogada_npc);

                imprime_mao(r, ALTURA_TELA, LARGURA_TELA, jogadas, jogada_npc);

                imprime_jogada(jogada_npc);

                if(jogada_player == jogada_npc){
                    printf("Empatou!\n");
                    g->resultado[rodada] = 2;
                }

                else{
                    g->resultado[rodada] = vitoria_jogador(jogada_player, jogada_npc);

                    if(g->resultado[rodada] == 1){// vitoria do jogador
                        printf("você venceu!\n");
                        adiciona_vitoria(g->p);
                    }

                    else{
                        printf("Você perdeu!\n");
                        adiciona_vitoria(g->npc);
                    }

                    if(sequencia_sample == 1){
                        al_play_sample(samples[sequencia_sample], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        sequencia_sample++;
                        ultima_vitoria = g->resultado[rodada];
                    }

                    else{
                        if(ultima_vitoria == g->resultado[rodada]){
                            al_play_sample(samples[sequencia_sample], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            if(sequencia_sample <= 10)
                                sequencia_sample++;
                            
                        }
                        else
                            sequencia_sample = 2;
                    }
                }
            }

            g->fase = 1;
            g->rodada++;
            seg_rodada = 10;
        }
 
        al_flip_display();
    }

    al_destroy_bitmap(fundo_jogo);
    
    for(int i = 0; i < 5; i++)
        al_destroy_bitmap(jogadas[i]);

    hsv_libera(h, altura);

    libera_rastreador(r);

    libera_parametros(p);

    camera_libera_matriz(cam, matriz);

    camera_finaliza(cam);

    game_finaliza(g);
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

void game_finaliza(game *g){
    libera_player(g->p);
    libera_player(g->npc);
    free(g);
}