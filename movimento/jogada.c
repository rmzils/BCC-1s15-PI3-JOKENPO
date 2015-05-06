#include "jogada.h"

int analiza_jogada(rastreador *r, parametros *p, unsigned char ***matriz, camera *cam){
	disjoint *d = aloca_disjoint(200);

	camera_atualiza(cam);

    copia_quadro(matriz, cam->quadro, r->altura, r->largura);

    black_white_finger(r, p, matriz);

    camera_atualiza(cam);

    black_white_finger(r, p, cam->quadro);

    merge(r, matriz, cam->quadro);

    camera_atualiza(cam);

    black_white_finger(r, p, cam->quadro);

    merge(r, matriz, cam->quadro);

	abertura(r, matriz, 10);

	erosao(r, matriz, 1, 5);

	connected_components(r, matriz, d);

	consiste_massa(r, p, d);

	int indice_conjutos[d->n], i = 0;

	printf("componentes no final:");

	for(int j = 0; j < d->n; j++){
		conjunto *c = d->conjuntos[j];
		if(c->massa > 0){
			printf(" %d massa: %d |", c->num, c->massa);
			indice_conjutos[i] = c->num;
			i++;
		}
	}

	free(d);

	int jogada = -1;

	printf("\n");

	printf("Numero de componentes: %d\n", i);

	if(i == 1)
		return 0;
	
	if(i == 2){
		for(int j = 0; j < i; j++){
			int massa = d->conjuntos[indice_conjutos[j]]->massa;
			if(massa > 2*p->massa_max)
				return 1;
		}

		return 3;
	}

	if(i == 3){
		for(int j = 0; j < i; j++){
			int massa = d->conjuntos[indice_conjutos[j]]->massa;
			if(massa >= 2*p->massa_min)
				return 4;
		}

		return 2;
	}

	if(i == 5)
		return 1;

	return -1;
}