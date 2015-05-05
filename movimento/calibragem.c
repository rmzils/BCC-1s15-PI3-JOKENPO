#include "calibragem.h"

parametros *aloca_parametros(){
	parametros *p = malloc(sizeof(parametros));

	p->matiz_dedo_min = 280;
	p->matiz_dedo_max = 340;
	p->saturacao_dedo = 30;
	p->matiz_mao_min = 180;
	p->matiz_mao_max = 240;
	p->saturacao_mao = 50;
	p->massa_min = 100;
	p->massa_max = 0;

	return p;
}

void calibra_mao(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam){
	p->matiz_mao_min = 180;
	p->matiz_mao_max = 240;
	p->saturacao_mao = 50;

	int componentes = 0;

	disjoint *d = aloca_disjoint(100);

	while(componentes != 1){
	    componentes = 0;

	    camera_atualiza(cam);

	    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

	    camera_atualiza(cam);

    	if(!detecta_movimento(r,matriz,cam->quadro)){
		    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

		    black_white_hand(r,p,matriz);

		    abertura(r, matriz, 5);

		    erosao(r, matriz, 1, 3);

		    al_flip_display();

		    reseta_disjoint(d);

		    connected_components(r, matriz, d);

		    componentes = conta_componentes(d);

		    printf("componentes: %d\n", componentes);
    	}
  	}

  atualiza_rastreador(r, matriz);

  libera_disjoint(d);
}

void calibra_dedos(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam){
	p->matiz_dedo_min = 280;
	p->matiz_dedo_max = 340;
	p->saturacao_dedo = 30;

	int componentes = 0;

	disjoint *d = aloca_disjoint(200);

	while(componentes != 5){
		reseta_disjoint(d);

	    componentes = 0;

	    camera_atualiza(cam);

	    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

		black_white_finger(r,p,matriz);

		abertura(r, matriz, 5);

		al_flip_display();

		connected_components(r, matriz, d);

		consiste_massa(r, p, d);

		componentes = conta_componentes(d);

		printf("componentes: %d\n", componentes);
  	}

  	p->massa_min = INT_MAX;

  	for(int i = 0; i < d->n; i++){
  		conjunto *c = d->conjuntos[i];
  		if(c-> massa > 0){
  			if(c->massa < p->massa_min)
  				p->massa_min = c->massa;
  			if(c->massa > p->massa_max)
  				p->massa_max = c->massa;
  		}
  	}

  	libera_disjoint(d);
}

void libera_parametros(parametros *p){
	free(p);
}