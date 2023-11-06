#include "tJogo.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado");
        return 1;
    }

    tJogo *jogo = CriaJogo(argv[1]);

    SalvaInicializacao(jogo);

    RealizaJogo(jogo);

    SalvaEstatisticas(jogo);

    SalvaTrilha(jogo);

    SalvaResumo(jogo);

    DesalocaJogo(jogo);
}