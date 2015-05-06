#include "connected.h"

void connected_components(rastreador *r, unsigned char ***matriz, disjoint *d){
	pixel matriz_p[r->s][r->e];

	for(int i = r->n; i < r->s; i++){
		for(int  j = r->w; j < r->e; j++){
			if(matriz[i][j][0] == 255){
				int menor;
				conjunto *c = NULL;

				if(j > r->w && matriz[i][j - 1][0] == 255){
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

				if(i > r->n && j > r->w && matriz[i - 1][j - 1][0] == 255){
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

				if(i > r->n && matriz[i - 1][j][0] == 255){
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

				if(i > r->n && j < r->e - 1 && matriz[i - 1][j + 1][0] == 255){
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

	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		if(c != NULL)
			imprime_parentes(c);
	}

	conta_componentes(d);

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
			printf("chamou a funcao para o conjunto %d: ", c->num);
			une_parentes(d, c);	
			printf("\n");
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
				printf(" %d", w->num);
				massa_total += w->massa;
				w->massa = 0;
				w->visitado = 1;
				pilha_insere(p, w);
			}

			topo->a = topo->a->prox;
		}
	}

	c->massa = massa_total;

	pilha_libera(p);

	printf("\n");
}

conjunto *find(pixel *p){
	return p->representante;
}

int conta_componentes(disjoint *d){
	int n = 0;

	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		printf("%d massa: %d\n", c->num, c->massa);
		if(c->massa > 0)
			n++;
	}

	return n;
}

void consiste_massa(rastreador *r, parametros *p, disjoint *d){
	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		if(c->massa < p->massa_min/2)
			c->massa = 0;
	}
}

void imprime_parentes(conjunto *c){
	printf("%d: ",c->num);
	for(arco *a = c->comeco; a != NULL; a = a->prox){
		conjunto *w = a->w;
		printf("%d ", w->num);
	}
	printf("\n");
}