#include "calibragem.h"

parametros *aloca_parametros(){
    parametros *p = malloc(sizeof(parametros));

    p->matiz_dedo_min = 260;
    p->matiz_dedo_max = 360;
    p->saturacao_dedo = 10;
    p->matiz_mao_min = 200;
    p->matiz_mao_max = 240;
    p->saturacao_mao = 30;
    p->massa_min = 100;
    p->k = 5;

    return p;
}

int calibragem(parametros *p, rastreador *r, hsv **h, camera *cam, ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *fundo, ALLEGRO_FONT *fonte, int altura, int largura){
    ALLEGRO_TIMER *timer = al_create_timer(1.0);

    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();

    ALLEGRO_BITMAP *buffer = al_get_backbuffer(janela);

    ALLEGRO_BITMAP *tela = al_create_sub_bitmap(buffer, (largura - cam->largura)/2, (altura - cam->altura)/2, cam->largura, cam->altura);

    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));

    camera_atualiza(cam);

	disjoint *d = aloca_disjoint(1000);

	int componentes = 0;

    int tempo = 0, sair = 0;

    al_start_timer(timer);

	while(componentes != 1 && tempo < 20 && !sair){
        camera_atualiza(cam);

        if(!al_is_event_queue_empty(fila)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);

            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = 1;
                break;
            }

            if(evento.type == ALLEGRO_EVENT_TIMER && evento.timer.source == timer){
        		rgb_hsv(r, cam->quadro, h);

        		erosao_mao(r, p, h, 3, 3);

            	dilatacao_mao(r, p, h, 3, 3);

            	connected_components_mao(r, p, h, d);

            	componentes = conta_componentes(d);

                printf("Componentes na tela: %d\n", componentes);

            	reseta_disjoint(d);

                tempo++;
            }
        }

        al_draw_bitmap(fundo, 0, 0, 0);

    	camera_copia(cam, cam->quadro, tela);

    	al_draw_textf(fonte, al_map_rgb(255, 0, 255), (largura / 2), 55, ALLEGRO_ALIGN_CENTRE, "Deixe a mão parada em frente a camera.");

  		al_flip_display();
	}

	libera_disjoint(d);

    al_destroy_timer(timer);
    al_destroy_event_queue(fila);

    if(sair)
        return -1;

    if(componentes != 1)
        return 0;

	atualiza_rastreador(r, p, h);

	int subindo = 0, descendo = 0;

	while(!sair){	
		camera_atualiza(cam);

		rgb_hsv(r, cam->quadro, h);

		erosao_mao(r, p, h, 3, 3);

        dilatacao_mao(r, p, h, 3, 3);

    	atualiza_rastreador(r, p, h);

    	int centro_mao = (r->s - r->n)/2 + r->n;

    	if(centro_mao < r->altura/2){
    		if(centro_mao < r->altura_max + 200)
    			r->altura_max = centro_mao;
    		else{
    			if(descendo)
    				sair = 1;
    		}
    		subindo = 1;
    	}

    	else{
    		if(centro_mao > r->altura_min)
    			r->altura_min = centro_mao;
    		else{
    			if(subindo)
    				sair = 1;
    		}
    		descendo = 1;
    	}

        al_draw_bitmap(fundo, 0, 0, 0);

		camera_copia(cam, cam->quadro, tela);

        int dx = (largura - r->largura)/2;
        int dy = (altura - r->altura)/2;

        al_draw_rectangle(r->w + dx,r->n + dy,r->e + dx,r->s + dy, al_map_rgb(0, 0, 255),1);

        al_draw_line((largura - cam->largura)/2, altura/2, (largura + cam->largura)/2, altura/2, al_map_rgb(0, 0, 255), 1);

    	al_draw_textf(fonte, al_map_rgb(255, 0, 255), (largura / 2), 55, ALLEGRO_ALIGN_CENTRE, "Mova sua mão para cima e para baixo da linha.");

    	al_flip_display();
	}

	r->delimitador = (r->altura_min - r->altura_max)/2 + r->altura_max;

	r->count = 0;

	return 1;
}

void libera_parametros(parametros *p){
	free(p);
}