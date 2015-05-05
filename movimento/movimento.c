#include "movimento.h"

rastreador *aloca_rastreador(int altura, int largura){
	rastreador *r = malloc(sizeof(rastreador));
	r->n = 0;
	r->s = altura - 1;
	r->w = 0;
	r->e = largura - 1;

	r->altura = altura;
	r->largura = largura;

	r->count = 0;

	r->delimitador = altura/2;

	r->flag_descendo = 0;

	return r;
}

void atualiza_delimitador(rastreador *r, int deli){
	r->delimitador = deli;
}

int detecta_movimento(rastreador *r, unsigned char ***quadro1, unsigned char ***quadro2){
	int pixel_diferente = 0;
	for(int i = 0; i < r->altura; i++){
		for(int j = 0; j < r->largura; j++){
			int r = quadro1[i][j][0] - quadro2[i][j][0];
			if(r < 0)
				r *= -1;

			int g = quadro1[i][j][1] - quadro2[i][j][1];
			if(g < 0)
				g *= -1;

			int b = quadro1[i][j][2] - quadro2[i][j][2];
			if(b < 0)
				b *= -1;

			//printf("i: %d j: %d r: %d g: %d b: %d\n",i, j, r, g, b);

			if(r >= 30 && g >= 30 && b >= 30)
				pixel_diferente++;

			if(pixel_diferente > 10000){
				return 1;
	}

		}	
	}

	if(pixel_diferente > 100){
		return 1;
	}

	return 0;
}

void atualiza_rastreador(rastreador *r, unsigned char ***quadro){
	int i,j;
	int north = r->n, south = r->s, west = r->w, east = r->e; 

	for(i = r->n; i < r->s; i++){
		for(j = r->w + 1; j < r->e; j++){
			if(quadro[i][j][0] == 255){
				north = i - 100;
				if(north < 0)
					north = 0;
			}
		}
	}

	if(r->s == r->altura)
		r->s--;

	for(i = r->s; i >= r->n; i--){
		for(j = r->w; j < r->e; j++){
			if(quadro[i][j][0] == 255){
				south = i + 100;
				if(south > r->altura)
					south = r->altura;
			}
		}
	}

	for(j = r->w; j < r->e; j++){
		for(i = r->n; i < r->s; i++){
			if(quadro[i][j][0] == 255){
				west = j - 100;
				if(west < 0)
					west = 0;
			}
		}
	}

	if(r->e == r->largura)
		r->e--;

	for(j = r->e; j >= r->w; j--){
		for(i = r->n; i < r->s; i++){
			if(quadro[i][j][0] == 255){
				east = j + 100;
				if(east > r->largura)
					east = r->largura;
			}
		}
	}

	r->n = north;
	r->s = south;
	r->w = west;
	r->e = east;

	int centro_mao = (r->s - r->n) + r->n;

	if(r->flag_descendo){
		if(centro_mao >= r->delimitador){
			r->count++;
			printf("contador: %d\n", r->count);
			r->flag_descendo = 0;
		}
	}
	else{
		if(centro_mao < r->delimitador)
			r->flag_descendo = 1;
	}
}

int contador(rastreador *r){
	if(r->count == 3){
		r->count = 0;
		return 1;
	}
	return 0;
}

void libera_rastreador(rastreador *r){
	free(r);
}