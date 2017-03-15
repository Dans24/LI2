#include <stdio.h>
#include <stdlib.h>
#include "estado.h"
//#include "jogo.h"

void print_header() {
  printf("content-type: text/html\n\n");
  printf("<svg width=600 height=600>\n");
}

void print_footer() {
  printf("</svg>\n");
}

int main() {
  print_header();
  parser(getenv("QUERY_STRING"));
  print_footer();
}
