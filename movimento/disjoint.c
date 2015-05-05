#include "disjoint.h"

disjoint *aloca_disjoint(int max){
	disjoint *d = malloc(sizeof(disjoint));

	d->conjuntos = malloc(max * sizeof(conjunto *));
	d->max = max;
	d->n = 0;

	return d;
}

conjunto *aloca_conjunto(int num){
	conjunto *c = malloc(sizeof(conjunto));

	c->num = num;
	c->massa = 0;
	c->comeco = NULL;

	return c;
}

void insere_conjunto(disjoint *d){
	if(d->n == d->max)
		return;

	d->conjuntos[d->n] = aloca_conjunto(d->n);

	d->n++;
}

void insere_parente(conjunto *c, conjunto *s){
	arco *a = malloc(sizeof(arco));

	printf("Inseriu conjunto %d no %d\n", s->num, c->num);

	a->w = s;
	a->prox = c->comeco;
	c->comeco = a;
}

int parentesco(conjunto *c, conjunto *s){
	for(arco *a = c->comeco; a != NULL; a = a->prox){
		conjunto *w = a->w;
		if(w == s)
			return 1;
	}

	return 0;
}

void adiciona_massa(conjunto *c){
	c->massa++;
}

void reseta_disjoint(disjoint *d){
	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		while(c->comeco != NULL){
			arco *a = c->comeco;
			c->comeco = c->comeco->prox;
			free(a);
		}
		free(c);
	}
	d->n = 0;
}

void libera_disjoint(disjoint *d){
	for(int i = 0; i < d->n; i++){
		conjunto *c = d->conjuntos[i];
		while(c->comeco != NULL){
			arco *a = c->comeco;
			c->comeco = c->comeco->prox;
			free(a);
		}
		free(c);
	}
	free(d);
}