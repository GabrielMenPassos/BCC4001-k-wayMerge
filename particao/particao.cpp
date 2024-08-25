#include "particao.h"
#include "../big_file/big_file.h"
#include "../ordenacao/quickSort.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include <inttypes.h>
#include <iostream>

using namespace std;

/**
 * 
 * @param arquivo
 * @param qtdParticoes
 * @return
 */
char **criarParticao(char *arquivo, int qtdParticoes) {
char **nomes = (char**)calloc(qtdParticoes, sizeof(char*));
  

  FILE *arq = fopen(arquivo, "rb");

  int qtdDigitosVersao = 0;
  int auxCount = qtdParticoes;
  while(auxCount > 1) {
    qtdDigitosVersao++;
    auxCount /= 10;
  }

  fseek(arq, 0, SEEK_END);
  int resto = ftell(arq);

  unsigned long int tamanho = ftell(arq)/(sizeof(ITEM_VENDA)*qtdParticoes);
  fseek(arq, 0, SEEK_SET);

  for(int i = 0; i < qtdParticoes; i++) {
    char versao[qtdDigitosVersao];
    char extensao[] = ".dat";

    sprintf(versao, "%d", i+1);

    int x = strlen(versao) + strlen(extensao) + 11;
    char *nome_arquivo = (char*)calloc(x, sizeof(char));

    strcat(nome_arquivo, "particao/p");
    strcat(nome_arquivo, versao);
    strcat(nome_arquivo, extensao);

    nomes[i] = (char*)malloc(strlen(nome_arquivo) + 1);
    memcpy(nomes[i], nome_arquivo, strlen(nome_arquivo) + 1);

    FILE *destino = fopen(nome_arquivo, "wb");

    ITEM_VENDA *aux = (ITEM_VENDA*)calloc(tamanho, sizeof(ITEM_VENDA));

    resto -= tamanho;

    int tam = (resto < 0) ? resto + tamanho : tamanho;

    fread(aux, sizeof(ITEM_VENDA), tam, arq);

    quickSort(aux, 0, tam-1);

    fwrite(aux, sizeof(ITEM_VENDA), tam, destino);

    fclose(destino);
    free(aux);
    free(nome_arquivo);

    if(resto < 0) break;
  }

  fclose(arq);
  return nomes;
}

/**
 * @param arquivo
 */
void teste_ordenacao_particao (char *arquivo) {
  unsigned long int i;
  ITEM_VENDA *itens;

  FILE *arq = fopen(arquivo, "rb");

  fseek(arq, 0, SEEK_END);
  unsigned long int tamanho = ftell(arq)/1024;

  fseek(arq, 0, SEEK_SET);

  itens = (ITEM_VENDA*)calloc(tamanho, sizeof(ITEM_VENDA));

  fread(itens, sizeof(ITEM_VENDA), tamanho, arq);

  for (i = 0; i < tamanho; i++) {
    printf("ID: %" PRIu32 "\n", itens[i].id);
  }

  fclose(arq);
}
