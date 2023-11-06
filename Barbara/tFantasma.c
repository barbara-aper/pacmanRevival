#include "tFantasma.h"

tFantasma **CriaFantasmas(tMapa *mapa)
{
    tFantasma **fantasmas = (tFantasma **)malloc(4 * sizeof(tFantasma));

    fantasmas[0] = AlocaFantasma(ObtemPosicaoItemMapa(mapa, 'B'), 'B');
    fantasmas[1] = AlocaFantasma(ObtemPosicaoItemMapa(mapa, 'P'), 'P');
    fantasmas[2] = AlocaFantasma(ObtemPosicaoItemMapa(mapa, 'I'), 'I');
    fantasmas[3] = AlocaFantasma(ObtemPosicaoItemMapa(mapa, 'C'), 'C');

    return fantasmas;
}

tFantasma *AlocaFantasma(tPosicao *posicao, char tipo)
{
    if (posicao == NULL)
        return NULL;

    tFantasma *fantasma = (tFantasma *)malloc(sizeof(tFantasma));

    if (fantasma == NULL)
        return NULL;

    fantasma->posicaoAtual = posicao;

    fantasma->posicaoAntiga = ClonaPosicao(posicao);

    fantasma->tipo = tipo;

    fantasma->pisaFruta = false;

    if (tipo == 'B')
        fantasma->direcao = MOV_ESQUERDA;
    else if (tipo == 'P')
        fantasma->direcao = MOV_CIMA;
    else if (tipo == 'I')
        fantasma->direcao = MOV_BAIXO;
    else if (tipo == 'C')
        fantasma->direcao = MOV_DIREITA;

    return fantasma;
}

tPosicao *ObtemPosicaoFantasma(tFantasma *fantasma)
{
    return fantasma->posicaoAtual;
}

tPosicao *ObtemRastroFantasma(tFantasma *fantasma)
{
    return fantasma->posicaoAntiga;
}

bool PisaEmFruta(tFantasma *fantasma)
{
    return fantasma->pisaFruta;
}

void SaiDaFruta(tFantasma *fantasma)
{
    fantasma->pisaFruta = false;
}

char ObtemTipoFantasma(tFantasma *fantasma)
{
    return fantasma->tipo;
}

void MoveFantasma(tFantasma *fantasma, tMapa *mapa)
{
    if (fantasma == NULL || mapa == NULL)
        return;

    int andaLinha = 0, andaColuna = 0;

    if (fantasma->direcao == MOV_BAIXO)
        andaLinha = 1;
    else if (fantasma->direcao == MOV_CIMA)
        andaLinha = -1;
    else if (fantasma->direcao == MOV_ESQUERDA)
        andaColuna = -1;
    else if (fantasma->direcao == MOV_DIREITA)
        andaColuna = 1;

    tPosicao *proxima = CriaPosicao(ObtemLinhaPosicao(fantasma->posicaoAtual) + andaLinha, ObtemColunaPosicao(fantasma->posicaoAtual) + andaColuna);

    if (proxima == NULL)
        return;

    if (EncontrouParedeMapa(mapa, proxima))
    {
        if (fantasma->direcao == MOV_BAIXO)
            fantasma->direcao = MOV_CIMA;
        else if (fantasma->direcao == MOV_CIMA)
            fantasma->direcao = MOV_BAIXO;
        else if (fantasma->direcao == MOV_ESQUERDA)
            fantasma->direcao = MOV_DIREITA;
        else if (fantasma->direcao == MOV_DIREITA)
            fantasma->direcao = MOV_ESQUERDA;

        andaLinha *= -1;
        andaColuna *= -1;

        tPosicao *retorno = CriaPosicao(ObtemLinhaPosicao(fantasma->posicaoAtual) + andaLinha, ObtemColunaPosicao(fantasma->posicaoAtual) + andaColuna);

        if (EncontrouComidaMapa(mapa, retorno))
            fantasma->pisaFruta = 1;

        AtualizaPosicao(fantasma->posicaoAntiga, fantasma->posicaoAtual);
        AtualizaPosicao(fantasma->posicaoAtual, retorno);
        DesalocaPosicao(proxima);
        DesalocaPosicao(retorno);
        return;
    }

    if (EncontrouComidaMapa(mapa, proxima))
        fantasma->pisaFruta = 1;

    AtualizaPosicao(fantasma->posicaoAntiga, fantasma->posicaoAtual);
    AtualizaPosicao(fantasma->posicaoAtual, proxima);
    DesalocaPosicao(proxima);
}

void MoveFantasmas(tFantasma **fantasmas, tMapa *mapa)
{
    MoveFantasma(fantasmas[0], mapa);
    MoveFantasma(fantasmas[1], mapa);
    MoveFantasma(fantasmas[2], mapa);
    MoveFantasma(fantasmas[3], mapa);
}

void DesalocaFantasma(tFantasma *fantasma)
{
    if (fantasma == NULL)
        return;

    DesalocaPosicao(fantasma->posicaoAtual);
    DesalocaPosicao(fantasma->posicaoAntiga);
    free(fantasma);
}

void DesalocaFantasmas(tFantasma **fantasmas)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        DesalocaFantasma(fantasmas[i]);
    }
}