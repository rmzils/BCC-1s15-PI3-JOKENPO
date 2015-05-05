#include "imagem.h"

void copia_quadro(unsigned char ***quadro1, unsigned char ***quadro2, int altura, int largura){
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < largura; j++){
			quadro1[i][j][0] = quadro2[i][j][0];
			quadro1[i][j][1] = quadro2[i][j][1];
			quadro1[i][j][2] = quadro2[i][j][2];
		}
	}
}

void black_white_hand(rastreador *r, parametros *p, unsigned char ***matriz){
	int **matiz = malloc(r->s * sizeof(int *));
	int **saturacao = malloc(r->s * sizeof(int *));
	for(int i = 0; i < r->s; i++){
		matiz[i] = malloc(r->e * sizeof(int));
		saturacao[i] = malloc(r->e * sizeof(int));
	}

	rgb_hsv(r, matriz, matiz, saturacao);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){

			if(matiz[i][j] >= 180 && matiz[i][j] <= 240 && saturacao[i][j] >= 50){
				matriz[i][j][0] = 255;
				matriz[i][j][1] = 255;
				matriz[i][j][2] = 255;
			}

			else{
				matriz[i][j][0] = 0;
				matriz[i][j][1] = 0;
				matriz[i][j][2] = 0;
			}
		}	
	}

	for(int i = 0; i < r->s; i++){
		free(matiz[i]);
		free(saturacao[i]);
	}

	free(matiz);
	free(saturacao);
}

void black_white_finger(rastreador *r, parametros *p, unsigned char ***matriz){
	int **matiz = malloc(r->s * sizeof(int *));
	int **saturacao = malloc(r->s * sizeof(int *));
	for(int i = 0; i < r->s; i++){
		matiz[i] = malloc(r->e * sizeof(int));
		saturacao[i] = malloc(r->e * sizeof(int));
	}

	rgb_hsv(r, matriz, matiz, saturacao);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){

			if(matiz[i][j] >= 280 && matiz[i][j] <= 340 && saturacao[i][j] >= 30){
				matriz[i][j][0] = 255;
				matriz[i][j][1] = 255;
				matriz[i][j][2] = 255;
			}

			else{
				matriz[i][j][0] = 0;
				matriz[i][j][1] = 0;
				matriz[i][j][2] = 0;
			}
		}	
	}

	for(int i = 0; i < r->s; i++){
		free(matiz[i]);
		free(saturacao[i]);
	}

	free(matiz);
	free(saturacao);
}

void rgb_hsv(rastreador *rast, unsigned char ***matriz, int **matiz, int **saturacao){
	for(int i = rast->n; i < rast->s; i++){
		for(int j = rast->w; j < rast->e; j++){
			float r = (float) matriz[i][j][0] / 255;
			float g = (float) matriz[i][j][1] / 255;
			float b = (float) matriz[i][j][2] / 255;

			float max, min, delta;

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
					matiz[i][j] = 60*((g - b)/delta);
				}
				else{
					matiz[i][j] = 360 + 60*((g - b)/delta);
				}
			}
			else if(max == g){
				matiz[i][j] = 120 + 60*((b - r)/delta);
			}
			else{
				matiz[i][j] = 240 + 60*((r - g)/delta);
			}

			if(max == 0.0)
				saturacao[i][j] = 0;
			else
				saturacao[i][j] = (delta / max)*100;
		}
	}
}

void abertura(rastreador *r, unsigned char ***matriz, int n){
	for(int i = 0; i < n; i++){
		dilatacao_int(r, matriz);
		erosao_int(r, matriz);
	}
}

void fechamento(rastreador *r, unsigned char ***matriz, int n){
	for(int i = 0; i < n; i++){
		erosao_int(r, matriz);
		dilatacao_int(r, matriz);
	}
}

void dilatacao(rastreador *r, unsigned char ***matriz, int n){
	for(int i = 0; i < n; i++)
		dilatacao_int(r, matriz);
}

void erosao(rastreador *r, unsigned char ***matriz, int n){
	for(int i = 0; i < n; i++)
		erosao_int(r, matriz);
}

void dilatacao_int(rastreador *r, unsigned char ***matriz){
	unsigned char matriz_aux[r->s][r->e];

	for(int i = r->n + 1; i < r->s - 1; i++){
		for(int j = r->w + 1; j < r->e - 1; j++){
			if(matriz[i][j][0] == 0){
				matriz_aux[i][j] = 0;
				if(matriz[i - 1][j - 1][0] == 255 || matriz[i - 1][j][0] == 255 || matriz[i - 1][j + 1][0] == 255 || matriz[i][j - 1][0] == 255 ||
					matriz[i][j - 1][0] == 255 || matriz[i + 1][j - 1][0] == 255 || matriz[i + 1][j][0] == 255 || matriz[i + 1][j + 1][0] == 255 ){
					matriz_aux[i][j] = 255;
				}
			}
			else
				matriz_aux[i][j] = 255;
		}
	}

	for(int i = r->n + 1; i < r->s - 1; i++){
		for(int j = r->w + 1; j < r->e - 1; j++){
			matriz[i][j][0] = matriz_aux[i][j];
			matriz[i][j][1] = matriz_aux[i][j];
			matriz[i][j][2] = matriz_aux[i][j];
		}
	}
}

void erosao_int(rastreador *r, unsigned char ***matriz){
	unsigned char matriz_aux[r->s][r->e];

	for(int i = r->n + 1; i < r->s - 1; i++){
		for(int j = r->w + 1; j < r->e - 1; j++){
			if(matriz[i][j][0] == 255){
				matriz_aux[i][j] = 255;
				if(matriz[i - 1][j - 1][0] == 0 || matriz[i - 1][j][0] == 0 || matriz[i - 1][j + 1][0] == 0 || matriz[i][j - 1][0] == 0 ||
					matriz[i][j - 1][0] == 0 || matriz[i + 1][j - 1][0] == 0 || matriz[i + 1][j][0] == 0 || matriz[i + 1][j + 1][0] == 0 ){
					matriz_aux[i][j] = 0;
				}
			}
			else
				matriz_aux[i][j] = 0;
		}
	}

	for(int i = r->n + 1; i < r->s - 1; i++){
		for(int j = r->w + 1; j < r->e - 1; j++){
			matriz[i][j][0] = matriz_aux[i][j];
			matriz[i][j][1] = matriz_aux[i][j];
			matriz[i][j][2] = matriz_aux[i][j];
		}
	}
}