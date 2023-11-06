#include "tTunel.h"

tTunel *CriaTunel(int linhaAcesso1, int colunaAcesso1, int linhaAcesso2, int colunaAcesso2)
{
    tTunel *tunel = (tTunel *)malloc(sizeof(tTunel));

    tPosicao *posicao1 = CriaPosicao(linhaAcesso1, colunaAcesso1);
    tPosicao *posicao2 = CriaPosicao(linhaAcesso2, colunaAcesso2);

    tunel->acesso1 = posicao1;
    tunel->acesso2 = posicao2;

    return tunel;
}

bool EntrouTunel(tTunel *tunel, tPosicao *posicao)
{
    return SaoIguaisPosicao(tunel->acesso1, posicao) || SaoIguaisPosicao(tunel->acesso2, posicao);
}

void LevaFinalTunel(tTunel *tunel, tPosicao *posicao)
{
    if (SaoIguaisPosicao(tunel->acesso1, posicao))
        AtualizaPosicao(posicao, tunel->acesso2);
    else if (SaoIguaisPosicao(tunel->acesso2, posicao))
        AtualizaPosicao(posicao, tunel->acesso1);
}

void DesalocaTunel(tTunel *tunel)
{
    if (tunel != NULL)
    {
        DesalocaPosicao(tunel->acesso1);
        DesalocaPosicao(tunel->acesso2);
        free(tunel);
    }
    else
        return;
}
