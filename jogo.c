#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estado.h"

#define SCALE   50
#define TAM     10
#define MAX_BUFFER 1024

#define PLAYER_IMAGE 0
#define INI_IMAGE 1
#define OBS_IMAGE 2
char * imagens [] = {
  "Minotaur05_DD.png",
  "Orc_03.png",
  "lava_pool1.png"
};

// 15 - 10 = 5

int rnd(int min, int max) {
  int dif = max - min;
  return (rand() % (dif + 1)) + min;
}

ESTADO inicializar () {
  ESTADO e;
  srand(time(NULL));
  //Número de inimigos e de obstaculos
  e.num_inimigos = 10;//rnd(10,20);
  e.num_obstaculos = 20;//rnd(20,40);
  //Conta o numero de inimigos/obstaculos/jogadores que já foram colocados no mapa
  int c_inimigos = 0;
  int c_obstaculos = 0;
  int c_jogador = 0;
  //Coloca as cenas no mapa
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 10; j++){
      //Dá um numero aletorio entre 0 e 99
      int r = rnd(0,99);
      //Numero de cenas
      int n_inimigos = (e.num_inimigos - c_inimigos) * 100;
      int n_obstaculos = (e.num_obstaculos - c_obstaculos) * 100;
      int n_jogador = 1 - c_jogador;
      //Numero de espaços livres
      int n_livres = (100 - i*10 - j);
      //Probabilidade de cenas
      int prob_inimigo = n_inimigos/n_livres;
      int prob_obstaculo = n_obstaculos/n_livres;
      int prob_jogador = n_jogador/n_livres;
      //Calcula se tem um inimigo na posição
      if(r < prob_inimigo){
        e.inimigo[c_inimigos].X = i;
        e.inimigo[c_inimigos].Y = j;
        c_inimigos ++;
        //Calcula se tem obstaculos na posição
      } else if(r - prob_inimigo < prob_obstaculo){
        e.obstaculo[c_obstaculos].X = i;
        e.obstaculo[c_obstaculos].Y = j;
        c_obstaculos ++;
        //Calcula se tem o jogador na posição
      } else if(r < 10){
        e.jog.X = i;
        e.jog.Y = j;
        printf("%d %d\n", i, j);
        c_jogador ++;
      }
    }
  }
  return e;
}

void print_image (int X, int Y, int i){
  char * nome = "/images/%s";
  char ficheiro [MAX_BUFFER];
  sprintf(ficheiro, nome, imagens[i]);
  int tam = SCALE;
  X *= SCALE;
  Y *= SCALE;
  printf("\t<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", X, Y, tam, tam, ficheiro );
}

void print_inimigo(ESTADO e){
  int i;
  for(i = 0; i < e.num_inimigos; i++){
    print_image(e.inimigo[i].X,e.inimigo[i].Y,INI_IMAGE);
  }
}

void print_obstaculo(ESTADO e){
  int i;
  for(i = 0; i < e.num_obstaculos; i++){
    print_image(e.obstaculo[i].X,e.obstaculo[i].Y,OBS_IMAGE);
  }
}

void print_cell (int C, int L, char * COLOR) {
  printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", C * SCALE, L * SCALE, SCALE, SCALE, COLOR);
}

void print_move (int X, int Y, ESTADO e) {
  int livre = 1;
  for(int i = 0; i < e.num_inimigos; i++) {
    if(e.inimigo[i].X == X && e.inimigo[i].Y == Y){
      livre = 0;
    }
  }
  if(livre){
    for(int i = 0; i < e.num_obstaculos; i++) {
      if(e.obstaculo[i].X == X && e.obstaculo[i].Y == Y){
        livre = 0;
      }
    }
  }
  if(X < TAM && X >= 0 && Y < TAM && Y >= 0 && livre) {
    char * COLOR[] = {"blue","cyan"};
    int i = (X + Y) % 2;
    printf("<a href=?%d,%d >\n", X, Y);
    print_cell(X, Y, COLOR[i]);
    printf("</a>\n");
  }
}

int disponivel(int X, int Y, ESTADO e) {
  if(X < TAM && X >= 0 && Y < TAM && Y >= 0){
    return 0;
  }
  for(int i = 0; i < e.num_inimigos; i++) {
    if(e.inimigo[i].X == X && e.inimigo[i].Y == Y){
      return 0;
    }
  }
  for(int i = 0; i < e.num_obstaculos; i++) {
    if(e.obstaculo[i].X == X && e.obstaculo[i].Y == Y){
      return 0;
    }
  }
  return 1;
}

void print_moves (int X, int Y,ESTADO e) {
  POSICAO movimentos[] = {
    { -1, 0 },
    { +1, 0 },
    { 0 ,-1 },
    { 0 ,+1 }
  };
  int moves = 4;
  for(int i = 0; i < moves; i++) {
    print_move (X + movimentos[i].X, Y + movimentos[i].Y,e);
  }
}

void print_player (ESTADO e) {
  int X = e.jog.X;
  int Y = e.jog.Y;
  print_image(X, Y,PLAYER_IMAGE);
  print_moves(X, Y, e);
}

void print_board () {
  int C, L;
  for(L = 0; L < TAM; L++) {
    for(C = 0; C < TAM; C++) {
      char * COLOR[] = {"black","white"};
      int i = (C + L) % 2;
      print_cell(C, L, COLOR[i]);
    }
  }
}

ESTADO ler_inimigos (ESTADO e, char* argumentos){
  int i;
  char resto [MAX_BUFFER];
  sscanf(argumentos, "%d,%s",&e.num_inimigos, resto);
  for(i = 0;i<e.num_inimigos;i++){
    sscanf(resto, "%d,%d,%s", &(e.inimigo[i].X), &(e.inimigo[i].Y), resto);
  }
  return e;
}

ESTADO str2estado (char * argumentos) {
  ESTADO e = inicializar();
  int X = 0;
  int Y = 0;
  char resto[MAX_BUFFER];
  if(sscanf (argumentos, "%d,%d,%s", &X, &Y, resto) > 1){
    e.jog.X = X;
    e.jog.Y = Y;
    e = ler_inimigos(e, resto);
  }
  return e;
}

void parser (char * arguments) {
  ESTADO e = str2estado(arguments);
  print_board ();
  print_player (e);
  print_inimigo(e);
  print_obstaculo(e);
}
