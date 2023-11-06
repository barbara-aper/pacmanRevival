#include "tPosicao.h"

tPosicao *CriaPosicao(int linha, int coluna)
{
    tPosicao *posicao = (tPosicao *)malloc(sizeof(tPosicao));
    posicao->linha = linha;
    posicao->coluna = coluna;

    return posicao;
}

tPosicao *ClonaPosicao(tPosicao *posicao)
{
    tPosicao *clonePosicao = (tPosicao *)malloc(sizeof(tPosicao));
    *clonePosicao = *posicao;

    return clonePosicao;
}

int ObtemLinhaPosicao(tPosicao *posicao)
{
    return posicao->linha;
}

int ObtemColunaPosicao(tPosicao *posicao)
{
    return posicao->coluna;
}

void AtualizaPosicao(tPosicao *posicaoAtual, tPosicao *posicaoNova)
{
    *posicaoAtual = *posicaoNova;
}

bool SaoIguaisPosicao(tPosicao *posicao1, tPosicao *posicao2)
{
    return (posicao1->linha == posicao2->linha && posicao1->coluna == posicao2->coluna);
}

void DesalocaPosicao(tPosicao *posicao)
{
    if (posicao != NULL)
        free(posicao);
}
