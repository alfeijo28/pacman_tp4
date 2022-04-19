#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include "ghosts.h"
#include "pontuacao.h"

using namespace std;

//*INICIALIZANDO AS VARIÁVEIS E O ALLEGRO*//

ALLEGRO_DISPLAY *game = NULL; //ALLEGRO_DISPLAY é um tipo de variável que guarda uma janela a ser desenhada
ALLEGRO_EVENT_QUEUE *event_queue = NULL; //declarando a fila de enventos 
ALLEGRO_BITMAP  *start   = NULL; // variável que vai recber uma imagem
ALLEGRO_BITMAP  *map   = NULL; // variável que vai recber uma imagem
ALLEGRO_BITMAP  *lost   = NULL; // variável que vai recber uma imagem
ALLEGRO_BITMAP *won = NULL;
ALLEGRO_TIMER *timer = NULL; //declarando o temporizador do jogo
ALLEGRO_FONT *font = NULL;
ALLEGRO_AUDIO_STREAM  *loserSong   = NULL; // variável que vai recber uma imagem
ALLEGRO_AUDIO_STREAM  *winnerSong  = NULL; // variável que vai recber uma imagem
ALLEGRO_BITMAP *avatar_d = NULL; // variável que vai receber um personagem escolhido
ALLEGRO_BITMAP *avatar_e = NULL; // variável que vai receber um personagem escolhido
ALLEGRO_BITMAP *avatar_c = NULL; // variável que vai receber um personagem escolhido
ALLEGRO_BITMAP *avatar_b = NULL; // variável que vai receber um personagem escolhido 
ALLEGRO_BITMAP *bolinha = NULL; // variável que vai receber a imagem do pacman com a boca fechada
ALLEGRO_BITMAP *bolinha_b = NULL; // variável que vai receber a imagem do pacman com a boca fechada
ALLEGRO_BITMAP *bolinha_c = NULL; // variável que vai receber a imagem do pacman com a boca fechada
ALLEGRO_AUDIO_STREAM *songSugar = NULL;
ALLEGRO_AUDIO_STREAM *opening = NULL;
ALLEGRO_BITMAP *frutinha = NULL;
ALLEGRO_BITMAP *bonus = NULL; 

//*FIM DA INICIALIZAÇÃO DAS VARIÁVEIS DO ALLEGRO*//


//*VARIÁVEIS DO GAME*//

const int tela_alt = 255; // constantes com o tamanho da tela do jogo
const int tela_lar = 240;
const float FPS = 7;// será usada no timer
const int pacman_size=15; //tamanho dos personagens do jogo
int move_cont = 0;
bool mov_img[] = {false, false, false, false};
bool redraw = true;
bool killer = false;
bool is_open = true;
int c=15; //tamanho da célula do mapa

//*FIM DAS VARIÁVEIS DO GAME*//



//*VARIÁVEIS DO PACMAN*//

int eixo_x = 7*c; // Variável da posição do personagem no eixo x
int eixo_y = 9*c; // Variável da posição do personagem no eixo y
int i=9; // Posição inicial do personagem na matriz
int j=7; // Posição inicial do personagem na matriz
bool teclas[4] = { false, false, false, false }; // Vetor que vai atribuir true às setinhas que o usuário precionar
bool proximo[4] = {false, false, false, false}; // Vetor q vai guardar com true o próximo movimento do personagem
bool intencao[4] = {false, false, false, false};
int kill_timer = 0; // Tempo com o bônus
int pontuacao = 0; // Pontuação do Pacman
bool win = false; // Vitória

//*FIM DAS VARIÁVEIS DO PACMAN*//



//*VARIÁVEIS DOS FANTASMAS*//

ALLEGRO_BITMAP *ghost1   = NULL;
ALLEGRO_BITMAP *ghost2   = NULL;
ALLEGRO_BITMAP *ghost3   = NULL;
ALLEGRO_BITMAP *ghost4   = NULL;

unsigned char gposy_1 = 105;
unsigned char gposx_1 = 90;
bool gmovement_1[] = {false, false, false, false};
bool started_1 = false;

unsigned char gposy_2 = 105;
unsigned char gposx_2 = 105;
bool gmovement_2[] = {false, false, false, false};
bool started_2 = false;

unsigned char gposy_3 = 105;
unsigned char gposx_3 = 120;
bool gmovement_3[] = {false, false, false, false};
bool started_3 = false;

unsigned char gposy_4 = 105;,


unsigned char gposx_4 = 135;
bool gmovement_4[] = {false, false, false, false};
bool started_4 = false;

//*FIM DAS VARIÁVEIS DOS FANTASMAS*//


//matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor
char MAPA[17][17] ={
   "1111111111111111",
   "1000000110002001",
   "1011110000111101",
   "1011110110111101",
   "1000000110000001",
   "1011110110111101",
   "1210000000000101",
   "1010101111010101",
   "1010101111010101",
   "1010100300010101",
   "1010101111010101",
   "1000000000002001",
   "1011101100011101",
   "1000101001010001",
   "1010100211010101",
   "1000000000000001",
   "1111111111111111"
};

void set_mov(bool [], char);
void set_false(bool []);

// Inicializa o conteúdo do jogo: imagens, fonte, sons etc.
int inicializa() {
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    if(!al_install_audio()){
       cout << "Falha ao inicializar o audio" << endl;
        return 0;      
   }

    if(!al_init_acodec_addon()){
        cout << "Falha ao inicializar o acodec" << endl;
        return 0;      
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }

    game = al_create_display(tela_lar, tela_alt);
    if(!game)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    songSugar = al_load_audio_stream("sugar-honey-honey-8bit.wav", 4, 1024);
    if(!songSugar){
        cout << "Falha ao inicializar a musica" << endl;
        al_destroy_audio_stream(songSugar);
        return 0;
    }

    opening = al_load_audio_stream("opening-song.wav", 4, 1024);
    if(!opening){
        cout << "Falha ao inicializar a musica" << endl;
        al_destroy_audio_stream(opening);
        return 0;
    }

    loserSong = al_load_audio_stream("death-sound.wav", 4, 1024);
    if(!loserSong){
        cout << "Falha ao inicializar a musica" << endl;
        al_destroy_audio_stream(loserSong);
        return 0;
    }

    winnerSong = al_load_audio_stream("victorious-song.wav", 4, 1024);
    if(!winnerSong){
        cout << "Falha ao inicializar a musica" << endl;
        al_destroy_audio_stream(winnerSong);
        return 0;
    }

    al_reserve_samples(4);

    al_init_font_addon();

      // Inicialização do add-on para uso de fontes True Type
      if (!al_init_ttf_addon())
      {
        cout << "Falha ao inicializar add-on allegro_ttf.\n";
        return -1;
      }

      // Carregando o arquivo de fonte
      font = al_load_font("Squarely.TTF", 17, 0);
      if (!font)
      {
        al_destroy_display(game);
        cout << "Falha ao carregar fonte.\n";
        return -1;
      }

       start= al_load_bitmap("start.bmp");
       map= al_load_bitmap("map.bmp");
       lost= al_load_bitmap("game-over.bmp");
       won=al_load_bitmap("final.bmp");



        avatar_d = al_load_bitmap("pac_d.tga");    // Cria um bitmap para o personagem
        if(!avatar_d){
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        avatar_c = al_load_bitmap("pac_c.tga");;     //cria um bitmap para o personagem
        if(!avatar_c){
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        avatar_e = al_load_bitmap("pac_e.tga");;     //cria um bitmap para o personagem
        if(!avatar_e)
        {
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        avatar_b = al_load_bitmap("pac_b.tga");;     //cria um bitmap para o personagem
        if(!avatar_b){
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        bolinha = al_load_bitmap("bolinha.tga");;     //cria um bitmap para o personagem
        if(!bolinha){
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        bolinha_c = al_load_bitmap("bolinha_c.tga");;     //cria um bitmap para o personagem
        if(!bolinha_c){
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        bolinha_b = al_load_bitmap("bolinha_b.tga");;     //cria um bitmap para o personagem
        if(!bolinha_b){
            cout << "Falha ao carregar imagem..." << endl;
            al_destroy_display(game);
            return -1;
        }

        frutinha = al_load_bitmap("dot.tga");;     //cria um bitmap para o personagem
        if(!frutinha){
            cout << "Nao achei a frutinha";
            al_destroy_display(game);
            return -1;
        }

        bonus = al_load_bitmap("doce.tga");;     //cria um bitmap para o personagem
        if(!bonus){
            cout << "Nao achei o bonus";
            al_destroy_display(game);
            return -1;
        }

        event_queue = al_create_event_queue();
        if(!event_queue)
        {
            cout << "Falha ao criar a fila de eventos" << endl;
            al_destroy_display(game);
            al_destroy_timer(timer);
            return 0;
        }

    al_draw_bitmap(start,0,0,0);   // Desenhado a imagem na tela
    al_flip_display();             // Mostrando a tela já desenhada com as informações iniciais do jogo

    al_register_event_source(event_queue, al_get_display_event_source(game));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    al_attach_audio_stream_to_mixer(opening, al_get_default_mixer());
    al_set_audio_stream_playing(opening, true);

    // Tela de Início do jogo
    while(1){
    ALLEGRO_EVENT ev0;
    al_wait_for_event(event_queue, &ev0);

    if(ev0.type == ALLEGRO_EVENT_KEY_DOWN){
        if(ev0.keyboard.keycode==ALLEGRO_KEY_SPACE){
            al_draw_bitmap(map,0,0,0);                // Desenhado a imagem do mapa na tela
            al_draw_bitmap(avatar_d,eixo_x,eixo_y,0); // Desenhado o personagem no centro da tela
            al_flip_display();                        // Mostrando a tela onde o jogo ocorrerá
            break;      
        }
        else if(ev0.keyboard.keycode==ALLEGRO_KEY_ESCAPE)return 0;
    }

    if(ev0.type == ALLEGRO_EVENT_DISPLAY_CLOSE)return 0;
   }


    return 1;
}

int main(int argc, char **argv){

  if(!inicializa()) return -1;

  srand (time(NULL)); 

  al_destroy_audio_stream(opening);
  al_attach_audio_stream_to_mixer(songSugar, al_get_default_mixer());
  al_set_audio_stream_playing(songSugar, true);

   // Loop do jogo.
   while(1){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        // Verifica vitória
        if(pontuacao == 129){
          win = true;
          break;
        }

        // A cada timer..
        if(ev.type == ALLEGRO_EVENT_TIMER){

          ghosts_move(gposx_1, gposy_1, gposx_2, gposy_2, gposx_3, gposy_3, gposx_4, gposy_4, gmovement_1, gmovement_2, gmovement_3, gmovement_4, 
            started_1, started_2, started_3, started_4, i, j); // Movimenta fantasmas

          // Teste se o pacman morreu
          if(!killer && killed(gposx_1, gposy_1, gposx_2, gposy_2, gposx_3, gposy_3, gposx_4, gposy_4, eixo_x, eixo_y))
            break;
          if(killer)
            kill(gposx_1, gposy_1, gposx_2, gposy_2, gposx_3, gposy_3, gposx_4, gposy_4, eixo_x, eixo_y, started_1, started_2, started_3, started_4);

          if(intencao[0] && MAPA[i-1][j] != '1'){
            mov_img[0] = true;
            mov_img[1] = false;
            mov_img[2] = false;
            mov_img[3] = false;

            intencao[0] = false;
            set_mov(teclas, 'U');
            i--;
            eixo_y=i*c;
            move_cont++;
         }

         else if(intencao[1] && MAPA[i+1][j] != '1'){
            mov_img[1] = true;
            mov_img[0] = false;
            mov_img[2] = false;
            mov_img[3] = false;

            intencao[1] = false;
            set_mov(teclas, 'D');
            i++;
            eixo_y=i*c;
            move_cont++;
         }
           
         else if(intencao[2] && MAPA[i][j-1] != '1'){
            mov_img[2] = true;
            mov_img[0] = false;
            mov_img[1] = false;
            mov_img[3] = false;

            intencao[2] = false;
            set_mov(teclas, 'L');
            j--;
            eixo_x=j*c;
            move_cont++;
          }
         
         else if(intencao[3] && MAPA[i][j+1] != '1'){
            mov_img[3] = true;
            mov_img[2] = false;
            mov_img[1] = false;
            mov_img[0] = false;

            intencao[3] = false;
            set_mov(teclas, 'R');
            j++;
            eixo_x=j*c;
            move_cont++;
          }

         else if(teclas[0] && MAPA[i-1][j] != '1'){
            mov_img[0] = true;
            mov_img[1] = false;
            mov_img[2] = false;
            mov_img[3] = false;

            i--;
            eixo_y=i*c;
            move_cont++;
         }

         else if(teclas[1] && MAPA[i+1][j] != '1'){
            mov_img[1] = true;
            mov_img[0] = false;
            mov_img[2] = false;
            mov_img[3] = false;

            i++;
            eixo_y=i*c;
            move_cont++;
         }
           
         else if(teclas[2] && MAPA[i][j-1] != '1'){
            mov_img[2] = true;
            mov_img[0] = false;
            mov_img[1] = false;
            mov_img[3] = false;

            j--;
            eixo_x=j*c;
            move_cont++;
          }
         
         else if(teclas[3] && MAPA[i][j+1] != '1'){
            mov_img[3] = true;
            mov_img[2] = false;
            mov_img[1] = false;
            mov_img[0] = false;

            j++;
            eixo_x=j*c;
            move_cont++;
          }

          redraw = true;

          comer (MAPA, eixo_x/15, eixo_y/15, killer, pontuacao, kill_timer);

          if(!killer && killed(gposx_1, gposy_1, gposx_2, gposy_2, gposx_3, gposy_3, gposx_4, gposy_4, eixo_x, eixo_y))
            break;
          if(killer)
            kill(gposx_1, gposy_1, gposx_2, gposy_2, gposx_3, gposy_3, gposx_4, gposy_4, eixo_x, eixo_y, started_1, started_2, started_3, started_4);
        }

        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            is_open = false;
            break;
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    if(MAPA[i-1][j] != '1'){
                        set_mov(teclas, 'U');
                        set_false(intencao);
                    }
                    else
                        set_mov(intencao, 'U');

                    break;

                case ALLEGRO_KEY_DOWN:
                    if(MAPA[i+1][j] != '1'){
                        set_mov(teclas, 'D');
                        set_false(intencao);
                      }
                    else 
                        set_mov(intencao, 'D');  

                    break;

                case ALLEGRO_KEY_LEFT:
                    if(MAPA[i][j-1] != '1'){
                        set_mov(teclas, 'L');
                        set_false(intencao);
                    }
                    else 
                        set_mov(intencao, 'L');  

                    break;

                case ALLEGRO_KEY_RIGHT:
                    if(MAPA[i][j+1] != '1'){
                        set_mov(teclas, 'R');
                        set_false(intencao);
                    }
                    else 
                        set_mov(intencao, 'R');

                    break;

                case ALLEGRO_KEY_ESCAPE:
                    return 0;
            }
        }


        if(redraw && al_is_event_queue_empty(event_queue)){
            redraw = false;

            al_draw_bitmap(map,0,0,0);
            for (int i=0; i<17; i++)
                for(int j=0; j<16; j++) {
                    if(MAPA[i][j]=='0')
                        al_draw_bitmap(frutinha, j*15, i*15, 0);

                    else if (MAPA[i][j] == '2')
                        al_draw_bitmap(bonus, j*15, i*15, 0);
                }


            if(move_cont%2 == 0){
                if(mov_img[0] == true) al_draw_bitmap(bolinha_c,eixo_x,eixo_y,0); //redesenhado o personagem 
                else if(mov_img[1] == true) al_draw_bitmap(bolinha_b,eixo_x,eixo_y,0); //redesenhado o personagem 
                else al_draw_bitmap(bolinha,eixo_x,eixo_y,0); //redesenhado o personagem 
            }
            else{
                if(mov_img[0] == true) al_draw_bitmap(avatar_c,eixo_x,eixo_y,0); //redesenhado o personagem 
                else if(mov_img[1] == true) al_draw_bitmap(avatar_b,eixo_x,eixo_y,0); //redesenhado o personagem 
                else if(mov_img[2] == true) al_draw_bitmap(avatar_e,eixo_x,eixo_y,0); //redesenhado o personagem 
                else  al_draw_bitmap(avatar_d,eixo_x,eixo_y,0); //redesenhado o personagem 
            }           

          char img[] = "brocolis1.tga";

          looking(gmovement_1, img, killer);
          ghost1 = al_load_bitmap(img);
          looking(gmovement_2, img, killer);
          ghost2 = al_load_bitmap(img);
          looking(gmovement_3, img, killer);
          ghost3 = al_load_bitmap(img);    
          looking(gmovement_4, img, killer);
          ghost4 = al_load_bitmap(img);

          al_draw_bitmap(ghost1, gposx_1, gposy_1,0);
          al_draw_bitmap(ghost2, gposx_2, gposy_2,0);
          al_draw_bitmap(ghost3, gposx_3, gposy_3,0);
          al_draw_bitmap(ghost4, gposx_4, gposy_4,0);

          al_draw_textf(font, al_map_rgb(82, 119, 158), tela_lar/2, 0, ALLEGRO_ALIGN_CENTRE, "%02d", pontuacao);

          al_flip_display();

          if(killer) kill_timer++;
          if(kill_timer == 25){
            kill_timer = 0;
            killer = false;
          }
        }
   }

    al_destroy_audio_stream(songSugar);

    if(win){
      al_attach_audio_stream_to_mixer(winnerSong, al_get_default_mixer());
      al_set_audio_stream_playing(winnerSong, true);
    }

    else{
      al_attach_audio_stream_to_mixer(loserSong, al_get_default_mixer());
      al_set_audio_stream_playing(loserSong, true);
    }

    while(!win && is_open){
      al_draw_bitmap(lost,0,0,0); //desenhado a imagem do mapa na tela
      al_flip_display();

      ALLEGRO_EVENT ev2;
      al_wait_for_event(event_queue, &ev2);

      if(ev2.type == ALLEGRO_EVENT_KEY_DOWN){
          if(ev2.keyboard.keycode==ALLEGRO_KEY_SPACE){
              break;      
          }
          else if(ev2.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            break;
      }
      if(ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        break;
   }

    while(win && is_open){
      al_draw_bitmap(won,0,0,0); //desenhado a imagem do mapa na tela
      al_flip_display();

      ALLEGRO_EVENT ev2;
      al_wait_for_event(event_queue, &ev2);

      if(ev2.type == ALLEGRO_EVENT_KEY_DOWN){
          if(ev2.keyboard.keycode==ALLEGRO_KEY_SPACE){
              break;      
          }
          else if(ev2.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
            break;
      }
      if(ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        break;
   }
   
   al_destroy_bitmap(start); //liberando o espaço da imagem que não será mais usada

  //*--------------------------------------*//

    


   //*TELA QUE FINALIZA O JOGO*//


   al_destroy_bitmap(map); //liberando o espaço da imagem que não será mais usada
   al_destroy_event_queue(event_queue); //limpando o evento
   al_destroy_bitmap(avatar_d); //liberando espaço do bitmap do personagem
   al_destroy_display(game);  //liberando o espaço da tela na memória

   //*--------------------------------------*//

  return 0;
}

void set_false(bool movs[]){
  movs[0] = false;
  movs[1] = false;
  movs[2] = false;
  movs[3] = false;  
}

void set_mov(bool movs[], char dir){
    switch(dir){
        case 'U':
          movs[0] = true;
          movs[1] = false;
          movs[2] = false;
          movs[3] = false;
          break;

        case 'D':
          movs[0] = false;
          movs[1] = true;
          movs[2] = false;
          movs[3] = false;
          break;

        case 'L':
          movs[0] = false;
          movs[1] = false;
          movs[2] = true;
          movs[3] = false;
          break;

        case 'R':
          movs[0] = false;
          movs[1] = false;
          movs[2] = false;
          movs[3] = true;
          break;
    }
}
