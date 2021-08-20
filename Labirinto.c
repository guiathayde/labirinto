#include <stdio.h>

#define FALSE 0
#define TRUE 1

#define INICIO 'I'
#define PAREDE '#'
#define DISPONIVEL ' '
#define SAIDA 'S'
#define CAMINHO_CERTO '+'
#define CAMINHO_ERRADO 'x'

#define N_LINHAS 10
#define N_COLUNAS 20

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
void imprimeLabirinto(int linhas, int colunas, char labirinto[linhas][colunas]);
Posicao posicaoInicial(int linhas, int colunas, char labirinto[linhas][colunas]);
int resolveLabirinto(int linhas, int colunas, char labirinto[linhas][colunas], int x, int y);

int main()
{
  // char labirinto[N_LINHAS][N_COLUNAS] =
  //     {
  //         "####################",
  //         "#I              ## #",
  //         "# ### ######## ### #",
  //         "# ###    ####      #",
  //         "##### ## ###########",
  //         "#     ## ###     ###",
  //         "########     ### ###",
  //         "################ ###",
  //         "S                ###",
  //         "####################"};

  Labirinto *labirinto = carregaLabirinto();

  imprimeLabirinto(N_LINHAS, N_COLUNAS, labirinto);

  Posicao inicio = posicaoInicial(N_LINHAS, N_COLUNAS, labirinto);

  if (inicio.coluna == -1 && inicio.linha == -1)
    printf("Error: Missing INICIO symbol S!\n");
  else if (inicio.coluna == -2 && inicio.linha == -2)
    printf("Error: Multiple INICIO symbols!\n");
  else
  {
    int achou = resolveLabirinto(N_LINHAS, N_COLUNAS, labirinto, inicio.coluna, inicio.linha);
    if (achou)
      printf("Caminho encontrado!\n");
    else
      printf("Caminho nao encontrado.\n");
    imprimeLabirinto(N_LINHAS, N_COLUNAS, labirinto);
  }

  return 0;
}

Labirinto *carregaLabirinto()
{
  char *caminhoArquivo;
  printf("Digite o caminho para o arquivo do labirinto: ");
  scanf("%s", caminhoArquivo);
  FILE *arquivo = fopen(caminhoArquivo, "r");

  int linhas, colunas;
  fscanf(arquivo, "%d", &linhas);
  fscanf(arquivo, "%d", &colunas);

  Labirinto *labirinto = NULL;
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

void imprimeLabirinto(int linhas, int colunas, char labirinto[linhas][colunas])
{
  for (int y = 0; y < linhas; y++)
  {
    for (int x = 0; x < colunas; x++)
    {
      if (labirinto[y][x] == CAMINHO_CERTO)
        printf("\033[1;32m");
      else if (labirinto[y][x] == CAMINHO_ERRADO)
        printf("\033[1;31m");
      printf("%c", labirinto[y][x]);
      printf("\033[0m");
    }
    printf("\n");
  }
  printf("\n");
}

// Funcao que retorna a posicao inicial do labirinto
// Se nao encontra o simbolo inicial retorna x e y = -1
// Se caso tiver dois simbolos iniciais retorna x e y = -2
Posicao posicaoInicial(int linhas, int colunas, char labirinto[linhas][colunas])
{
  int achou = 0;
  Posicao p;
  for (int y = 0; y < linhas; y++)
  {
    for (int x = 0; x < colunas; x++)
    {
      if (labirinto[y][x] == INICIO)
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
int resolveLabirinto(int linhas, int colunas, char labirinto[linhas][colunas], int x, int y)
{
  // Verificar se a posicao eh valida
  if (x < 0 || x > colunas - 1 || y < 0 || y > linhas - 1)
    return FALSE;

  // Verificar se achou a saida
  if (labirinto[y][x] == SAIDA)
    return TRUE;

  // Verificar se eh uma posicao disponivel para andar
  if (labirinto[y][x] != DISPONIVEL && labirinto[y][x] != INICIO)
    return FALSE;

  // Marca a posicao como caminho para saida
  if (labirinto[y][x] != INICIO)
    labirinto[y][x] = CAMINHO_CERTO;

  // Verifica qual camimho seguir como parte do caminho da solucao em todas as direcoes
  if (resolveLabirinto(linhas, colunas, labirinto, x, y - 1)) // cima
    return TRUE;

  if (resolveLabirinto(linhas, colunas, labirinto, x + 1, y)) // direita
    return TRUE;

  if (resolveLabirinto(linhas, colunas, labirinto, x, y + 1)) // baixo
    return TRUE;

  if (resolveLabirinto(linhas, colunas, labirinto, x - 1, y)) // esquerda
    return TRUE;

  // Se nenhum dos movimentos acima funcionar, backtrack. Retira a posicao como parte do caminho da solucao
  if (labirinto[y][x] != INICIO)
    labirinto[y][x] = CAMINHO_ERRADO;

  return FALSE;
}