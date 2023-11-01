#include "header.h"
#include <stdio.h>
#include <stdlib.h>

// Função para remover uma chave da subárvore que tem esse nó como raiz
void removeChave(ArvoreBNo *raiz, int chave, int ordem) {
  // Encontra o indice da chave que queremos remover neste nó, ou o índice do filho que deve levar para o caminho certo para encontrar a chave.
  int idx = procurarChave(raiz, chave);

  // Chave a ser removida está neste nó
  if (idx < raiz->NChaves && raiz->chaves[idx] == chave) {
    if (raiz->folha) {
      // A chave a ser removida está neste nó e este nó é uma folha, então remove a chave
      removerChaveFolha(raiz, idx);
    } else {
      // A chave a ser removida está presente em um nó não-folha
      removerChave(raiz, idx, ordem);
    }
  } else {
    // Se este nó for uma folha, então a chave não está presente na árvore
    if (raiz->folha) {
      printf("A chave %d não foi encontrada na arvore\n", chave);
      return;
    }

    // A chave a ser removida está presente na subárvore cuja raiz é este nó
    // A flag com o ternário define se a chave está presente na subárvore cuja raiz é o último filho do nó
    int flag = ((idx == raiz->NChaves) ? 1 : 0);

    // Se o filho onde a chave deveria estar tem menos de ordem/grau de total de chaves preenche ele
    if (raiz->filhos[idx]->NChaves < ordem) {
      preencher(raiz, idx, ordem);
    }

    // Se o último filho foi merged nós chamamos a função recursivamente para o filho (idx-1)
    if (flag && idx > raiz->NChaves) {
      removeChave(raiz->filhos[idx - 1], chave, ordem);
    }
    // Se não chamamos a função para o filho (idx) que agora tem pelo menos ordem/grau de total de chaves
    else {
      removeChave(raiz->filhos[idx], chave, ordem);
    }
  }
}

// Função para achar o indice da primeira chave que é maior ou igual a chave que estamos procurando
int procurarChave(ArvoreBNo *raiz, int chave) {
  int idx = 0;
  // Enquanto não chegamos no final das chaves do nó e a chave no índice atual é menor do que a chave que estamos procurando
  while (idx < raiz->NChaves && raiz->chaves[idx] < chave) {
    ++idx;
  }
  // Quando sai do while, idx é a posição da primeira chave que é maior ou igual a chave que estamos procurando (ou é igual ao número de chaves no nó, caso todas sejam menores)
  return idx;
}

// Função para remover a chave que está na posição idx deste nó  (nó folha)
void removerChaveFolha(ArvoreBNo *raiz, int idx) {
  // Percorre as chaves do nó a partir de idx + 1, jogando cada uma delas uma posição para trás (preenche o espaço da chave que queremos remover)
  for (int i = idx + 1; i < raiz->NChaves; ++i) {
    raiz->chaves[i - 1] = raiz->chaves[i];
  }
  // Atualiza a quantidade de chaves que temos nesse nó
  raiz->NChaves = raiz->NChaves - 1;
}

// Função para remover uma chave da subárvore que tem esse nó como raiz (que não é folha)
void removerChave(ArvoreBNo *raiz, int idx, int ordem) {
  int chave = raiz->chaves[idx];

  // Se o filho que vem antes de "chave" (filhos[idx]) tem pelo menos "ordem" de chaves
  if (raiz->filhos[idx]->NChaves >= ordem) {
    // Encontra o antecessor
    int antecessor = getAntecessor(raiz, idx, ordem);

    // Substitui chave pelo antecessor
    raiz->chaves[idx] = antecessor;

    // Remove o antecessor recursivamente
    removeChave(raiz->filhos[idx], antecessor, ordem);
  }
  // Se o filho antecessor não tem chaves suficientes, mas o próximo filho tem
  else if (raiz->filhos[idx + 1]->NChaves >= ordem) {
    // Encontra o sucessor
    int sucessor = getSucessor(raiz, idx, ordem);

    // Substitui chave pelo sucessor
    raiz->chaves[idx] = sucessor;

    // Remove o sucessor com a recursividade
    removeChave(raiz->filhos[idx + 1], sucessor, ordem);
  }
  // Se ambos os filhos têm menos do que a "ordem/grau" do total de chaves,
  else {
    // Faz merge
    merge(raiz, idx, ordem);
    // Remove a chave recursivamente
    removeChave(raiz->filhos[idx], chave, ordem);
  }
}

// Função para pegar o antecessor
int getAntecessor(ArvoreBNo *raiz, int idx, int ordem) {
  // Inicializa 'cur' com o filho na posição 'idx' do nó 'raiz'
  ArvoreBNo *cur = raiz->filhos[idx];

  // Navega até a folha mais à direita para encontrar o maior valor menor que a chave na posição 'idx'
  while (!cur->folha) {
    cur = cur->filhos[cur->NChaves];
  }

  // Retorna o último valor da folha, que é o antecessor da chave na posição 'idx'
  return cur->chaves[cur->NChaves - 1];
}

// Função para pegar o sucessor da chave na posição idx
int getSucessor(ArvoreBNo *raiz, int idx, int ordem) {
  // Começa do nó filho que está imediatamente à direita da chave que estamos olhando (raiz->filhos[idx+1])
  ArvoreBNo *cur = raiz->filhos[idx + 1];

  // Desce a árvore para a esquerda até chegar em um nó folha
  while (!cur->folha) {
    cur = cur->filhos[0];
  }
  // Retorna a primeira chave do nó folha
  return cur->chaves[0];
}

// Função para preencher o filho na posição idx, que tem menos do que ordem-1 chaves
void preencher(ArvoreBNo *raiz, int idx, int ordem) {
  // Se o filho anterior tem mais do que ordem-1 chaves, a gente pega emprestado uma chave desse filho
  if (idx != 0 && raiz->filhos[idx - 1]->NChaves >= ordem) {
    emprestaDoAnterior(raiz, idx, ordem);
  }
  // Se o filho seguinte tem mais do que ordem-1 chaves, a gente também pega uma chave emprestada desse filho
  else if (idx != raiz->NChaves && raiz->filhos[idx + 1]->NChaves >= ordem) {
    emprestaDoProximo(raiz, idx, ordem);
  }
  // Se nenhum dos irmãos tem chaves suficientes para emprestar, a gente tem dar merge no filho em questão com um dos irmãos
  else {
    // Se o filho não é o último junta com o próximo irmão
    if (idx != raiz->NChaves) {
      merge(raiz, idx, ordem);
    }
    // Se o filho é o último junta com o irmão anterior
    else {
      merge(raiz, idx - 1, ordem);
    }
  }
}

// Função para pegar uma chave emprestada do filho e colocar no filho que precisa de uma chave extra
void emprestaDoAnterior(ArvoreBNo *raiz, int idx, int ordem) {
  // filho é o nó que precisa de uma chave extra e irmao é o nó do qual vamos pegar a chave emprestada
  ArvoreBNo *filho = raiz->filhos[idx];
  ArvoreBNo *irmao = raiz->filhos[idx - 1];

  // Precisamos abrir espaço no 'filho' para a nova chave, então empurramos todas as chaves uma posição para frente
  for (int i = filho->NChaves - 1; i >= 0; --i) {
    filho->chaves[i + 1] = filho->chaves[i];
  }

  // Se o filho não é uma folha empurra os ponteiros para os filhos uma posição para frente
  if (!filho->folha) {
    for (int i = filho->NChaves; i >= 0; --i) {
      filho->filhos[i + 1] = filho->filhos[i];
    }
  }

  // Agora colocamos a última chave do irmao na primeira posição do filho
  filho->chaves[0] = raiz->chaves[idx - 1];

  // Se o filho não é uma folha, pegamos o último filho do irmao e colocamos como primeiro filho do filho
  if (!raiz->folha) {
    filho->filhos[0] = irmao->filhos[irmao->NChaves];
  }

  // Atualiza a chave na posição idx-1 da raiz, colocando a última chave do irmao ali
  raiz->chaves[idx - 1] = irmao->chaves[irmao->NChaves - 1];

  // Aumenta o número de chaves no filho e diminui no irmao
  filho->NChaves = filho->NChaves + 1;
  irmao->NChaves = irmao->NChaves - 1;
}

// Função para pegar uma chave do proximo filho e colocar no filho anterior
void emprestaDoProximo(ArvoreBNo *raiz, int idx, int ordem) {
  ArvoreBNo *filho = raiz->filhos[idx];
  ArvoreBNo *irmao = raiz->filhos[idx + 1];

  // Insere a chave na posição idx da raiz como a última chave no filho
  filho->chaves[(filho->NChaves)] = raiz->chaves[idx];

  // Se o filho não é uma folha, pega o primeiro filho do irmão e colocamos como último filho do filho
  if (!(filho->folha)) {
    filho->filhos[(filho->NChaves) + 1] = irmao->filhos[0];
  }

  // Insere a primeira chave do irmão na raiz na posição idx
  raiz->chaves[idx] = irmao->chaves[0];

  // Move todas as chaves no irmão uma posição para trás
  for (int i = 1; i < irmao->NChaves; ++i) {
    irmao->chaves[i - 1] = irmao->chaves[i];
  }

  // Se o irmão não é uma folha, move os ponteiros para os filhos uma posição para trás
  if (!irmao->folha) {
    for (int i = 1; i <= irmao->NChaves; ++i) {
      irmao->filhos[i - 1] = irmao->filhos[i];
    }
  }

  // Aumenta o número de chaves no filho e diminuímos no irmão
  filho->NChaves = filho->NChaves + 1;
  irmao->NChaves = irmao->NChaves - 1;
}


// Função para juntar o filho[idx] com o filho[idx+1]
void merge(ArvoreBNo *raiz, int idx, int ordem) {
  ArvoreBNo *filho = raiz->filhos[idx];
  ArvoreBNo *irmao = raiz->filhos[idx + 1];

  // Puxa uma chave do nó atual e insere na posição (ordem-1) do filho
  filho->chaves[ordem - 1] = raiz->chaves[idx];

  // Copia as chaves do irmão para o final do filho
  for (int i = 0; i < irmao->NChaves; ++i) {
    filho->chaves[i + ordem] = irmao->chaves[i];
  }

  // Se o filho não é uma folha, copia os ponteiros para os filhos do irmão para o filho
  if (!filho->folha) {
    for (int i = 0; i <= irmao->NChaves; ++i) {
      filho->filhos[i + ordem] = irmao->filhos[i];
    }
  }

  // Move todas as chaves após idx no nó atual uma posição para trás para preencher o espaço criado ao mover a chave[idx] para o filho
  for (int i = idx + 1; i < raiz->NChaves; ++i) {
    raiz->chaves[i - 1] = raiz->chaves[i];
  }

  // Move os ponteiros para os filhos após (idx+1) no nó atual uma posição para trás
  for (int i = idx + 2; i <= raiz->NChaves; ++i) {
    raiz->filhos[i - 1] = raiz->filhos[i];
  }

  // Atualiza a quantidade de chaves no filho e no nó atual
  filho->NChaves += irmao->NChaves + 1;
  raiz->NChaves--;
}