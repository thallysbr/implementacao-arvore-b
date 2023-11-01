#include "insercao.cpp"
#include "remocao.cpp"
#include <stdio.h>
#include <stdlib.h>

int main() {
  ArvoreBNo *raiz = NULL;

  // Define o grau da arvore
  int ordem = 4;

  insere(&raiz, 8, ordem);
  insere(&raiz, 10, ordem);
  insere(&raiz, 9, ordem);
  insere(&raiz, 24, ordem);
  removeChave(raiz, 20, ordem);
  insere(&raiz, 12, ordem);
  insere(&raiz, 30, ordem);
  insere(&raiz, 7, ordem);
  insere(&raiz, 17, ordem);

  printf("A arvore B é:");
  exibirNos(raiz);

  return 0;
}
