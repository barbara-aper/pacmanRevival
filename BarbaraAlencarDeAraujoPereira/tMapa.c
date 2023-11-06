#include "tMapa.h"

tMapa *CriaMapa(const char *caminhoConfig)
{
    char caminhoMapa[1000];
    FILE *arquivoMapa;

    tMapa *mapa = (tMapa *)malloc(sizeof(tMapa));

    sprintf(caminhoMapa, "%s/mapa.txt", caminhoConfig);

    arquivoMapa = fopen(caminhoMapa, "r");

    if (!arquivoMapa)
    {
        printf("Erro ao abrir o arquivo %s\n", caminhoMapa);
        return NULL;
    }

    mapa->nColunas = 0;
    mapa->nLinhas = -1;
    mapa->nMaximoMovimentos = 0;
    mapa->nFrutasAtual = 0;

    fscanf(arquivoMapa, "%d\n", &mapa->nMaximoMovimentos);

    bool fimColuna = 0;

    while ((!feof(arquivoMapa)))
    {
        char c;
        fscanf(arquivoMapa, "%c", &c);

        if (c != '\n' && !fimColuna)
            mapa->nColunas++;
        else if (c == '\n')
        {
            fimColuna = 1;
            mapa->nLinhas++;
        }
    }

    mapa->grid = (char **)malloc(mapa->nLinhas * sizeof(char *));
    for (int i = 0; i < mapa->nLinhas; i++)
    {
        mapa->grid[i] = (char *)malloc(mapa->nColunas * sizeof(char));
    }

    rewind(arquivoMapa);

    fscanf(arquivoMapa, "%d\n", &mapa->nMaximoMovimentos);

    int i, j, linhaAcesso1 = -1, linhaAcesso2 = -1, colunaAcesso1 = -1, colunaAcesso2 = -1;

    for (i = 0; i < mapa->nLinhas; i++)
    {
        for (j = 0; j < mapa->nColunas; j++)
        {
            fscanf(arquivoMapa, "%c", &mapa->grid[i][j]);

            if (mapa->grid[i][j] == '*')
            {
                mapa->nFrutasAtual++;
            }
            else if (mapa->grid[i][j] == '@')
            {
                if (linhaAcesso1 == -1)
                {
                    linhaAcesso1 = i;
                    colunaAcesso1 = j;
                }
                else
                {
                    linhaAcesso2 = i;
                    colunaAcesso2 = j;
                }
            }
        }
        fscanf(arquivoMapa, "%*c");
    }

    if (linhaAcesso2 != -1)
        mapa->tunel = CriaTunel(linhaAcesso1, colunaAcesso1, linhaAcesso2, colunaAcesso2);
    else
        mapa->tunel = NULL;

    fclose(arquivoMapa);

    return mapa;
}

tPosicao *ObtemPosicaoItemMapa(tMapa *mapa, char item)
{
    int i, j;

    for (i = 0; i < mapa->nLinhas; i++)
    {
        for (j = 0; j < mapa->nColunas; j++)
        {
            if (mapa->grid[i][j] == item)
            {
                tPosicao *posicao = CriaPosicao(i, j);
                return posicao;
            }
        }
    }

    return NULL;
}

tTunel *ObtemTunelMapa(tMapa *mapa)
{
    return mapa->tunel;
}

char ObtemItemMapa(tMapa *mapa, tPosicao *posicao)
{
    if (mapa->grid == NULL || ObtemLinhaPosicao(posicao) > mapa->nLinhas - 1 || ObtemColunaPosicao(posicao) > mapa->nColunas - 1 || ObtemLinhaPosicao(posicao) < 0 || ObtemColunaPosicao(posicao) < 0)
        return '\0';
    else
        return mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)];
}

int ObtemNumeroLinhasMapa(tMapa *mapa)
{
    return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa *mapa)
{
    return mapa->nColunas;
}

int ObtemQuantidadeFrutasIniciaisMapa(tMapa *mapa)
{
    return mapa->nFrutasAtual;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa *mapa)
{
    return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa *mapa, tPosicao *posicao)
{
    if (mapa->grid == NULL || ObtemLinhaPosicao(posicao) > mapa->nLinhas - 1 || ObtemColunaPosicao(posicao) > mapa->nColunas - 1 || ObtemLinhaPosicao(posicao) < 0 || ObtemColunaPosicao(posicao) < 0)
        return 0;
    else if (mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)] == '*')
        return 1;
    else
        return 0;
}

bool EncontrouParedeMapa(tMapa *mapa, tPosicao *posicao)
{
    if (mapa->grid == NULL || ObtemLinhaPosicao(posicao) > mapa->nLinhas - 1 || ObtemColunaPosicao(posicao) > mapa->nColunas - 1 || ObtemLinhaPosicao(posicao) < 0 || ObtemColunaPosicao(posicao) < 0)
        return 0;
    else if (mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)] == '#')
        return 1;
    else
        return 0;
}

bool AtualizaItemMapa(tMapa *mapa, tPosicao *posicao, char item)
{
    if (mapa->grid == NULL || ObtemLinhaPosicao(posicao) > mapa->nLinhas - 1 || ObtemColunaPosicao(posicao) > mapa->nColunas - 1 || ObtemLinhaPosicao(posicao) < 0 || ObtemColunaPosicao(posicao) < 0)
        return 0;

    mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)] = item;
    return 1;
}

bool PossuiTunelMapa(tMapa *mapa)
{
    if (mapa->tunel == NULL)
        return 0;
    else
        return 1;
}

bool AcessouTunelMapa(tMapa *mapa, tPosicao *posicao)
{
    return EntrouTunel(mapa->tunel, posicao);
}

void EntraTunelMapa(tMapa *mapa, tPosicao *posicao)
{
    LevaFinalTunel(mapa->tunel, posicao);
}

void DesalocaMapa(tMapa *mapa)
{
    if (mapa != NULL)
    {
        int i;

        for (i = 0; i < mapa->nLinhas; i++)
        {
            free(mapa->grid[i]);
        }
        free(mapa->grid);

        DesalocaTunel(mapa->tunel);
        free(mapa);
    }
    else
        return;
}
