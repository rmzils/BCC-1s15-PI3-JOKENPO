 hsv **h = hsv_aloca(altura, largura);

 while(!contador(r)){
        camera_atualiza(cam);

        copia_quadro(matriz, cam->quadro, altura, largura);

        camera_copia(cam, matriz, esquerda);

        rgb_hsv(r, matriz, h);

        dilatacao_mao(r, h, 1, 5);

        erosao_mao(r, h, 1, 5);

        atualiza_rastreador(r, h);

        al_draw_rectangle(r->w,r->n,r->e,r->s,cor,1);

        al_flip_display();
}

while(detecta_movimento(r, matriz, cam->quadro)){
        copia_quadro(matriz, cam->quadro, r->altura, r->largura);

        camera_copia(cam, matriz, esquerda);

        rgb_hsv(r, matriz, h);

        dilatacao_mao(r, h, 1, 5);

        erosao_mao(r, h, 1, 5);

        atualiza_rastreador(r, cam->quadro);

        al_draw_rectangle(r->w,r->n,r->e,r->s,cor,1);

        camera_copia(cam, cam->quadro, direita);

        camera_atualiza(cam);

        al_flip_display();        
}

rgb_hsv(r, matriz, h);

int jogada = analiza_jogada(r, p, h);
