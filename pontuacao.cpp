	void comer (char mapa[17][17], int eixox, int eixoy, bool &bonus, int &cont, int &timer){
		if(mapa[eixoy][eixox] == '0'){
			mapa[eixoy][eixox] = '3';
			cont++;
		}
		else if(mapa[eixoy][eixox] == '2'){
			mapa[eixoy][eixox] = '3';
			cont++;
			bonus = true;
			timer = 0;
		}

	}
