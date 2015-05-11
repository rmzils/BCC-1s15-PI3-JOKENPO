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

	dilatacao(r, matriz, 1, 5);

	connected_components(r, matriz, d);

	consiste_massa(r, p, d);

	int quantidade = conta_componentes(d);

	int conjutos[quantidade], i = 0;

	printf("componentes no final:");

	for(int j = 0; j < d->n; j++){
		conjunto *c = d->conjuntos[j];
		if(c->massa > 0){
			printf(" %d massa: %d |", c->num, c->massa);
			conjutos[i] = c->massa;
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
		sort(conjuntos, quantidade);

		if(conjuntos[0] > 2*conjuntos[1])
			return 1;

		return 3;
	}

	if(i == 3){
		sort(conjuntos, quantidade);

		int media = (conjuntos[0] + conjuntos[2])/2;

		if(conjuntos[1] >= meida)
			return 4;

		return 2;
	}

	if(i == 5)
		return 1;

	return -1;
}

void sort(int conjuntos[], int quantidade){
	for(int i = 0; i < quantidade - 1; i++){
		if(conjuntos[i + 1] > conjuntos[i]){
			int aux = conjuntos[i];
			conjuntos[i] = conjuntos[i + 1];
			conjuntos[i + 1] = aux;
		}
	}
}
