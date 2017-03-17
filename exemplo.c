#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM						11
#define ESCALA				50

int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < TAM && y < TAM;
}

int posicao_igual(STATS p, int x, int y){
	return p.x == x && p.y == y;
}

int tem_objeto(ESTADO e,int x, int y){
	int tipo;
	int i;
	for(tipo = 0; tipo < NUM_OBJECTOS; tipo++){
		for(i = 0; i < e.num[tipo]; i++){
			if(posicao_igual(e.obj[tipo][i], x, y))
				return 1;
		}
	}
	return 0;
}

int posicao_ocupada(ESTADO e, int x, int y){
	return tem_objeto(e, x, y) && posicao_valida(x, y);
}

void imprime_casa(int x, int y) {
	int i = 1 + random() % 4;
	QUADRADO_BG(x, y, ESCALA, i);
}

void imprime_mov(int x, int y) {
	QUADRADO(x, y, ESCALA, "transparent");
}

ESTADO inicializar_objeto(ESTADO e,int tipo){
	int X;
	int Y;
	do {
		X = random() % TAM;
		Y = random() % TAM;
	} while (posicao_ocupada(e, X, Y));
	e.obj[tipo][(int) e.num[tipo]].x = X;
	e.obj[tipo][(int) e.num[tipo]].y = Y;
	e.num[tipo]++;
	return e;
}

ESTADO inicializar_objetos(ESTADO e,int num,int tipo){
	int i;
	for(i = 0; i < num; i++){
		e = inicializar_objeto(e, tipo);
	}
	return e;
}

ESTADO inicializar() {
	ESTADO e = {{0}};
	e.num[JOGADOR] = 1;
	e.obj[JOGADOR][0].x = 5;
	e.obj[JOGADOR][0].y = 5;
	e = inicializar_objetos (e, 20, INIMIGO);
	e = inicializar_objetos (e, 25, OBSTACULO);
	return e;
}

void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo = e;
	int x = e.obj[JOGADOR][0].x + dx;
	int y = e.obj[JOGADOR][0].y + dy;
	char link[MAX_BUFFER];
	if(!posicao_valida(x, y))
		return;
	if(posicao_ocupada(e, x, y))
		return;

	novo.obj[JOGADOR][0].x = x;
	novo.obj[JOGADOR][0].y = y;
	sprintf(link, "http://localhost/cgi-bin/exemplo?%s", estado2str(novo));
	ABRIR_LINK(link);
	imprime_mov(x, y);
	FECHAR_LINK;
}

void imprime_movimentos(ESTADO e) {
	int x;
	int y;
	for(x = -1; x <= 1; x++){
		for(y = -1; y <= 1; y++){
			if(x || y){
				imprime_movimento(e, x, y);
			}
		}
	}
}

ESTADO ler_estado(char *args) {
	if(strlen(args) == 0)
		return inicializar();
	return str2estado(args);
}

void imprime_objetos(ESTADO e,int tipo) {
	int i;
	char* img[NUM_OBJECTOS][20];
	int img_length[NUM_OBJECTOS] = {1,1,3};
	img[JOGADOR][0] = "Minotaur05_DD.png";
	img[INIMIGO][0] =  "Driders_04.png";
	img[OBSTACULO][0] = "barrel.png";
	img[OBSTACULO][1] = "box1.png";
	img[OBSTACULO][2] = "box2.png";
	for(i = 0; i < e.num[tipo]; i ++) {
			IMAGEM(e.obj[tipo][i].x, e.obj[tipo][i].y, ESCALA, img[tipo][random() % img_length[tipo]]);
	}
}

int main() {
	int x, y;
	srandom(time(NULL));
	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(600, 600);
	srandom(1);
	for(y = 0; y < TAM; y++)
		for(x = 0; x < TAM; x++)
			imprime_casa(x, y);

	for(int tipo = 0; tipo < NUM_OBJECTOS; tipo++){
		imprime_objetos(e, tipo);
	}
	imprime_movimentos(e);

	FECHAR_SVG;

	return 0;
}
