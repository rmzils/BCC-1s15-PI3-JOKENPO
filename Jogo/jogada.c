#include "jogada.h"

void sort(int conjuntos[], int quantidade){
	for(int n = 0; n < quantidade; n++, quantidade--){
		for(int i = 0; i < quantidade - 1; i++){
			if(conjuntos[i + 1] > conjuntos[i]){
				int aux = conjuntos[i];
				conjuntos[i] = conjuntos[i + 1];
				conjuntos[i + 1] = aux;
			}
		}
	}
}

int analiza_jogada(rastreador *r, parametros *p, hsv **h){
	disjoint *d = aloca_disjoint(200);

	erosao_mao(r, p, h, 1, 3);

    dilatacao_mao(r, p, h, 3, 5);

    erosao_dedo(r, p, h, 1, 3);

    dilatacao_dedo(r, p, h, 1, 5);

    erosao_dedo(r, p, h, 2, 5);

	dilatacao_dedo(r, p, h, 2, 5);

	connected_components(r, p, h, d);

	consiste_dedo(d);

	int quantidade = conta_componentes(d);

	int conjuntos[quantidade], i = 0;

	for(int j = 0; j < d->n; j++){
		conjunto *c = d->conjuntos[j];
		if(c->massa > 0){
			conjuntos[i] = c->massa;
			i++;
		}
	}

	free(d);

	int jogada = -1;

	if(i == 1)
		return 0;
	
	if(i == 2){
		sort(conjuntos, quantidade);
		if(conjuntos[0] > 2*conjuntos[1])
			return 1;

		return 3;
	}

	if(i == 3 || i == 4){
		sort(conjuntos, quantidade);

		int media = (conjuntos[0] + conjuntos[2])/2;

		if(conjuntos[1] < media)
			return 2;

		return 4;
	}

	if(i >= 5)
		return 1;

	return -1;
}