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

hsv **hsv_aloca(int altura, int largura){
	hsv **h = malloc(altura * sizeof(hsv *));

	for(int i = 0; i < altura; i++)
		h[i] = malloc(largura*sizeof(hsv));

	return h;
}

void rgb_hsv(rastreador *rast, unsigned char ***matriz, hsv **h){

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
					h[i][j].matiz = 60*((g - b)/delta);
				}
				else{
					h[i][j].matiz = 360 + 60*((g - b)/delta);
				}
			}
			else if(max == g){
				h[i][j].matiz = 120 + 60*((b - r)/delta);
			}
			else{
				h[i][j].matiz = 240 + 60*((r - g)/delta);
			}

			if(max == 0.0)
				h[i][j].saturacao = 0;
			else
				h[i][j].saturacao = (delta/max)*100;

			h[i][j].iluminacao = max*100;
		}
	}
}

void black_white(rastreador *r, parametros *p, hsv **h, unsigned char ***matriz){
	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			if(h[i][j].matiz >= p->matiz_mao_min && h[i][j].matiz <= p->matiz_mao_max && h[i][j].saturacao >= p->saturacao_mao){
				//printf("matiz: %d saturacao: %d\n", matiz[i][j], saturacao[i][j]);
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
}

void black_white_finger(rastreador *r, parametros *p, hsv **h, unsigned char ***matriz){
	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			if(h[i][j].matiz >= p->matiz_dedo_min && h[i][j].matiz <= p->matiz_dedo_max && h[i][j].saturacao >= p->saturacao_dedo){
				//printf("matiz: %d saturacao: %d\n", matiz[i][j], saturacao[i][j]);
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
}

int pixel_dedo(parametros *p, hsv *h){
	if(h->matiz >= p->matiz_dedo_min && h->matiz <= p->matiz_dedo_max && h->saturacao >= p->saturacao_dedo)
		return 1;
	return 0;
}

int pixel_mao(parametros *p, hsv *h){
	if(h->matiz >= p->matiz_mao_min && h->matiz <= p->matiz_mao_max && h->saturacao >= p->saturacao_mao)
		return 1;
	return 0;
}

void dilatacao_dedo(rastreador *r, parametros *p, hsv **h, int n, int k){
	for(int i = 0; i < n; i++)
		dilatacao_dedo_int(r, p, h, k);
}

void dilatacao_mao(rastreador *r, parametros *p, hsv **h, int n, int k){
	for(int i = 0; i < n; i++)
		dilatacao_mao_int(r, p, h, k);
}

void erosao_mao(rastreador *r, parametros *p, hsv **h, int n, int k){
	for(int i = 0; i < n; i++)
		erosao_mao_int(r, p, h, k);
}

void dilatacao_dedo_int(rastreador *r, parametros *p, hsv **h, int k){
	hsv h_aux[r->s][r->e];

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			h_aux[i][j].matiz = h[i][j].matiz;
			h_aux[i][j].saturacao = h[i][j].saturacao;
			h_aux[i][j].iluminacao = h[i][j].iluminacao;

			if(!pixel_dedo(p, &h[i][j])){
				for(int x = i - k; x <= i + k; x++){
					for(int y = j - k; y <= j + k; y++){
						if((x >= 0 && x < r->s) && (y >= 0 && y < r->e) && pixel_dedo(p, &h[x][y])){
							h_aux[i][j].matiz = h[x][y].matiz;
							h_aux[i][j].saturacao = h[x][y].saturacao;
							h_aux[i][j].iluminacao = h[x][y].iluminacao;
						}
					}
				}
			}
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			h[i][j].matiz = h_aux[i][j].matiz;
			h[i][j].saturacao = h_aux[i][j].saturacao;
			h[i][j].iluminacao = h_aux[i][j].iluminacao;
		}
	}
}

void dilatacao_mao_int(rastreador *r, parametros *p, hsv **h, int k){
	hsv h_aux[r->s][r->e];

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			h_aux[i][j].matiz = h[i][j].matiz;
			h_aux[i][j].saturacao = h[i][j].saturacao;
			h_aux[i][j].iluminacao = h[i][j].iluminacao;

			if(!pixel_mao(p, &h[i][j])){
				for(int x = i - k; x <= i + k; x++){
					for(int y = j - k; y <= j + k; y++){
						if((x >= 0 && x < r->s) && (y >= 0 && y < r->e) && pixel_mao(p, &h[x][y])){
							h_aux[i][j].matiz = h[x][y].matiz;
							h_aux[i][j].saturacao = h[x][y].saturacao;
							h_aux[i][j].iluminacao = h[x][y].iluminacao;
						}
					}
				}
			}
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			h[i][j].matiz = h_aux[i][j].matiz;
			h[i][j].saturacao = h_aux[i][j].saturacao;
			h[i][j].iluminacao = h_aux[i][j].iluminacao;
		}
	}
}

void erosao_mao_int(rastreador *r, parametros *p, hsv **h, int k){
	hsv h_aux[r->s][r->e];

	k = (float)(((k - 1)/2)+0.5);

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			h_aux[i][j].matiz = h[i][j].matiz;
			h_aux[i][j].saturacao = h[i][j].saturacao;
			h_aux[i][j].iluminacao = h[i][j].iluminacao;

			if(pixel_mao(p, &h[i][j])){
				for(int x = i - k; x <= i + k; x++){
					for(int y = j - k; y <= j + k; y++){
						if((x >= 0 && x < r->s) && (y >= 0 && y < r->e) && !pixel_mao(p, &h[x][y])){
							h_aux[i][j].matiz = 0;
							h_aux[i][j].saturacao = 0;
							h_aux[i][j].iluminacao = 0;
						}
					}
				}
			}
		}
	}

	for(int i = r->n; i < r->s; i++){
		for(int j = r->w; j < r->e; j++){
			h[i][j].matiz = h_aux[i][j].matiz;
			h[i][j].saturacao = h_aux[i][j].saturacao;
			h[i][j].iluminacao = h_aux[i][j].iluminacao;
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

void hsv_libera(rastreador *r, hsv **h){
	for(int i = 0; i < r->s; i++){
		free(h[i]);
	}

	free(h);
}