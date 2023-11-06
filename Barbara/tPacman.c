#include "tPacman.h"

tPacman *CriaPacman(tPosicao *posicao)
{
    if (posicao == NULL)
        return NULL;

    tPacman *pacman = (tPacman *)malloc(sizeof(tPacman));

    if (pacman == NULL)
        return NULL;

    pacman->estaVivo = 1;
    pacman->nMovimentosBaixo = 0;
    pacman->nFrutasComidasBaixo = 0;
    pacman->nColisoesParedeBaixo = 0;
    pacman->nMovimentosCima = 0;
    pacman->nFrutasComidasCima = 0;
    pacman->nColisoesParedeCima = 0;
    pacman->nMovimentosEsquerda = 0;
    pacman->nFrutasComidasEsquerda = 0;
    pacman->nColisoesParedeEsquerda = 0;
    pacman->nMovimentosDireita = 0;
    pacman->nFrutasComidasDireita = 0;
    pacman->nColisoesParedeDireita = 0;
    pacman->nMovimentosSignificativos = 0;
    pacman->nLinhasTrilha = 0;
    pacman->nColunasTrilha = 0;
    pacman->trilha = NULL;

    pacman->posicaoAtual = posicao;
    pacman->historicoDeMovimentosSignificativos = (tMovimento **)malloc(sizeof(tMovimento));

    return pacman;
}

tPacman *ClonaPacman(tPacman *pacman)
{
    tPacman *clonePacman = CriaPacman(CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual), ObtemColunaPosicao(pacman->posicaoAtual)));

    return clonePacman;
}

tMovimento **ClonaHistoricoDeMovimentosSignificativosPacman(tPacman *pacman)
{
    tMovimento **clone = (tMovimento **)malloc(pacman->nMovimentosSignificativos * sizeof(tMovimento));

    int i;
    for (i = 0; i < pacman->nMovimentosSignificativos; i++)
    {
        clone[i] = CriaMovimento(ObtemNumeroMovimento(pacman->historicoDeMovimentosSignificativos[i]), ObtemComandoMovimento(pacman->historicoDeMovimentosSignificativos[i]), ObtemAcaoMovimento(pacman->historicoDeMovimentosSignificativos[i]));
    }

    return clone;
}

tPosicao *ObtemPosicaoPacman(tPacman *pacman)
{
    return pacman->posicaoAtual;
}

int EstaVivoPacman(tPacman *pacman)
{
    return pacman->estaVivo;
}

void MovePacman(tPacman *pacman, tMapa *mapa, COMANDO comando)
{
    if (mapa == NULL)
        return;

    int andaLinha = 0, andaColuna = 0;

    if (comando == MOV_BAIXO)
    {
        andaLinha = 1;
        pacman->nMovimentosBaixo++;
    }
    else if (comando == MOV_CIMA)
    {
        andaLinha = -1;
        pacman->nMovimentosCima++;
    }
    else if (comando == MOV_ESQUERDA)
    {
        andaColuna = -1;
        pacman->nMovimentosEsquerda++;
    }
    else if (comando == MOV_DIREITA)
    {
        andaColuna = 1;
        pacman->nMovimentosDireita++;
    }

    tPosicao *proxima = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual) + andaLinha, ObtemColunaPosicao(pacman->posicaoAtual) + andaColuna);

    if (proxima == NULL)
        return;

    if (EncontrouParedeMapa(mapa, proxima))
    {
        if (comando == MOV_BAIXO)
            pacman->nColisoesParedeBaixo++;
        else if (comando == MOV_CIMA)
            pacman->nColisoesParedeCima++;
        else if (comando == MOV_ESQUERDA)
            pacman->nColisoesParedeEsquerda++;
        else if (comando == MOV_DIREITA)
            pacman->nColisoesParedeDireita++;

        InsereNovoMovimentoSignificativoPacman(pacman, comando, "colidiu com a parede");
        AtualizaTrilhaPacman(pacman);
        DesalocaPosicao(proxima);
        return;
    }
    else if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, proxima))
    {
        AtualizaPosicao(pacman->posicaoAtual, proxima);
        DesalocaPosicao(proxima);
        AtualizaTrilhaPacman(pacman);

        EntraTunelMapa(mapa, pacman->posicaoAtual);
        AtualizaTrilhaPacman(pacman);
        return;
    }
    else if (EncontrouComidaMapa(mapa, proxima))
    {
        if (comando == MOV_BAIXO)
            pacman->nFrutasComidasBaixo++;
        else if (comando == MOV_CIMA)
            pacman->nFrutasComidasCima++;
        else if (comando == MOV_ESQUERDA)
            pacman->nFrutasComidasEsquerda++;
        else if (comando == MOV_DIREITA)
            pacman->nFrutasComidasDireita++;

        AtualizaItemMapa(mapa, proxima, ' ');
        InsereNovoMovimentoSignificativoPacman(pacman, comando, "pegou comida");
    }

    AtualizaPosicao(pacman->posicaoAtual, proxima);
    DesalocaPosicao(proxima);
    AtualizaTrilhaPacman(pacman);
}

void CriaTrilhaPacman(tPacman *pacman, int nLinhas, int nColunas)
{
    pacman->nLinhasTrilha = nLinhas;
    pacman->nColunasTrilha = nColunas;
    int i, j;

    pacman->trilha = (int **)malloc(nLinhas * sizeof(int *));
    for (i = 0; i < nLinhas; i++)
    {
        pacman->trilha[i] = (int *)malloc(nColunas * sizeof(int));
    }

    for (i = 0; i < pacman->nLinhasTrilha; i++)
    {
        for (j = 0; j < pacman->nColunasTrilha; j++)
        {
            pacman->trilha[i][j] = -1;
        }
    }
}

void AtualizaTrilhaPacman(tPacman *pacman)
{
    pacman->trilha[ObtemLinhaPosicao(pacman->posicaoAtual)][ObtemColunaPosicao(pacman->posicaoAtual)] = ObtemNumeroAtualMovimentosPacman(pacman);
}

void SalvaTrilhaPacman(tPacman *pacman)
{
    FILE *arquivoTrilha;
    arquivoTrilha = fopen("trilha.txt", "w");

    int i, j;

    for (i = 0; i < pacman->nLinhasTrilha; i++)
    {
        for (j = 0; j < pacman->nColunasTrilha; j++)
        {
            if (pacman->trilha[i][j] == -1)
                fprintf(arquivoTrilha, "# ");
            else
                fprintf(arquivoTrilha, "%d ", pacman->trilha[i][j]);
        }
        fprintf(arquivoTrilha, "\n");
    }

    fclose(arquivoTrilha);
}

void InsereNovoMovimentoSignificativoPacman(tPacman *pacman, COMANDO comando, const char *acao)
{
    pacman->nMovimentosSignificativos++;

    pacman->historicoDeMovimentosSignificativos = realloc(pacman->historicoDeMovimentosSignificativos, pacman->nMovimentosSignificativos * sizeof(tMovimento *));

    pacman->historicoDeMovimentosSignificativos[pacman->nMovimentosSignificativos - 1] = CriaMovimento(ObtemNumeroAtualMovimentosPacman(pacman), comando, acao);
}

void MataPacman(tPacman *pacman)
{
    pacman->estaVivo = 0;
}

void DesalocaPacman(tPacman *pacman)
{
    if (pacman == NULL)
        return;

    int i;

    if (pacman->trilha != NULL)
    {
        for (i = 0; i < pacman->nLinhasTrilha; i++)
        {
            free(pacman->trilha[i]);
        }
        free(pacman->trilha);
    }

    if (pacman->historicoDeMovimentosSignificativos != NULL)
    {
        for (i = 0; i < pacman->nMovimentosSignificativos; i++)
        {
            DesalocaMovimento(pacman->historicoDeMovimentosSignificativos[i]);
        }
        free(pacman->historicoDeMovimentosSignificativos);
    }

    DesalocaPosicao(pacman->posicaoAtual);

    free(pacman);
}

int ObtemNumeroAtualMovimentosPacman(tPacman *pacman)
{
    return pacman->nMovimentosBaixo + pacman->nMovimentosCima + pacman->nMovimentosDireita + pacman->nMovimentosEsquerda;
}

int ObtemNumeroMovimentosSemPontuarPacman(tPacman *pacman)
{
    return ObtemNumeroAtualMovimentosPacman(pacman) - ObtemPontuacaoAtualPacman(pacman);
}

int ObtemNumeroColisoesParedePacman(tPacman *pacman)
{
    return pacman->nColisoesParedeBaixo + pacman->nColisoesParedeCima + pacman->nColisoesParedeDireita + pacman->nColisoesParedeEsquerda;
}

int ObtemNumeroMovimentosBaixoPacman(tPacman *pacman)
{
    return pacman->nMovimentosBaixo;
}

int ObtemNumeroFrutasComidasBaixoPacman(tPacman *pacman)
{
    return pacman->nFrutasComidasBaixo;
}

int ObtemNumeroColisoesParedeBaixoPacman(tPacman *pacman)
{
    return pacman->nColisoesParedeBaixo;
}

int ObtemNumeroMovimentosCimaPacman(tPacman *pacman)
{
    return pacman->nMovimentosCima;
}

int ObtemNumeroFrutasComidasCimaPacman(tPacman *pacman)
{
    return pacman->nFrutasComidasCima;
}

int ObtemNumeroColisoesParedeCimaPacman(tPacman *pacman)
{
    return pacman->nColisoesParedeCima;
}

int ObtemNumeroMovimentosEsquerdaPacman(tPacman *pacman)
{
    return pacman->nMovimentosEsquerda;
}

int ObtemNumeroFrutasComidasEsquerdaPacman(tPacman *pacman)
{
    return pacman->nFrutasComidasEsquerda;
}

int ObtemNumeroColisoesParedeEsquerdaPacman(tPacman *pacman)
{
    return pacman->nColisoesParedeEsquerda;
}

int ObtemNumeroMovimentosDireitaPacman(tPacman *pacman)
{
    return pacman->nMovimentosDireita;
}

int ObtemNumeroFrutasComidasDireitaPacman(tPacman *pacman)
{
    return pacman->nFrutasComidasDireita;
}

int ObtemNumeroColisoesParedeDireitaPacman(tPacman *pacman)
{
    return pacman->nColisoesParedeDireita;
}

int ObtemNumeroMovimentosSignificativosPacman(tPacman *pacman)
{
    return pacman->nMovimentosSignificativos;
}

int ObtemPontuacaoAtualPacman(tPacman *pacman)
{
    return pacman->nFrutasComidasBaixo + pacman->nFrutasComidasCima + pacman->nFrutasComidasDireita + pacman->nFrutasComidasEsquerda;
}