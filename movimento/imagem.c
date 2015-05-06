#include "imagem.h"

#define PI 3.14

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
			if(matiz[i][j] >= p->matiz_mao_min && matiz[i][j] <= p->matiz_mao_max && saturacao[i][j] >= p->saturacao_mao){
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

			if(matiz[i][j] >= p->matiz_dedo_min && matiz[i][j] <= p->matiz_dedo_max && saturacao[i][j] >= p->saturacao_dedo){
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

void abertura(rastreador *r, unsigned char ***matriz, int k){
	dilatacao_int(r, matriz, k);
	erosao_int(r, matriz, k);
}

void fechamento(rastreador *r, unsigned char ***matriz, int k){
	erosao_int(r, matriz, k);
	dilatacao_int(r, matriz, k);
}

void dilatacao(rastreador *r, unsigned char ***matriz, int n, int k){
	for(int i = 0; i < n; i++)
		dilatacao_int(r, matriz, k);
}

void erosao(rastreador *r, unsigned char ***matriz, int n, int k){
	for(int i = 0; i < n; i++)
		erosao_int(r, matriz, k);
}

void dilatacao_int(rastreador *r, unsigned char ***matriz, int k){
	unsigned char matriz_aux[r->s][r->e];

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			if(matriz[i][j][0] == 0){
				matriz_aux[i][j] = 0;
				for(int x = i - k; (x <= i + k) && (matriz_aux[i][j] == 0); x++){
					for(int y = j - k; y <= j + k; y++){
						if((x >= 0 && x < r->altura) && (y >= 0 && y < r->largura) && matriz[x][y][0] == 255){
							matriz_aux[i][j] = 255;
							break;
						}
					}
				}
			}
			else
				matriz_aux[i][j] = 255;
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			matriz[i][j][0] = matriz_aux[i][j];
			matriz[i][j][1] = matriz_aux[i][j];
			matriz[i][j][2] = matriz_aux[i][j];
		}
	}
}

void erosao_int(rastreador *r, unsigned char ***matriz, int k){
	unsigned char matriz_aux[r->s][r->e];

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			if(matriz[i][j][0] == 255){
				matriz_aux[i][j] = 255;
				for(int x = i - k; (x <= i + k) && (matriz_aux[i][j] == 255); x++){
					for(int y = j - k; y <= j + k; y++){
						if((x >= 0 && x < r->altura) && (y >= 0 && y < r->largura) && matriz[x][y][0] == 0){
							matriz_aux[i][j] = 0;
							break;
						}
					}
				}
			}
			else
				matriz_aux[i][j] = 0;
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			matriz[i][j][0] = matriz_aux[i][j];
			matriz[i][j][1] = matriz_aux[i][j];
			matriz[i][j][2] = matriz_aux[i][j];
		}
	}
}

void blur_simples(rastreador *r, unsigned char ***matriz, int k){
	int matriz_aux[r->s][r->e][3];

	int div_med = (k*k);

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			matriz_aux[i][j][0] = 0;
			matriz_aux[i][j][1] = 0;
			matriz_aux[i][j][2] = 0;
			for(int x = i - k; x <= i + k; x++){
				for(int y = j - k; y <= j + k; y++){
					if((x >= 0 && x < r->altura) && (y >= 0 && y < r->largura)){
						matriz_aux[i][j][0] += matriz[x][y][0];
						matriz_aux[i][j][1] += matriz[x][y][1];
						matriz_aux[i][j][2] += matriz[x][y][2];
					}
				}
			}
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			matriz[i][j][0] = matriz_aux[i][j][0]/div_med;
			matriz[i][j][1] = matriz_aux[i][j][1]/div_med;
			matriz[i][j][2] = matriz_aux[i][j][2]/div_med;
		}
	}
}

void merge(rastreador *r, unsigned char ***matriz, unsigned char ***matriz_aux){
	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			if(matriz[i][j][0] == 0 && matriz_aux[i][j][0] == 255){
				matriz[i][j][0] == 255;
				matriz[i][j][1] == 255;
				matriz[i][j][2] == 255;
			}
		}
	}
}

void fritacao(rastreador *r, unsigned char ***matriz, int k){
	int matriz_aux[r->s][r->e][3];

	int div_med = (k*k);

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			for(int x = i - k; x <= i + k; x++){
				for(int y = j - k; y <= j + k; y++){
					if((x >= 0 && x < r->altura) && (y >= 0 && y < r->largura)){
						matriz_aux[i][j][0] += matriz[x][y][0];
						matriz_aux[i][j][1] += matriz[x][y][1];
						matriz_aux[i][j][2] += matriz[x][y][2];
					}
				}
			}
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			matriz[i][j][0] = matriz_aux[i][j][0]/div_med;
			matriz[i][j][1] = matriz_aux[i][j][1]/div_med;
			matriz[i][j][2] = matriz_aux[i][j][2]/div_med;
		}
	}
}