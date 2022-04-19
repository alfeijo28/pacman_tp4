#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;

unsigned char ENEMIES_MAP[17][17] ={
	"1111111111111111",
	"1200002112000021",
	"1011112002111101",
	"1011110110111101",
	"1200002112000021",
	"1011110110111101",
	"1012022002202101",
	"1010103553010101",
	"1010103333010101",
	"1010100000010101",
	"1010101111010101",
	"1202020020202021",
	"1011101100011101",
	"1202101221012021",
	"1010100011010101",
	"1202020200202021",
	"1111111111111111"
};

// Verifica se é possível tomar aquela direção para seguir, evitando de voltar pelo caminho oposto
// (e.g. estar indo para a direita e escolher seguir para a esquerda)
bool possible(bool movs[], bool intention[], unsigned char n){
	unsigned char oposite; 

	switch(n){
		case 0:
			oposite = 2;
			break;
		case 1:
			oposite = 3;
			break;
		case 2: 
			oposite = 0;
			break;
		case 3:
			oposite = 1;
			break;
	}

	if(movs[(short int)oposite] == intention[(short int)n]) return false; // Se estiver escolhido o oposto, retorna falso.

	return true;
}

// Movimenta os fantasmas
void move(unsigned char &posx, unsigned char &posy, bool movs[], bool &started){
	short int x, y;
	x = posx/15;
	y = posy/15;

	// Se o caminho estiver disponível e o fantasma já estiver fora da região inicial.
	if(ENEMIES_MAP[y][x] == '0' && started == true){
		if(movs[0] == true) posy -= 15;
		else if(movs[1] == true) posx += 15;
		else if(movs[2] == true) posy += 15;
		else if(movs[3] == true) posx -= 15;
	}

	// Caso encontre uma esquina ou ainda esteja na região inicial
	else{
		bool possible_movs[4] = {false, false, false, false};
		unsigned char randChoice;

		// Verifica quais caminhos podem ser opções.
		if(ENEMIES_MAP[y][x] == '2'){
				if(ENEMIES_MAP[y-1][x] == '0' || ENEMIES_MAP[y-1][x] == '2')
					possible_movs[0] = true;

				if(ENEMIES_MAP[y][x+1] == '0' || ENEMIES_MAP[y][x+1] == '2')
					possible_movs[1] = true;

				if(ENEMIES_MAP[y+1][x] == '0' || ENEMIES_MAP[y+1][x] == '2')
					possible_movs[2] = true;

				if(ENEMIES_MAP[y][x-1] == '0' || ENEMIES_MAP[y][x-1] == '2')
					possible_movs[3] = true;


			// Sorteia valores até que atinja um possível
			do{
				randChoice = (unsigned char)rand()%4;
			}while(possible_movs[(short int)randChoice] == false || !possible(movs, possible_movs, randChoice));
			// OBS: Terá que sortear um novo caso não possa seguir pela direção e/ou escolha o oposto do caminho atual.

			// Atualiza posição a partir da escolha aleatória.
			switch(randChoice){
				case 0:
					posy -= 15;
					movs[0] = true;
					movs[1] = false;
					movs[2] = false;
					movs[3] = false;				
					break;
				case 1:
					posx += 15;
					movs[0] = false;
					movs[1] = true;
					movs[2] = false;
					movs[3] = false;
					break;
				case 2:
					posy += 15;
					movs[0] = false;
					movs[1] = false;
					movs[2] = true;
					movs[3] = false;
					break;
				case 3:
					posx -= 15;
					movs[0] = false;
					movs[1] = false;
					movs[2] = false;
					movs[3] = true;
					break;
				}	
		}

		// Os fantasmas estarão nas posições '3' e '5' se ainda estiverem na região inicial e, assim, sguirão
		// uma trajetória pré-definida.
		else if (ENEMIES_MAP[y][x] == '3'){
			if(ENEMIES_MAP[y][x+1] == '5')
				posx += 15;
			else if(ENEMIES_MAP[y][x-1] == '5')
				posx -= 15;
		}

		else if(ENEMIES_MAP[y][x] == '5') posy -= 15;
		else if(x == 8 && y == 6){
			posx += 15;
			started = true;
		}
		else if(x == 7 && y == 6){
			posx -= 15;
			started = true;
		}
	}
}

// Define qual imagem escolherá para desenhar a partir da direção do fantasmas.
void looking(bool movement[], char img[], bool killer){
	if(!killer){
		if(movement[0] == true) strcpy(img, "brocolis2.tga");
		else if(movement[1] == true) strcpy(img, "brocolis1.tga");
		else if(movement[2] == true) strcpy(img, "brocolis4.tga");
		else if(movement[3] == true) strcpy(img, "brocolis3.tga");
	}
	else{
		if(movement[0] == true) strcpy(img, "cupcake2.tga");
		else if(movement[1] == true) strcpy(img, "cupcake1.tga");
		else if(movement[2] == true) strcpy(img, "cupcake4.tga");
		else if(movement[3] == true) strcpy(img, "cupcake3.tga");
	}	
}

// Movimenta todos os fantasmas.
void ghosts_move(unsigned char &posx_1, unsigned char &posy_1, unsigned char &posx_2, unsigned char &posy_2, unsigned char &posx_3, unsigned char &posy_3, 
	unsigned char &posx_4, unsigned char &posy_4, bool movement_1[], bool movement_2[], bool movement_3[], bool movement_4[], bool &started_1, 
	bool &started_2, bool &started_3, bool &started_4, int px, int py){

	move(posx_1, posy_1, movement_1, started_1);
	move(posx_2, posy_2, movement_2, started_2);
	move(posx_3, posy_3, movement_3, started_3);
	move(posx_4, posy_4, movement_4, started_4);
}

// Verifica se o pacman morreu para um dos fantasmas.
bool killed(unsigned char posx_1, unsigned char posy_1, unsigned char posx_2, unsigned char posy_2, unsigned char posx_3, unsigned char posy_3, 
	unsigned char posx_4, unsigned char posy_4, int x, int y){

	if(x == posx_1 && y == posy_1) return true;
	if(x == posx_2 && y == posy_2) return true;
	if(x == posx_3 && y == posy_3) return true;
	if(x == posx_4 && y == posy_4) return true;

	return false;
}

// Verifica se um fantasma morreu para o pacman com bônus.
void kill(unsigned char &posx_1, unsigned char &posy_1, unsigned char &posx_2, unsigned char &posy_2, unsigned char &posx_3, unsigned char &posy_3, 
	unsigned char &posx_4, unsigned char &posy_4, int x, int y, bool &started_1, bool &started_2, bool &started_3, bool &started_4){

	if(x == posx_1 && y == posy_1){
		posy_1 = 105;
		posx_1 = 90;
		started_1 = false;
	}

	if(x == posx_2 && y == posy_2){
		posy_2 = 105;
		posx_2 = 105;
		started_2 = false;
	}

	if(x == posx_3 && y == posy_3) {
		posy_3 = 105;
		posx_3 = 120;
		started_3 = false;
	}

	if(x == posx_4 && y == posy_4) {
		posy_4 = 105;
		posx_4 = 135;
		started_4 = false;
	}
}


