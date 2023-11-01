#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>

// Define o No
typedef struct ArvoreBNo {
  int *chaves; // Array com as chaves
  int ordem;     // Ordem mínima da arvore
  struct ArvoreBNo **filhos; // Array de filhos
  int NChaves;                       // Numero de chaves atual
  int folha;                    // Se for 1 é uma folha, se 0 não é folha
} ArvoreBNo;


/*
  Regras:
  ordem == grau == t (nos PDFs)
  1. Cada nó pode ter no mínimo grau - 1 chaves e no máximo 2grau - 1 chaves.
  2. Um nó não folha pode ter entre grau e 2grau filhos.
  3. Todas as chaves de um nó são ordenadas.
*/

// Cria um novo nó
ArvoreBNo *criarNo(int ordem, int folha) {
  ArvoreBNo *no = (ArvoreBNo *)malloc(sizeof(ArvoreBNo));

  // define as propriedades do nó
  no->ordem = ordem; // Cada nó pode ter no mínimo ordem - 1 chaves e no máximo 2 * ordem - 1 chaves.
  no->folha = folha;
  no->chaves = (int *)malloc((2 * ordem - 1) * sizeof(int));
  no->filhos = (ArvoreBNo **)malloc(2 * ordem * sizeof(ArvoreBNo *)); // Um nó não folha pode ter entre ordem e 2 * ordem filhos.
  no->NChaves = 0;

  return no;
}

// inserção
void divideNo(ArvoreBNo *no, int i);
void insereEmNoNaoCheio(ArvoreBNo *no, int chave);
void insere(ArvoreBNo **root, int chave, int ordem);
void exibirNos(ArvoreBNo *root);

// remoção
void removeChave(ArvoreBNo *root, int chave, int ordem);
int procurarChave(ArvoreBNo *root, int chave);
void removerChaveFolha(ArvoreBNo *root, int idx);
void removerChave(ArvoreBNo *root, int idx, int ordem);
int getAntecessor(ArvoreBNo *root, int idx, int ordem);
int getSucessor(ArvoreBNo *root, int idx, int ordem);
void preencher(ArvoreBNo *root, int idx, int ordem);
void emprestaDoAnterior(ArvoreBNo *root, int idx, int ordem);
void emprestaDoProximo(ArvoreBNo *root, int idx, int ordem);
void merge(ArvoreBNo *root, int idx, int ordem);

#endif
