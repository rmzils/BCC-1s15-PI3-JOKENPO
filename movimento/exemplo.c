#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "camera.h"
#include "imagem.h"
#include "movimento.h"
#include "jogada.h"



#define FPS 60

void delay(int milliseconds){
  long pause;
  clock_t now,then;

  pause = milliseconds*(CLOCKS_PER_SEC/1000);
  now = then = clock();
  while((now-then) < pause)
    now = clock();
}

void erro(char *mensagem) {
  fprintf(stderr, "%s\n", mensagem);

  exit(EXIT_FAILURE);
}


int main() {
  camera *cam = camera_inicializa(0);
  if(!cam)
    erro("nao foi possivel inicializar camera");

  int largura = cam->largura;
  int altura = cam->altura;

  if(!al_init())
    erro("nao foi possivel inicializar allegro");

  if(!al_init_primitives_addon())
    erro("nao foi possivel inicializar adicional de primitivas");

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  if(!queue)
    erro("nao foi possivel criar fila de eventos");

  ALLEGRO_DISPLAY *display = al_create_display(2 * largura, altura);
  if(!display)
    erro("nao foi possivel criar janela");

  al_register_event_source(queue, al_get_display_event_source(display));

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  if(!timer)
    erro("nao foi possivel criar relogio");

  al_register_event_source(queue, al_get_timer_event_source(timer));

  /**********/

  unsigned char ***matriz = camera_aloca_matriz(cam);
  unsigned char ***matriz_anterior = camera_aloca_matriz(cam);

  rastreador *r = aloca_rastreador(altura, largura);

  ALLEGRO_COLOR cor = al_map_rgb_f(0, 0, 1);

  ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);

  ALLEGRO_BITMAP *esquerda = al_create_sub_bitmap(buffer, 0, 0, largura, altura);

  ALLEGRO_BITMAP *direita = al_create_sub_bitmap(buffer, largura, 0, largura, altura);

  /**********/

  int continuar = 1;
  int atualizar = 0;

  parametros *p = aloca_parametros();

  //calibra_dedos(p, r, matriz, cam);

  calibra_mao(p, r, matriz, cam);

  al_start_timer(timer);

  while(continuar) {
    ALLEGRO_EVENT event;

    al_wait_for_event(queue, &event);

    switch(event.type) {
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      continuar = 0;
      break;
    case ALLEGRO_EVENT_TIMER:
      atualizar = 1;
    }

    if(atualizar && al_is_event_queue_empty(queue)){

      /*while(1){
        camera_atualiza(cam);

        copia_quadro(matriz, cam->quadro, altura, largura);

        camera_copia(cam, matriz, esquerda);

        int jogada = analiza_jogada(r, p, matriz, cam);

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
        else
          printf("INVALIDA\n");

        printf("min: %d max: %d\n", p->massa_min, p->massa_max);

        camera_copia(cam, matriz, direita);

        al_flip_display();
      }*/

      while(!contador(r)){
        camera_atualiza(cam);

        copia_quadro(matriz, cam->quadro, altura, largura);

        camera_copia(cam, matriz, esquerda);

        black_white_hand(r,p,matriz);

        abertura(r, matriz, 6);

        erosao(r, matriz, 3, 3);

        atualiza_rastreador(r, matriz);

        al_draw_rectangle(r->w,r->n,r->e,r->s,cor,1);

        camera_copia(cam, matriz, direita);

        al_flip_display();
      }

      printf("contou\n");

      camera_atualiza(cam);

      copia_quadro(matriz, cam->quadro, r->altura, r->largura);

      camera_atualiza(cam);

      while(detecta_movimento(r, matriz, cam->quadro)){
        copia_quadro(matriz, cam->quadro, r->altura, r->largura);

        camera_copia(cam, matriz, esquerda);

        black_white_hand(r, p, cam->quadro);

        abertura(r, cam->quadro, 3);

        erosao(r, cam->quadro, 1, 10);

        atualiza_rastreador(r, cam->quadro);

        if(r->flag_descendo){
          printf("Era pra ter jogado e sua mão voltou a subir!\n");
          exit(EXIT_SUCCESS);
        }

        al_draw_rectangle(r->w,r->n,r->e,r->s,cor,1);

        camera_copia(cam, cam->quadro, direita);

        camera_atualiza(cam);

        al_flip_display();        
      };

      int jogada = analiza_jogada(r, p, matriz, cam);

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
      else
        printf("INVALIDA\n");

      printf("min: %d max: %d\n", p->massa_min, p->massa_max);

      camera_copia(cam, matriz, direita);

      al_flip_display();
    }
  }

  al_stop_timer(timer);

  /**********/

  al_destroy_bitmap(direita);

  al_destroy_bitmap(esquerda);

  camera_libera_matriz(cam, matriz);

  /**********/

  al_unregister_event_source(queue, al_get_timer_event_source(timer));
  al_destroy_timer(timer);

  al_unregister_event_source(queue, al_get_display_event_source(display));
  al_destroy_display(display);

  al_destroy_event_queue(queue);
  al_shutdown_primitives_addon();
  al_uninstall_system();

  camera_finaliza(cam);

  return EXIT_SUCCESS;
}
