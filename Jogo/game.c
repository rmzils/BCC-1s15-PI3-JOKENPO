#include "game.h"

#define FPS 60

game *inicializa_game(){
	game *g = malloc(sizeof(game));

	g->p = inicializa_player();
	g->npc = inicializa_player();
	g->rodada = 1;
	g->fase = 1;
	g->continuar = 1;
}

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

void inicializa(){
  	if(!al_init())
    	erro("nao foi possivel inicializar allegro");

  	if(!al_init_primitives_addon())
    	erro("nao foi possivel inicializar adicional de primitivas");

  	al_init_font_addon();

  	if(!al_init_ttf_addon())
    	erro("nao foi possivel inicializar adicional de formato de fontes");

  	if(!al_install_audio())
    	erro("nao foi possivel instalar audio");

  	if(!al_init_acodec_addon())
    	erro("nao foi possivel inicializar adicional de formato de som");

  	if(!al_reserve_samples(1))
    	erro("nao foi possivel inicializar canais de áudio");
}

int aloca_musicas(ALLEGRO_AUDIO_STREAM *musicas[]){
  	if(!(musicas[0] = al_load_audio_stream("musicas/teste.ogg", 4, 1024)))
    	return 0;

  	return 1;
}

int aloca_samples(ALLEGRO_SAMPLE *samples[]){
  	if(!(samples[0] = al_load_sample("samples/maytheforce.wav")))
    	return 0;

  	if(!(samples[1] = al_load_sample("samples/prepare.wav")))
    	return 0;

  	if(!(samples[2] = al_load_sample("samples/firstblood.wav")))
    	return 0;

  	if(!(samples[3] = al_load_sample("samples/doublekill.wav")))
    	return 0;

  	if(!(samples[4] = al_load_sample("samples/killingspree.wav")))
    	return 0;

  	if(!(samples[5] = al_load_sample("samples/monsterkill.wav")))
    	return 0;

  	if(!(samples[6] = al_load_sample("samples/unstoppable.wav")))
    	return 0;

  	if(!(samples[7] = al_load_sample("samples/multikill.wav")))
    	return 0;

  	if(!(samples[8] = al_load_sample("samples/ultrakill.wav")))
    	return 0;

  	if(!(samples[9] = al_load_sample("samples/godlike.wav")))
    	return 0;

  	if(!(samples[10] = al_load_sample("samples/holyshit.wav")))
    	return 0;

  	/*if(!(samples[11] = al_load_sample("samples/fatality.wav")))
    	return 0;

  	if(!(samples[12] = al_load_sample("samples/finish.wav")))
    	return 0;*/

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

void game_loop(){
	game *g = inicializa_game();
	srand(time(NULL));

	camera *cam = camera_inicializa(1);
	if(!cam)
    erro("nao foi possivel inicializar camera");

  	int largura = cam->largura;
  	int altura = cam->altura;

  	inicializa();

  	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  	if(!queue)
    	erro("nao foi possivel criar fila de eventos");

  	ALLEGRO_DISPLAY *display = al_create_display(largura, altura);
  	if(!display)
    	erro("nao foi possivel criar janela");

 	  al_register_event_source(queue, al_get_display_event_source(display));

  	ALLEGRO_FONT *fonte = al_load_font("unifont.ttf", 40, 0);
  	if(!fonte)
    	erro("nao foi possivel carregar fonte");

  	/*ALLEGRO_AUDIO_STREAM *musicas[1];
  	if(!aloca_musicas(musicas))
    	erro("nao foi possivel carregar musicas");*/

  	ALLEGRO_SAMPLE *samples[13];
  	if(!aloca_samples(samples))
    	erro("nao foi possivel carregar samples");

  	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  	if(!timer)
    	erro("nao foi possivel criar relogio");

  	al_register_event_source(queue, al_get_timer_event_source(timer));

  	ALLEGRO_COLOR cor = al_map_rgb_f(0, 0, 1);

  	ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);

  	ALLEGRO_BITMAP *tela = al_create_sub_bitmap(buffer, 0, 0, largura, altura);

  	unsigned char ***matriz = camera_aloca_matriz(cam);

  	rastreador *r = aloca_rastreador(altura, largura);

    hsv **h = hsv_aloca(altura, largura);

  	parametros *p = aloca_parametros();

 	  /**********/

 	  printf("x: %d y: %d\n", largura, altura);

  	//calibragem(p, r, matriz, cam);

  	al_start_timer(timer);

  	int sequencia_sample, ultima_vitoria;

  	/*al_attach_audio_stream_to_mixer(musicas[0], al_get_default_mixer());
  	al_set_audio_stream_playing(musicas[0], 0);*/

  	for(sequencia_sample = 1; sequencia_sample < 2; sequencia_sample++){
    	al_play_sample(samples[sequencia_sample], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

    	int length = al_get_sample_length(samples[sequencia_sample]); // ganbis provisoria;

    	delay(length/25);
  	}

  	printf("passou\n");

  	//al_set_audio_stream_playing(musicas[0], 1);

  	/**********/

  	while(g->continuar){
  		char aux = g->npc->vitoria + 48;

  		al_draw_text(fonte, cor, 100, 100, 0, &aux);

  		aux = g->rodada + 48;

  		al_draw_text(fonte, cor, 300,100, 0, &aux);

  		aux = g->p->vitoria + 48;

  		al_draw_text(fonte, cor, 500, 100, 0, &aux);

  		al_flip_display();

  		ALLEGRO_EVENT event;

    	al_wait_for_event(queue, &event);

    	if(g->rodada < 11){
			    if(g->fase == 1){
	        	if(!contador(r)){
	          		camera_atualiza(cam);

                copia_quadro(matriz, cam->quadro, altura, largura);

                rgb_hsv(r, cam->quadro, h);

                erosao_mao(r, p, h, 3, 3);

                dilatacao_mao(r, p, h, 3, 3);

                atualiza_rastreador(r, p, h);

                black_white(r, p, h, matriz);

                camera_copia(cam, matriz, tela);

	          		al_draw_rectangle(r->w,r->n,r->e,r->s,cor,1);

	          		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	            		g->continuar = 0;
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

	          		camera_copia(cam, matriz, tela);

	          		rgb_hsv(r, cam->quadro, h);

                erosao_mao(r, p, h, 1, 3);

                dilatacao_mao(r, p, h, 3, 5);

                atualiza_rastreador(r, p, h);

	          		al_draw_rectangle(r->w,r->n,r->e,r->s,cor,1);

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
	      		int rodada = g->rodada - 1;

	        	int jogada_player = analiza_jogada(r, p, h);

            black_white_finger(r, p, h, matriz);

	        	adiciona_jogada(g->p, rodada, jogada_player);

	        	camera_copia(cam, matriz, tela);

	        	al_flip_display();

	        	delay(2000);

	        	if(jogada_player == -1){
	          		printf("jogada invalida!\n");
	          		//buzina
	          		jogada_invalida(g);
	        	}

	        	else{
	          		printf("Você jogou: ");

	          		imprime_jogada(jogada_player);

	          		int jogada_npc = 0; //rand() % 5;

	          		adiciona_jogada(g->npc, rodada, jogada_npc);

	          		printf("jogada do npc: ", jogada_npc);

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

	            		if(sequencia_sample == 2){
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
	                			sequencia_sample = 3;
	            		}
	          		}
	        	}

	        	g->fase = 1;
	        	g->rodada++;
      		}
		}
	}

	if(g->continuar){
		if(g->p->vitoria > g->npc->vitoria)
			printf("Você venceu!\n");
		else if(g->p->vitoria == g->npc->vitoria)
			printf("Empatou!\n");
		else
			printf("Voce perdeu!\n");
	}

	al_stop_timer(timer);

  /**********/

  	al_destroy_bitmap(tela);

  	camera_libera_matriz(cam, matriz);

  	/**********/

  	al_unregister_event_source(queue, al_get_timer_event_source(timer));
  	al_destroy_timer(timer);

  	al_unregister_event_source(queue, al_get_display_event_source(display));
  	al_destroy_display(display);

  	al_destroy_font(fonte);

  	al_destroy_event_queue(queue);
  	al_shutdown_primitives_addon();
  	al_uninstall_system();

  	camera_finaliza(cam);

  	libera_rastreador(r);

  	libera_parametros(p);
}