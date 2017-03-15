#define MAX_INIMIGOS 100
#define MAX_OBSTACULOS 100


typedef struct POS {
  int X;
  int Y;
} POSICAO;

typedef struct estado {
  POSICAO jog;
  POSICAO inimigo [ MAX_INIMIGOS ];
  POSICAO obstaculo [ MAX_OBSTACULOS ];
  int num_inimigos;
  int num_obstaculos;
} ESTADO;
