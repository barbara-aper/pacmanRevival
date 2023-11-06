#include "tJogo.h"

tJogo *CriaJogo(const char *caminhoConfig)
{
    tJogo *jogo = (tJogo *)malloc(sizeof(tJogo));

    if (jogo == NULL)
        return NULL;

    jogo->mapa = CriaMapa(caminhoConfig);

    if (jogo->mapa == NULL)
    {
        free(jogo);
        return NULL;
    }

    jogo->pacman = CriaPacman(ObtemPosicaoItemMapa(jogo->mapa, '>'));
    jogo->fantasmas = CriaFantasmas(jogo->mapa);
    CriaTrilhaPacman(jogo->pacman, ObtemNumeroLinhasMapa(jogo->mapa), ObtemNumeroColunasMapa(jogo->mapa));
    AtualizaTrilhaPacman(jogo->pacman);

    return jogo;
}

void SalvaInicializacao(tJogo *jogo)
{
    FILE *arquivoInicializacao;
    arquivoInicializacao = fopen("inicializacao.txt", "w");

    int i = 0, j = 0;

    for (i = 0; i < ObtemNumeroLinhasMapa(jogo->mapa); i++)
    {
        for (j = 0; j < ObtemNumeroColunasMapa(jogo->mapa); j++)
        {
            tPosicao *posicao = CriaPosicao(i, j);
            fprintf(arquivoInicializacao, "%c", ObtemItemMapa(jogo->mapa, posicao));
            DesalocaPosicao(posicao);
        }
        fprintf(arquivoInicializacao, "\n");
    }

    fprintf(arquivoInicializacao, "Pac-Man comecara o jogo na linha %d e coluna %d\n", ObtemLinhaPosicao(ObtemPosicaoPacman(jogo->pacman)) + 1, ObtemColunaPosicao(ObtemPosicaoPacman(jogo->pacman)) + 1);

    fclose(arquivoInicializacao);
}

void SalvaEstatisticas(tJogo *jogo)
{
    FILE *arquivoStats;
    arquivoStats = fopen("estatisticas.txt", "w");

    fprintf(arquivoStats, "Numero de movimentos: %d\n", ObtemNumeroAtualMovimentosPacman(jogo->pacman));
    fprintf(arquivoStats, "Numero de movimentos sem pontuar: %d\n", ObtemNumeroMovimentosSemPontuarPacman(jogo->pacman));
    fprintf(arquivoStats, "Numero de colisoes com parede: %d\n", ObtemNumeroColisoesParedePacman(jogo->pacman));
    fprintf(arquivoStats, "Numero de movimentos para baixo: %d\n", ObtemNumeroMovimentosBaixoPacman(jogo->pacman));
    fprintf(arquivoStats, "Numero de movimentos para cima: %d\n", ObtemNumeroMovimentosCimaPacman(jogo->pacman));
    fprintf(arquivoStats, "Numero de movimentos para esquerda: %d\n", ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman));
    fprintf(arquivoStats, "Numero de movimentos para direita: %d\n", ObtemNumeroMovimentosDireitaPacman(jogo->pacman));

    fclose(arquivoStats);
}

void SalvaTrilha(tJogo *jogo)
{
    SalvaTrilhaPacman(jogo->pacman);
}

void SalvaResumo(tJogo *jogo)
{
    FILE *arquivoResumo;
    arquivoResumo = fopen("resumo.txt", "w");

    int i = 0;
    char tecla;
    for (i = 0; i < ObtemNumeroMovimentosSignificativosPacman(jogo->pacman); i++)
    {
        if (ObtemComandoMovimento(jogo->pacman->historicoDeMovimentosSignificativos[i]) == MOV_BAIXO)
            tecla = 's';
        else if (ObtemComandoMovimento(jogo->pacman->historicoDeMovimentosSignificativos[i]) == MOV_CIMA)
            tecla = 'w';
        else if (ObtemComandoMovimento(jogo->pacman->historicoDeMovimentosSignificativos[i]) == MOV_DIREITA)
            tecla = 'd';
        else if (ObtemComandoMovimento(jogo->pacman->historicoDeMovimentosSignificativos[i]) == MOV_ESQUERDA)
            tecla = 'a';

        fprintf(arquivoResumo, "Movimento %d (%c) %s\n", ObtemNumeroMovimento(jogo->pacman->historicoDeMovimentosSignificativos[i]), tecla, ObtemAcaoMovimento(jogo->pacman->historicoDeMovimentosSignificativos[i]));
    }

    fclose(arquivoResumo);
}

void LimpaMapa(tJogo *jogo)
{
    if (jogo == NULL)
        return;

    AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), ' ');

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        if (jogo->fantasmas[i] == NULL)
            continue;
        if (PisaEmFruta(jogo->fantasmas[i]))
        {
            AtualizaItemMapa(jogo->mapa, ObtemPosicaoFantasma(jogo->fantasmas[i]), '*');
            SaiDaFruta(jogo->fantasmas[i]);
        }
        else
            AtualizaItemMapa(jogo->mapa, ObtemPosicaoFantasma(jogo->fantasmas[i]), ' ');
    }
}

void RealizaJogo(tJogo *jogo)
{
    if (jogo == NULL)
        return;

    char tecla;
    int comando;
    int m, i, j;

    for (m = 0; m < ObtemNumeroMaximoMovimentosMapa(jogo->mapa); m++)
    {
        scanf("%c%*c", &tecla);

        if (tecla == 'w')
            comando = MOV_CIMA;
        else if (tecla == 'a')
            comando = MOV_ESQUERDA;
        else if (tecla == 's')
            comando = MOV_BAIXO;
        else if (tecla == 'd')
            comando = MOV_DIREITA;
        else
            continue;

        AndaJogo(jogo, comando);

        printf("Estado do jogo apos o movimento '%c':\n", tecla);

        for (i = 0; i < ObtemNumeroLinhasMapa(jogo->mapa); i++)
        {
            for (j = 0; j < ObtemNumeroColunasMapa(jogo->mapa); j++)
            {
                tPosicao *posicao = CriaPosicao(i, j);

                if (PossuiTunelMapa(jogo->mapa) && AcessouTunelMapa(jogo->mapa, posicao))
                {
                    AtualizaItemMapa(jogo->mapa, posicao, '@');
                    if (EstaVivoPacman(jogo->pacman))
                        AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), '>');
                }

                printf("%c", ObtemItemMapa(jogo->mapa, posicao));

                DesalocaPosicao(posicao);
            }
            printf("\n");
        }
        printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(jogo->pacman));

        if (ObtemPontuacaoAtualPacman(jogo->pacman) == ObtemQuantidadeFrutasIniciaisMapa(jogo->mapa))
        {
            printf("Voce venceu!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(jogo->pacman));
            return;
        }
        if (!EstaVivoPacman(jogo->pacman))
        {
            printf("Game over!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(jogo->pacman));
            return;
        }
        if (m == ObtemNumeroMaximoMovimentosMapa(jogo->mapa) - 1)
        {
            printf("Game over!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(jogo->pacman));
            return;
        }
    }
}

void AndaJogo(tJogo *jogo, COMANDO comando)
{
    if (jogo == NULL)
        return;

    LimpaMapa(jogo);

    MoveFantasmas(jogo->fantasmas, jogo->mapa);

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        if (jogo->fantasmas[i] == NULL)
            continue;
        if (!PisaEmFruta(jogo->fantasmas[i]))
            AtualizaItemMapa(jogo->mapa, ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemTipoFantasma(jogo->fantasmas[i]));
    }

    tPacman *passado = ClonaPacman(jogo->pacman);
    MovePacman(jogo->pacman, jogo->mapa, comando);

    for (i = 0; i < 4; i++)
    {
        if (jogo->fantasmas[i] == NULL)
            continue;
        if (PisaEmFruta(jogo->fantasmas[i]))
        {
            AtualizaItemMapa(jogo->mapa, ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemTipoFantasma(jogo->fantasmas[i]));

            /*tratamento do caso em que pacman pontua na mesma posicao que fantasma*/
            if (SaoIguaisPosicao(ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)))
            {
                MataPacman(jogo->pacman);
                InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, "fim de jogo por encostar em um fantasma");
                break;
            }
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (jogo->fantasmas[i] == NULL)
            continue;

        if (EstaVivoPacman(jogo->pacman) && SaoIguaisPosicao(ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)))
        {
            MataPacman(jogo->pacman);
            InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, "fim de jogo por encostar em um fantasma");
            break;
        }
        else if (SaoIguaisPosicao(ObtemRastroFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)) && SaoIguaisPosicao(ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(passado)))
        {
            MataPacman(jogo->pacman);
            InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, "fim de jogo por encostar em um fantasma");
            break;
        }
    }

    if (EstaVivoPacman(jogo->pacman))
        AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), '>');

    DesalocaPacman(passado);
}

void DesalocaJogo(tJogo *jogo)
{
    if (jogo == NULL)
        return;

    DesalocaMapa(jogo->mapa);
    DesalocaPacman(jogo->pacman);
    DesalocaFantasmas(jogo->fantasmas);
    free(jogo);
}