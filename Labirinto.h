#pragma once

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define TAM_MAX 124

#define INICIO 'I'
#define PAREDE '#'
#define DISPONIVEL ' '
#define SAIDA 'S'
#define CAMINHO_CERTO '+'
#define CAMINHO_ERRADO 'x'

typedef struct
{
  int linha;
  int coluna;
} Posicao;

typedef struct
{
  int linhas;
  int colunas;
  char **matriz;
} Labirinto;

Labirinto *carregaLabirinto();
void imprimeLabirinto(Labirinto *labirinto);
Posicao posicaoInicial(Labirinto *labirinto);
int resolveLabirinto(Labirinto *labirinto, int x, int y);