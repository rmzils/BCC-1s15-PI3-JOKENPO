#include "connected.h"

void connected_components(rastreador *r, parametros *p, hsv **h, disjoint *d){
	pixel matriz_p[r->s][r->e];

	for(int i = r->n; i < r->s; i++){
		for(int  j = r->w; j < r->e; j++){
			if(pixel_dedo(p, &h[i][j])){
				int menor;
				conjunto *c = NULL;

				if(j > r->w && pixel_dedo(p, &h[i][j -1])){
					if(c == NULL){
						c = find(&matriz_p[i][j - 1]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i][j - 1]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								c = find(&matriz_p[i][j - 1]);
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(i > r->n && j > r->w && pixel_dedo(p, &h[i - 1][j - 1])){
					if(c == NULL){
						c = find(&matriz_p[i - 1][j - 1]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i - 1][j - 1]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(i > r->n && pixel_dedo(p, &h[i - 1][j])){
					if(c == NULL){
						c = find(&matriz_p[i - 1][j]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i - 1][j]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(i > r->n && j < r->e - 1 && pixel_dedo(p, &h[i - 1][j + 1])){
					if(c == NULL){
						c = find(&matriz_p[i - 1][j + 1]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i - 1][j + 1]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(c == NULL){
					menor = d->n;
					insere_conjunto(d);
				}
					
				adiciona_massa(d->conjuntos[menor]);

				matriz_p[i][j].representante = d->conjuntos[menor];

				for(int x = i - 1; x <= i + 1; x++){
					for(int y = j - 1; y <= j + 1; y++){
						if((x >= 0 && y >= 0) && (x < r->s && y < r->e) && pixel_mao(p, &h[x][y])){
							if(!d->conjuntos[menor]->dedo)
								d->conjuntos[menor]->dedo = 1;
						}
					}
				}
			}
		}
	}

	une_conjuntos(d);
}

void connected_components_mao(rastreador *r, parametros *p, hsv **h, disjoint *d){
	pixel matriz_p[r->s][r->e];

	for(int i = r->n; i < r->s; i++){
		for(int  j = r->w; j < r->e; j++){
			if(pixel_mao(p, &h[i][j])){
				int menor;
				conjunto *c = NULL;

				if(j > r->w && pixel_mao(p, &h[i][j -1])){
					if(c == NULL){
						c = find(&matriz_p[i][j - 1]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i][j - 1]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								c = find(&matriz_p[i][j - 1]);
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(i > r->n && j > r->w && pixel_mao(p, &h[i - 1][j - 1])){
					if(c == NULL){
						c = find(&matriz_p[i - 1][j - 1]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i - 1][j - 1]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(i > r->n && pixel_mao(p, &h[i - 1][j])){
					if(c == NULL){
						c = find(&matriz_p[i - 1][j]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i - 1][j]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(i > r->n && j < r->e - 1 && pixel_mao(p, &h[i - 1][j + 1])){
					if(c == NULL){
						c = find(&matriz_p[i - 1][j + 1]);
						menor = c->num;
					}
					else{
						c = find(&matriz_p[i - 1][j + 1]);
						if(d->conjuntos[menor] != c){
							if(c->num < menor){
								if(!parentesco(c,d->conjuntos[menor]))
									insere_parente(c,d->conjuntos[menor]);
								menor = c->num;
							}
							else{
								if(!parentesco(d->conjuntos[menor],c))
									insere_parente(d->conjuntos[menor],c);
							}
						}
					}
				}

				if(c == NULL){
					menor = d->n;
					insere_conjunto(d);
				}
					
				adiciona_massa(d->conjuntos[menor]);

				matriz_p[i][j].representante = d->conjuntos[menor];
			}
		}
	}

	une_conjuntos(d);
}

void une_conjuntos(disjoint *d){
	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		c->visitado = 0;
		c->a = c->comeco;
	}

	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		if(c->visitado == 0){
			une_parentes(d, c);	
		}
	}
}

void une_parentes(disjoint *d, conjunto *c){
	pilha *p = pilha_aloca();

	c->visitado = 1;
	pilha_insere(p,c);

	int massa_total = c->massa;

	while(!pilha_vazia(p)){
		conjunto *topo = pilha_topo(p);

		if(topo->a == NULL){
			pilha_retira(p);

		}
		else{
			conjunto *w = topo->a->w;

			if(w->visitado == 0){
				massa_total += w->massa;
				w->massa = 0;

				if(w->dedo)
					c->dedo = 1;

				w->visitado = 1;
				pilha_insere(p, w);
			}

			topo->a = topo->a->prox;
		}
	}

	c->massa = massa_total;

	pilha_libera(p);
}

conjunto *find(pixel *p){
	return p->representante;
}

int conta_componentes(disjoint *d){
	int n = 0;

	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		if(c->massa > 0)
			n++;
	}

	return n;
}

void consiste_dedo(disjoint *d){
	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		if(!c->dedo || c->massa < 200)
			c->massa = 0;
	}
}