#include "calibragem.h"

parametros *aloca_parametros(){
	parametros *p = malloc(sizeof(parametros));

	p->matiz_dedo_min = 300;
	p->matiz_dedo_max = 320;
	p->saturacao_dedo = 50;
	p->matiz_mao_min = 180;
	p->matiz_mao_max = 240;
	p->saturacao_mao = 30;
	p->massa_min = 100;
	p->k = 5;

	return p;
}

/*void calibragem(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam){
	//calibra_dedos(p, r, matriz, cam);
	//calibra_mao(p, r, matriz, cam);
}

void calibra_mao(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam){
	p->matiz_mao_min = 180;
	p->matiz_mao_max = 240;
	p->saturacao_mao = 0;

	int contador = 0;

	while(contador < 5){
		camera_atualiza(cam);

	    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

	    camera_atualiza(cam);

	    int media_matiz;

	    int media_antiga = media_matiz;

	    media_matiz = 0;

    	if(!detecta_movimento(r,matriz,cam->quadro)){
		    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

			int quantidade = 0;

			for(int i = r->n; i < r->s; i++){
		    	for(int j = r->w; j < r->e; j++){
		    		float r = (float) cam->quadro[i][j][0] / 255;
					float g = (float) cam->quadro[i][j][1] / 255;
					float b = (float) cam->quadro[i][j][2] / 255;

		    		float max, min, delta;

		    		int matiz;

					if(r > b)
						max = r;
					else
						max = b;

					if(g > max)
						max = g;

					if(r < b)
						min = r;
					else
						min = b;

					if(g < min)
						min = g;

					delta = max - min;

					if(max == r){
						if(g >= b){
							matiz = 60*((g - b)/delta);
						}
						else{
							matiz = 360 + 60*((g - b)/delta);
						}
					}
					else if(max == g){
						matiz = 120 + 60*((b - r)/delta);
					}
					else{
						matiz = 240 + 60*((r - g)/delta);
					}

					if(matiz >= p->matiz_mao_min && matiz <= p->matiz_mao_max){
						media_matiz += matiz;
						quantidade++;
					}
		    	}
		    	
		    }

		    media_matiz /= quantidade;

		    printf("%d\n", media_matiz);

		    p->matiz_mao_min = media_matiz - 10;
		    p->matiz_mao_max = media_matiz + 10;

		    int diferenca = media_matiz - media_antiga;

		    if(diferenca < 0)
		    	diferenca *= -1;

		    if(diferenca < 5)
		    	contador++;
		    else
		    	contador = 0;
		}
	}

	printf("matiz mao min: %d, matiz mao max: %d\n", p->matiz_mao_min, p->matiz_mao_max);

	contador = 0;

	disjoint *d = aloca_disjoint(500);

	printf("passou aqui\n");

	while(contador < 5){
	    camera_atualiza(cam);

	    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

	    camera_atualiza(cam);

    	if(!detecta_movimento(r,matriz,cam->quadro)){
		    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

		    black_white_hand(r,p,matriz);

		    abertura(r, matriz, 10);

        	erosao(r, matriz, 1, 3);

        	dilatacao(r, matriz, 2, 10);

		    reseta_disjoint(d);

		    connected_components(r, matriz, d);

		    int componentes = conta_componentes(d);

		    printf("componentes: %d\n", componentes);

		    if(componentes == 1)
		    	contador++;
		    else{
		    	contador = 0;
		    	if(componentes > 1)
		    		p->saturacao_mao += 5;
		    	else if(componentes == 0)
		    		p->saturacao_mao -= 5;
		    }
    	}
  	}

  	atualiza_rastreador(r, matriz);

  	libera_disjoint(d);
}

void calibra_dedos(parametros *p, rastreador *r, unsigned char ***matriz, camera *cam){
	p->matiz_dedo_min = 280;
	p->matiz_dedo_max = 340;
	p->saturacao_dedo = 30;

	int contador = 0;

	/*while(contador < 5){
		camera_atualiza(cam);

	    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

	    camera_atualiza(cam);

	    int media_matiz;

	    int media_antiga = media_matiz;

	    media_matiz = 0;

    	if(!detecta_movimento(r,matriz,cam->quadro)){
		    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

			int quantidade = 0;

			for(int i = r->n; i < r->s; i++){
		    	for(int j = r->w; j < r->e; j++){
		    		float r = (float) cam->quadro[i][j][0] / 255;
					float g = (float) cam->quadro[i][j][1] / 255;
					float b = (float) cam->quadro[i][j][2] / 255;

		    		float max, min, delta;

		    		int matiz;

					if(r > b)
						max = r;
					else
						max = b;

					if(g > max)
						max = g;

					if(r < b)
						min = r;
					else
						min = b;

					if(g < min)
						min = g;

					delta = max - min;

					if(max == r){
						if(g >= b){
							matiz = 60*((g - b)/delta);
						}
						else{
							matiz = 360 + 60*((g - b)/delta);
						}
					}
					else if(max == g){
						matiz = 120 + 60*((b - r)/delta);
					}
					else{
						matiz = 240 + 60*((r - g)/delta);
					}

					if(matiz >= p->matiz_dedo_min && matiz <= p->matiz_dedo_max){
						media_matiz += matiz;
						quantidade++;
					}
		    	}
		    }

		    media_matiz /= quantidade;

		    printf("media_matiz: %d\n", media_matiz);

		    p->matiz_dedo_min = media_matiz - 20;
		    p->matiz_dedo_max = media_matiz + 20;

		    int diferenca = media_matiz - media_antiga;

		    if(diferenca < 0)
		    	diferenca *= -1;

		    if(diferenca < 5)
		    	contador++;
		    else
		    	contador = 0;
		}
	}

	printf("matiz dedo min: %d, matiz dedo max: %d\n", p->matiz_dedo_min, p->matiz_dedo_max);

	contador = 0;

	disjoint *d = aloca_disjoint(200);

	while(contador < 5){
		reseta_disjoint(d);

	    camera_atualiza(cam);

	    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

        black_white_finger(r, p, matriz);

        camera_atualiza(cam);

        black_white_finger(r, p, cam->quadro);

        merge(r, matriz, cam->quadro);

        camera_atualiza(cam);

        black_white_finger(r, p, cam->quadro);

        merge(r, matriz, cam->quadro);

		erosao(r, matriz, 1, 5);

		dilatacao(r, matriz, 1, 10);

		connected_components(r, matriz, d);

		consiste_massa(r, p, d);

		int componentes = conta_componentes(d);

		printf("componentes: %d\n", componentes);

		if(componentes == 5)
			contador++;
		else{
			contador = 0;
			if(componentes > 5){
				if(p->saturacao_dedo < 100)
					p->saturacao_dedo += 5;
				printf("saturacao: %d\n", p->saturacao_dedo);
			}
			else if(componentes < 5){
				if(p->saturacao_dedo > 0)
					p->saturacao_dedo -= 5;
			} 
		}
  	}

  	int quantidade = conta_componentes(d);

  	int conjuntos[quantidade], i = 0;

  	for(int j = 0; j < d->n; j++){
		conjunto *c = d->conjuntos[j];
		if(c->massa > 0){
			printf(" %d massa: %d |", c->num, c->massa);
			conjuntos[i] = c->massa;
			i++;
		}
	}

  	libera_disjoint(d);

  	for(int k = 0; k < quantidade; quantidade--){
		for(int j = 0; j < quantidade - 1; j++){
			if(conjuntos[j] > conjuntos[j + 1]){
				int aux = conjuntos[i];
				conjuntos[i] = conjuntos[i + 1];
				conjuntos[i + 1] = aux;
			}
		}
	}

	p->k = sqrt(conjuntos[0])/4;

	printf("k: %d\n", p->k);
}*/

void libera_parametros(parametros *p){
	free(p);
}