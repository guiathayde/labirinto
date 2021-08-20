#include "Labirinto.h"

Labirinto *carregaLabirinto()
{
  char caminhoArquivo[TAM_MAX];
  printf("Digite o caminho para o arquivo do labirinto: ");
  scanf("%s", &caminhoArquivo);
  FILE *arquivo = fopen(caminhoArquivo, "r");

  if (arquivo == NULL)
    printf("Arquivo nao encontrado");

  int linhas, colunas;
  fscanf(arquivo, "%d", &linhas);
  fscanf(arquivo, "%d", &colunas);

  Labirinto *labirinto = (Labirinto *)malloc(sizeof(Labirinto));
  labirinto->linhas = linhas;
  labirinto->colunas = colunas;

  labirinto->matriz = (char **)malloc(linhas * sizeof(char *));
  for (int i = 0; i < linhas; i++)
    labirinto->matriz[i] = (char *)malloc((colunas - 1) * sizeof(char));

  for (int y = 0; y < linhas; y++)
  {
    fgetc(arquivo); // pegar o \n
    for (int x = 0; x < colunas; x++)
      fscanf(arquivo, "%c", &(labirinto->matriz[y][x]));
  }
  fclose(arquivo);

  return labirinto;
}

void imprimeLabirinto(Labirinto *labirinto)
{
  for (int y = 0; y < labirinto->linhas; y++)
  {
    for (int x = 0; x < labirinto->colunas; x++)
    {
      if (labirinto->matriz[y][x] == CAMINHO_CERTO)
        printf("\033[1;32m");
      else if (labirinto->matriz[y][x] == CAMINHO_ERRADO)
        printf("\033[1;31m");
      printf("%c", labirinto->matriz[y][x]);
      printf("\033[0m");
    }
    printf("\n");
  }
  printf("\n");
}

// Funcao que retorna a posicao inicial do labirinto
// Se nao encontra o simbolo inicial retorna x e y = -1
// Se caso tiver dois simbolos iniciais retorna x e y = -2
Posicao posicaoInicial(Labirinto *labirinto)
{
  int achou = 0;
  Posicao p;
  for (int y = 0; y < labirinto->linhas; y++)
  {
    for (int x = 0; x < labirinto->colunas; x++)
    {
      if (labirinto->matriz[y][x] == INICIO)
      {
        if (achou > 0)
        {
          p.linha = -2;
          p.coluna = -2;
          return p;
        }
        achou = 1;
        p.linha = y;
        p.coluna = x;
      }
    }
  }
  if (!achou)
  {
    p.linha = -1;
    p.coluna = -1;
  }
  return p;
}

// Funcao recursiva para resolver o labirinto, utilizando backtracking
int resolveLabirinto(Labirinto *labirinto, int x, int y)
{
  // Verificar se a posicao eh valida
  if (x < 0 || x > labirinto->colunas - 1 || y < 0 || y > labirinto->linhas - 1)
    return FALSE;

  // Verificar se achou a saida
  if (labirinto->matriz[y][x] == SAIDA)
    return TRUE;

  // Verificar se eh uma posicao disponivel para andar
  if (labirinto->matriz[y][x] != DISPONIVEL && labirinto->matriz[y][x] != INICIO)
    return FALSE;

  // Marca a posicao como caminho para saida
  if (labirinto->matriz[y][x] != INICIO)
    labirinto->matriz[y][x] = CAMINHO_CERTO;

  // Verifica qual camimho seguir como parte do caminho da solucao em todas as direcoes
  if (resolveLabirinto(labirinto, x, y - 1)) // cima
    return TRUE;

  if (resolveLabirinto(labirinto, x + 1, y)) // direita
    return TRUE;

  if (resolveLabirinto(labirinto, x, y + 1)) // baixo
    return TRUE;

  if (resolveLabirinto(labirinto, x - 1, y)) // esquerda
    return TRUE;

  // Se nenhum dos movimentos acima funcionar, backtrack. Retira a posicao como parte do caminho da solucao
  if (labirinto->matriz[y][x] != INICIO)
    labirinto->matriz[y][x] = CAMINHO_ERRADO;

  return FALSE;
}