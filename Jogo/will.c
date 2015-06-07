void erosao(camera *cam, unsigned char ***matri, int k, int n){
	for(int i = 0; i < n; i++)
		erosao_int(cam, matriz, k);
}

void erosao_int(camera *cam, unsigned char ***matriz, int k){
	unsigned char matriz_aux[cam->altura][cam->largura][3];

	for(int i = 0; i < cam->altura; i++){
		for(int j = 0; j < cam->largura; j++){
			matriz_aux[i][j][0] = matriz[i][j][0];
			matriz_aux[i][j][1] = matriz[i][j][1];
			matriz_aux[i][j][2] = matriz[i][j][2];

    		if(matriz[i][j][0] == 255){
    			int achou = 0;

    			for(int x = i - k; x <= i + k && !achou; x++){
					for(int y = j - k; y <= j + k; y++){
						if((x >= 0 && x < cam->altura) && (y >= 0 && y < cam->largura) && matriz[i][j][0] = 0){
							matriz_aux[i][j][0] = 0;
							matriz_aux[i][j][1] = 0;
							matriz_aux[i][j][2] = 0;
							achou = 1;
							break;
    					}
    				}
    			}
    		}
    	}
	}

	for(int i = 0; i < cam->altura; i++){
		for(int j = 0; j < cam->largura; j++){
			matriz[i][j][0] = matri[i][j]_aux[0];
			matriz[i][j][1] = matri[i][j]_aux[1];
			matriz[i][j][2] = matri[i][j]_aux[2];
		}
	}	
}