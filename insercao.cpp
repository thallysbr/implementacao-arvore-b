#include "header.h"
#include <stdio.h>
#include <stdlib.h>

// Função para dividir o filho y deste nó. filhoIndex é o índice de y na array de filhos
void divideNo(ArvoreBNo *no, int filhoIndex) {
  // Cria um novo nó z que é irmão do nó y (que está cheio)
  ArvoreBNo *z = criarNo(no->ordem, no->filhos[filhoIndex]->folha);
  ArvoreBNo *y = no->filhos[filhoIndex];
  z->NChaves = no->ordem - 1;

  // Copia a metade de trás das chaves de y para z
  for (int j = 0; j < no->ordem - 1; j++){
    z->chaves[j] = y->chaves[j + no->ordem];
  }

  // Se y não for uma folha, também copia a metade de trás dos filhos de y para z
  if (!y->folha) {
    for (int j = 0; j < no->ordem; j++)
      z->filhos[j] = y->filhos[j + no->ordem];
  }

  // Agora y tem apenas metade das chaves que tinha antes
  y->NChaves = no->ordem - 1;

  // Abre espaço no nó pai para o novo filho z
  for (int j = no->NChaves; j >= filhoIndex + 1; j--){
    no->filhos[j + 1] = no->filhos[j];
  }

  // Conecta z ao nó pai
  no->filhos[filhoIndex + 1] = z;


  // Move as chaves no nó pai para abrir espaço para a chave de y que vai subir
  for (int j = no->NChaves - 1; j >= filhoIndex; j--){
    no->chaves[j + 1] = no->chaves[j];
  }

  // Move a chave do meio de y para o nó pai
  no->chaves[filhoIndex] = y->chaves[no->ordem - 1];

  // Atualiza a quantidade de chaves no nó pai
  no->NChaves = no->NChaves + 1;
}

// Função para inserir uma nova chave em um no
void insereEmNoNaoCheio(ArvoreBNo *no, int chave) {
  // Começamos do final do nó para encontrar onde a nova chave deve ser inserida
  int i = no->NChaves - 1;

  // Se o nó atual for uma folha, a gente pode inserir a chave diretamente aqui
  if (no->folha) {
    // Encontra onde a nova chave deve ser inserida
    while (i >= 0 && no->chaves[i] > chave) {
      i--;
    }
    
    // Move todas as chaves maiores uma posição para frente para abrir espaço
    int posicaoParaInserir = i + 1;
    for (int j = no->NChaves - 1; j >= posicaoParaInserir; j--) {
      no->chaves[j + 1] = no->chaves[j];
    }

    // Insere a chave na posição encontrada
    no->chaves[i + 1] = chave;
    no->NChaves = no->NChaves + 1;
  } 

  // Se o nó não é uma folha, temos que descer na árvore
  else { 
    // Encontra qual filho vai receber a nova chave
    while (i >= 0 && no->chaves[i] > chave){
      i--;
    }

    // Verifica se o filho encontrado está cheio
    if (no->filhos[i + 1]->NChaves == 2 * no->ordem - 1) {
      // Se o filho está cheio, divide ele
      divideNo(no, i + 1);

      // Depois de dividir, uma chave do filho que foi dividido sobe para o nó atual e o filho dividido vira dois. Precisamos decidir em qual dos dois a nova chave vai
      if (no->chaves[i + 1] < chave){
        i++;
      }
    }
    insereEmNoNaoCheio(no->filhos[i + 1], chave);
  }
}

// Insere a chave na arvore
void insere(ArvoreBNo **raiz, int chave, int ordem) {
  // Se a árvore estiver vazia
  if (*raiz == NULL) {
    // Cria o no
    *raiz = criarNo(ordem, 1);
    // Insere a chave
    (*raiz)->chaves[0] = chave;
    // Atualiza a quantidade de chaves na raiz
    (*raiz)->NChaves = 1;
  }

  // Se a árvore não estiver vazia
  else {
    // Se a raiz estiver cheia a árvore vai cresce em altura
    if ((*raiz)->NChaves == 2 * ordem - 1) {
      // Cria  a nova raiz
      ArvoreBNo *s = criarNo(ordem, 0);

      // Transforma a antiga raiz em filha da nova raiz
      s->filhos[0] = *raiz;

      // Divide a antiga raiz e move uma chave para a nova raiz
      divideNo(s, 0);

      // Decide em qual dos dois filhos da nova raiz a nova chave deve ser inserida
      int i = 0;
      if (s->chaves[0] < chave){
        i++;
      }

      insereEmNoNaoCheio(s->filhos[i], chave);

      // Atualiza a raiz
      *raiz = s;
    } else {
      // Se a raiz não estiver cheia, chama insereEmNoNaoCheio para a raiz
      insereEmNoNaoCheio(*raiz, chave);
    }
  }
}

// Função para exibir todos os nós da árvore
void exibirNos(ArvoreBNo *raiz) {
  if (raiz != NULL) {
    int i;
    for (i = 0; i < raiz->NChaves; i++) {
      // Se o nó atual não é uma folha, percorre o filho antes de imprimir a chave
      if (!raiz->folha){
        exibirNos(raiz->filhos[i]);
      }
      printf(" %d", raiz->chaves[i]);
    }
    // Depois de imprimir todas as chaves do nó, se ele não for uma folha, percorre o último filho
    if (!raiz->folha){
      exibirNos(raiz->filhos[i]);
    }
  }
}