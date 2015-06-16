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

    if(!(samples[10] = al_load_sample("Samples/wrong_answer.wav")))
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
    int rodada = g->rodada;

    g->resultado[rodada] = -1;

    adiciona_jogada(g->p, rodada, -1);

    adiciona_jogada(g->npc, rodada, -1);

    g->rodada++;
}

void imprime_mao(rastreador *r, int altura, int largura, ALLEGRO_BITMAP *jogadas[], int jogada){
  int cx = al_get_bitmap_width(jogadas[jogada])/2;
  int cy = al_get_bitmap_height(jogadas[jogada]);

  float angulo = ((r->angulo)*PI)/180;

  al_draw_rotated_bitmap(jogadas[jogada], cx, cy, (largura - cy)/2, altura/2, angulo, 0);
}

int game_turn(ALLEGRO_DISPLAY *janela, const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_FONT *fonte)
{
	game *g = inicializa_game();

    camera *cam = camera_inicializa(0);
    if(!cam)
        erro("nao foi possivel inicializar camera");

    ALLEGRO_BITMAP *buffer = al_get_backbuffer(janela);

    ALLEGRO_BITMAP *tela = al_create_sub_bitmap(buffer, 0, 90, cam->largura, cam->altura);

    ALLEGRO_TIMER *tempo = NULL;
    tempo = al_create_timer(1.0);
    ALLEGRO_TIMER *fps = NULL;
    fps = al_create_timer(1.0 / 60);

    ALLEGRO_EVENT_QUEUE *fila_tempo = al_create_event_queue();
    ALLEGRO_EVENT_QUEUE *fila_fps = al_create_event_queue();

    al_register_event_source(fila_tempo, al_get_timer_event_source(tempo));
    al_register_event_source(fila_tempo, al_get_display_event_source(janela));
    al_register_event_source(fila_fps, al_get_timer_event_source(fps));

    srand(time(NULL));

    int largura = cam->largura;
    int altura = cam->altura;

    ALLEGRO_SAMPLE *samples[11];
    if(!aloca_samples(samples))
        erro("nao foi possivel carregar samples");

    ALLEGRO_AUDIO_STREAM *musica = al_load_audio_stream("Musicas/Spaceman.ogg", 4, 1024);
    if(!musica)
        erro("nao foi possivel carregar musicas");

    rastreador *r = aloca_rastreador(altura, largura);

    parametros *p = aloca_parametros();

    ALLEGRO_BITMAP *fundo_jogo = al_load_bitmap("Bitmaps/fundo2.png");

    ALLEGRO_BITMAP *jogadas[6];

    jogadas[0] = al_load_bitmap("Bitmaps/rock.png");
    jogadas[1] = al_load_bitmap("Bitmaps/paper.png");
    jogadas[2] = al_load_bitmap("Bitmaps/scissors.png");
    jogadas[3] = al_load_bitmap("Bitmaps/lizard.png");
    jogadas[4] = al_load_bitmap("Bitmaps/spock.png");
    jogadas[5] = al_load_bitmap("Bitmaps/lizardh.png");

    unsigned char ***matriz = camera_aloca_matriz(cam);

    hsv **h = hsv_aloca(cam->altura, cam->largura);

    int sequencia_sample = 1, ultima_vitoria = -2, sequencia_vitorias;

    bool sair = false;

    int calibrou = calibragem(p, r, h, cam, janela, fundo_jogo, fonte, ALTURA_TELA, LARGURA_TELA);

    if(calibrou == 0){
        al_show_native_message_box(janela, "Erro", "Erro na calibragem",
                                    "O ambiente contem muito ruido, tente mudar de local ou virar a camere em outra direcao.",
                                    "OK", ALLEGRO_MESSAGEBOX_ERROR);
        sair = true;
    }

    else if(calibrou == -1)
        sair = true;

    else
        al_play_sample(samples[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, 1);

    int fim_jogo = 0;
    int tecla = 0; 
    int	min_jogo = 2, seg_jogo = 0;
    int contador_seg = 0;
    int aguardar = 0;

    al_start_timer(tempo);
    al_start_timer(fps);

    while (!sair && !fim_jogo)
    {
        int jogada_npc, jogada;

        ALLEGRO_EVENT evento;

        if(!al_is_event_queue_empty(fila_tempo))
        {
            al_wait_for_event(fila_tempo, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
                break;
            }

            if (evento.type == ALLEGRO_EVENT_TIMER)
            {
                if(evento.timer.source == tempo){
                    if (min_jogo == 0 && seg_jogo <= 1) 
                    {
                        sair = true;
                        fim_jogo = 1;
                    }

                    seg_jogo--;

                    if (seg_jogo <= 0)
                    {
                        min_jogo--;
                        seg_jogo = 59;
                    }
                }
            }
        }

        if(!al_is_event_queue_empty(fila_fps)){
            al_wait_for_event(fila_fps, &evento);

            if(evento.type == ALLEGRO_EVENT_TIMER){
                if(evento.timer.source == fps){
                    al_draw_bitmap(fundo_jogo, 0, 0, 0);

                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA / 2), 15, 
                                ALLEGRO_ALIGN_CENTRE, "%dm %ds", min_jogo, seg_jogo);

                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 
                                ALLEGRO_ALIGN_LEFT, "Rodada: %d", g->rodada + 1);

                    al_draw_textf(fonte,al_map_rgb(255, 255, 255), LARGURA_TELA - 30, 10, 
                                ALLEGRO_ALIGN_RIGHT, "Jogador: %d", g->p->vitoria);

                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA - 30, 50, 
                                ALLEGRO_ALIGN_RIGHT, "NPC: %d", g->npc->vitoria);

                    al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA - 200,
                                ALLEGRO_ALIGN_CENTER, "Contador: %d", r->count);

                    camera_copia(cam, cam->quadro, tela);
                    
                    al_draw_rectangle(r->w,r->n + 90,r->e,r->s + 90, al_map_rgb(0, 0, 255),1);

                    al_draw_line(0, 90 + r->delimitador, cam->largura, 90 + r->delimitador, al_map_rgb(0, 0, 255), 1);

                    if(g->rodada > 0){
                        jogada = g->p->jogada[g->rodada - 1];

                        if(aguardar > 0 && jogada != -1){
                            imprime_mao(r, ALTURA_TELA, LARGURA_TELA + LARGURA_TELA/2, jogadas, jogada_npc);
                            aguardar--;
                        }
                        
                        else
                            aguardar = 0;
                    }

                    if(aguardar == 0 && g->fase != 3){
                            atualiza_angulo(r);
                            imprime_mao(r, ALTURA_TELA, LARGURA_TELA + LARGURA_TELA/2, jogadas, 0);
                    }

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
                                al_play_sample(samples[10], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                jogada_invalida(g);
                                g->fase = 1;
                                r->count = 0;
                            }    
                        }

                        else
                            g->fase = 3;
                    }

                    /**********/

                    else if(g->fase == 3){
                        hsv **h_aux = hsv_aloca(altura, largura);

                        camera_atualiza(cam);

                        rgb_hsv(r, cam->quadro, h);

                        camera_atualiza(cam);

                        rgb_hsv(r, cam->quadro, h_aux);

                        merge(r, p, h, h_aux);

                        camera_atualiza(cam);

                        rgb_hsv(r, cam->quadro, h_aux);

                        merge(r, p, h, h_aux);

                        hsv_libera(h_aux, cam->altura);

                        atualiza_rastreador(r, p, h);

                        int rodada = g->rodada;

                        int jogada_player = analiza_jogada(r, p, h);

                        al_flip_display();

                        adiciona_jogada(g->p, rodada, jogada_player);

                        if(jogada_player == -1){
                            al_play_sample(samples[10], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            jogada_invalida(g);
                        }

                        else{
                            jogada_npc = rand() % 5;

                            adiciona_jogada(g->npc, rodada, jogada_npc);

                            if(jogada_player == jogada_npc){
                                g->resultado[rodada] = 2;

                                if(sequencia_sample != -2)
                                    sequencia_sample = 0;
                            }

                            else{
                                g->resultado[rodada] = vitoria_jogador(jogada_player, jogada_npc);

                                if(g->resultado[rodada] == 1)
                                    adiciona_vitoria(g->p);

                                else
                                    adiciona_vitoria(g->npc);

                                if(ultima_vitoria == -2){
                                    al_play_sample(samples[sequencia_sample], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    sequencia_sample = 1;
                                    ultima_vitoria = -1;
                                }

                                else{
                                    ultima_vitoria = g->resultado[rodada - 1];

                                    if(ultima_vitoria == g->resultado[rodada])
                                        sequencia_sample++;
                                    else
                                        sequencia_sample = 1;

                                    if(sequencia_sample >= 2){
                                        al_play_sample(samples[sequencia_sample], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                        if(sequencia_sample < 10)
                                            sequencia_sample++;                                    
                                    }
                                }
                            }
                        }

                        g->fase = 1;
                        g->rodada++;
                        r->count = 0;
                        aguardar = 20;
                    }

                    al_flip_display();
                }
            }
        }
    }

    int flag = 1;

    if(fim_jogo){
        char *msg_final;

        if(g->p->vitoria == g->npc->vitoria)
            msg_final = "Empatou!";
        else
            if(g->p->vitoria > g->npc->vitoria)
                msg_final = "Voce Venceu!";
            else
                msg_final = "Voce Perdeu";

        int botao = 0;

        while(!botao)
            botao = al_show_native_message_box(janela, "Fim de Jogo", msg_final, "Clique em OK para voltar ao menu.", NULL, 0);
    }
    else
        flag = -1;

    al_set_audio_stream_playing(musica, 0);

    al_destroy_bitmap(fundo_jogo);
    
    for(int i = 0; i < 5; i++)
        al_destroy_bitmap(jogadas[i]);

    hsv_libera(h, cam->altura);

    libera_rastreador(r);

    libera_parametros(p);

    camera_libera_matriz(cam, matriz);

    camera_finaliza(cam);

    game_finaliza(g);

    al_destroy_timer(tempo);
    al_destroy_timer(fps);
    al_destroy_event_queue(fila_tempo);
    al_destroy_event_queue(fila_fps);

    return flag;
}

int game_tutorial(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
    ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_BITMAP *tutorial, ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *tesoura,
    ALLEGRO_BITMAP *spock, ALLEGRO_BITMAP *papel, ALLEGRO_BITMAP *lagarto, ALLEGRO_BITMAP *pedra, ALLEGRO_BITMAP *jogadas, ALLEGRO_BITMAP *regras, ALLEGRO_FONT *fonte)
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
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(jogadas) / 2 &&
                         evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(jogadas) / 2 &&
                         evento.mouse.y >= (ALTURA_TELA / 3) - 50 - al_get_bitmap_height(jogadas) / 2 &&
                         evento.mouse.y <= (ALTURA_TELA / 3) - 50 + al_get_bitmap_height(jogadas) / 2)
                {
                    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(regras) / 2 &&
                         evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(regras) / 2 &&
                         evento.mouse.y >= (ALTURA_TELA / 2) - 40 - al_get_bitmap_height(regras) / 2 &&
                         evento.mouse.y <= (ALTURA_TELA / 2) - 40 + al_get_bitmap_height(regras) / 2)
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
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(jogadas) / 2 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(jogadas) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 3) - 50 - al_get_bitmap_height(jogadas) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 3) - 50 + al_get_bitmap_height(jogadas) / 2)
                {
                    flag = jogada(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, tutorial, janela, tesoura, spock, papel, lagarto, pedra, fonte);
                    if (flag == -1)
                    {
                        return -1;
                    }
                }
                else if (evento.mouse.x >= (LARGURA_TELA / 2) - al_get_bitmap_width(regras) / 2 &&
                    evento.mouse.x <= (LARGURA_TELA / 2) + al_get_bitmap_width(regras) / 2 &&
                    evento.mouse.y >= (ALTURA_TELA / 2) - 40 - al_get_bitmap_height(regras) / 2 &&
                    evento.mouse.y <= (ALTURA_TELA / 2) - 40 + al_get_bitmap_height(regras) / 2)
                {
                    flag = regra(LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, janela, fonte);
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
        
        al_draw_bitmap(jogadas, (LARGURA_TELA / 2) - al_get_bitmap_width(jogadas) / 2, (ALTURA_TELA / 3) - 50 -
            al_get_bitmap_height(jogadas) / 2, 0);
        al_draw_bitmap(regras, (LARGURA_TELA / 2) - al_get_bitmap_width(regras) / 2, (ALTURA_TELA / 2) - 40 - 
            al_get_bitmap_height(regras) / 2, 0);

        al_draw_bitmap(botao_voltar, 130 - al_get_bitmap_width(botao_voltar) / 2, 50 - al_get_bitmap_height(botao_voltar) / 2, 0);

        al_flip_display();
    }
    return 0;
}

int regra(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
    ALLEGRO_BITMAP *fundo, ALLEGRO_BITMAP *botao_voltar, ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte)
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
        
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2), (ALTURA_TELA / 2) - 350, 
                    ALLEGRO_ALIGN_LEFT, "Regras");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 600, (ALTURA_TELA / 2) - 200, 
                    ALLEGRO_ALIGN_LEFT, "- O jogo tem duração de 2 minutos.");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 600, (ALTURA_TELA / 2) - 130, 
                    ALLEGRO_ALIGN_LEFT, "- Balançar a mão 3 vezes antes de fazer a jogada.");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 600, (ALTURA_TELA / 2) - 50, 
                    ALLEGRO_ALIGN_LEFT, "- Há somente 5 opções de jogadas válidas,");
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 550, (ALTURA_TELA / 2) + 10, 
                    ALLEGRO_ALIGN_LEFT, "    são elas, Pedra, Papel, Tesoura, Spock e Lagarto.");

        al_draw_bitmap(botao_voltar, 130 - al_get_bitmap_width(botao_voltar) / 2,
                50 - al_get_bitmap_height(botao_voltar) / 2, 0);

        al_flip_display();
    }
    return 0;
}

int jogada(const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
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
                        flag = ganhade(1, LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, tesoura, lagarto, papel, spock, janela, fonte);
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
                        flag = ganhade(2, LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, pedra, tesoura, papel, lagarto, janela, fonte);
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
                        flag = ganhade(3, LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, pedra, spock, tesoura, lagarto, janela, fonte);
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
                        flag = ganhade(4, LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, papel, lagarto, pedra, spock, janela, fonte);
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
                        flag = ganhade(5, LARGURA_TELA, ALTURA_TELA, fila_eventos, fundo, botao_voltar, papel, spock, tesoura, pedra, janela, fonte);
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

int ganhade(int clique, const int LARGURA_TELA, const int ALTURA_TELA, ALLEGRO_EVENT_QUEUE *fila_eventos,
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


            if (clique == 1)
            {
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2), (ALTURA_TELA / 2) - 350, 
                    ALLEGRO_ALIGN_CENTRE, "Pedra");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Tesoura");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Lagarto");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Papel");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Spock");
            }
            else if (clique == 2)
            {
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2), (ALTURA_TELA / 2) - 350, 
                    ALLEGRO_ALIGN_CENTRE, "Spock");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Pedra");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Tesoura");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Papel");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Lagarto");
            }
            else if (clique == 3)
            {
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2), (ALTURA_TELA / 2) - 350, 
                    ALLEGRO_ALIGN_CENTRE, "Papel");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Pedra");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Spock");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Tesoura");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Lagarto");
            }
            else if (clique == 4)
            {
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2), (ALTURA_TELA / 2) - 350, 
                    ALLEGRO_ALIGN_CENTRE, "Tesoura");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Papel");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Lagarto");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Pedra");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Spock");
            }
            else if (clique == 5)
            {
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2), (ALTURA_TELA / 2) - 350, 
                    ALLEGRO_ALIGN_CENTRE, "Lagarto");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Papel");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) - 350, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Spock");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2), 
                    ALLEGRO_ALIGN_CENTRE, "Tesoura");
                al_draw_textf(fonte, al_map_rgb(0, 0, 0), (LARGURA_TELA / 2) + 450, (ALTURA_TELA / 2) + 250, 
                    ALLEGRO_ALIGN_CENTRE, "Pedra");
            }
       
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