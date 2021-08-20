#include "Labirinto.h"

int main()
{
  Labirinto *labirinto = carregaLabirinto();

  imprimeLabirinto(labirinto);

  Posicao inicio = posicaoInicial(labirinto);

  if (inicio.coluna == -1 && inicio.linha == -1)
    printf("Erro: Simbolo do inicio faltando! :(\n");
  else if (inicio.coluna == -2 && inicio.linha == -2)
    printf("Erro: Varios simbolos de inicio! :(\n");
  else
  {
    if (resolveLabirinto(labirinto, inicio.coluna, inicio.linha))
      printf("Caminho encontrado! :)\n");
    else
      printf("Caminho nao encontrado! :(\n");
    imprimeLabirinto(labirinto);
  }

  return 0;
}