#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de obstáculos */
#define MAX_OBJ						100

/** \brief O nº de tipos de objetos no jogo */
#define NUM_OBJECTOS					3

/** \brief Indíce do jogador */
#define JOGADOR						0

/** \brief Índice dos obstaculos */
#define INIMIGO						1

/** \brief Índice dos inimigos */
#define OBSTACULO					2

/**
\brief Estrutura que armazena uma posição
*/
typedef struct stats {
	char x;
	char y;
	char hp;
	char mhp;
	char str;
	char def;
	char type;
} STATS;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	/** \brief O nº de objetos */
	char num[NUM_OBJECTOS];
	/** \brief Array com os stats dos objetos */
	STATS obj[NUM_OBJECTOS][MAX_OBJ];
} ESTADO;

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

/**
\brief Função que converte uma string num estado
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);

#endif
