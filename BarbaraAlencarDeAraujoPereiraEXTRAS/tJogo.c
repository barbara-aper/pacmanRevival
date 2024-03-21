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
    jogo->bomba = CriaBomba(ObtemPosicaoItemMapa(jogo->mapa, '&'));
    CriaTrilhaPacman(jogo->pacman, ObtemNumeroLinhasMapa(jogo->mapa), ObtemNumeroColunasMapa(jogo->mapa));
    AtualizaTrilhaPacman(jogo->pacman);

    return jogo;
}

void SalvaInicializacao(tJogo *jogo)
{
    FILE *arquivoInicializacao;
    arquivoInicializacao = fopen("inicializacao.txt", "w");

    if (!arquivoInicializacao)
    {
        printf("Erro ao abrir o arquivo inicializacao.txt\n");
        return;
    }

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

    if (!arquivoStats)
    {
        printf("Erro ao abrir o arquivo estatisticas.txt\n");
        return;
    }

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

    if (!arquivoResumo)
    {
        printf("Erro ao abrir o arquivo resumo.txt\n");
        return;
    }

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

int MelhorMovimento(tJogo *jogo, int *lista)
{
    int i, indiceMaior = 0, maior = 0;
    bool empate = false;

    for (i = 0; i < 6; i++)
    {
        if (lista[i] == 0)
        {
            indiceMaior = i;
            break;
        }
    }

    if (indiceMaior == MOV_ESQUERDA)
        maior = ObtemNumeroFrutasComidasEsquerdaPacman(jogo->pacman);
    else if (indiceMaior == MOV_CIMA)
        maior = ObtemNumeroFrutasComidasCimaPacman(jogo->pacman);
    else if (indiceMaior == MOV_BAIXO)
        maior = ObtemNumeroFrutasComidasBaixoPacman(jogo->pacman);
    else if (indiceMaior == MOV_DIREITA)
        maior = ObtemNumeroFrutasComidasDireitaPacman(jogo->pacman);

    if (ObtemNumeroFrutasComidasEsquerdaPacman(jogo->pacman) > maior && lista[MOV_ESQUERDA] == 0)
    {
        maior = ObtemNumeroFrutasComidasEsquerdaPacman(jogo->pacman);
        indiceMaior = MOV_ESQUERDA;
    }
    if (ObtemNumeroFrutasComidasCimaPacman(jogo->pacman) > maior && lista[MOV_CIMA] == 0)
    {
        maior = ObtemNumeroFrutasComidasCimaPacman(jogo->pacman);
        indiceMaior = MOV_CIMA;
    }
    if (ObtemNumeroFrutasComidasBaixoPacman(jogo->pacman) > maior && lista[MOV_BAIXO] == 0)
    {
        maior = ObtemNumeroFrutasComidasBaixoPacman(jogo->pacman);
        indiceMaior = MOV_BAIXO;
    }
    if (ObtemNumeroFrutasComidasDireitaPacman(jogo->pacman) > maior && lista[MOV_DIREITA] == 0)
    {
        maior = ObtemNumeroFrutasComidasDireitaPacman(jogo->pacman);
        indiceMaior = MOV_DIREITA;
    }

    if (ObtemNumeroFrutasComidasEsquerdaPacman(jogo->pacman) == maior && lista[MOV_ESQUERDA] == 0)
    {
        lista[MOV_ESQUERDA] = -1;
        empate = true;
    }
    if (ObtemNumeroFrutasComidasCimaPacman(jogo->pacman) == maior && lista[MOV_CIMA] == 0)
    {
        lista[MOV_CIMA] = -1;
        empate = true;
    }
    if (ObtemNumeroFrutasComidasBaixoPacman(jogo->pacman) == maior && lista[MOV_BAIXO] == 0)
    {
        lista[MOV_BAIXO] = -1;
        empate = true;
    }
    if (ObtemNumeroFrutasComidasDireitaPacman(jogo->pacman) == maior && lista[MOV_DIREITA] == 0)
    {
        lista[MOV_DIREITA] = -1;
        empate = true;
    }

    if (!empate)
    {
        for (i = 0; i < 4; i++)
        {
            if (lista[i] != 1)
                lista[i] = 0;
        }

        lista[indiceMaior] = 1;
        return indiceMaior;
    }
    else
    {
        empate = false;

        for (i = 0; i < 4; i++)
        {
            if (lista[i] == -1)
            {
                indiceMaior = i;
                break;
            }
        }

        if (indiceMaior == MOV_ESQUERDA)
            maior = ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman);
        else if (indiceMaior == MOV_CIMA)
            maior = ObtemNumeroColisoesParedeCimaPacman(jogo->pacman);
        else if (indiceMaior == MOV_BAIXO)
            maior = ObtemNumeroColisoesParedeBaixoPacman(jogo->pacman);
        else if (indiceMaior == MOV_DIREITA)
            maior = ObtemNumeroColisoesParedeDireitaPacman(jogo->pacman);

        if (ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman) < maior && lista[MOV_ESQUERDA] == -1)
        {
            maior = ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman);
            indiceMaior = MOV_ESQUERDA;
        }
        if (ObtemNumeroColisoesParedeCimaPacman(jogo->pacman) < maior && lista[MOV_CIMA] == -1)
        {
            maior = ObtemNumeroColisoesParedeCimaPacman(jogo->pacman);
            indiceMaior = MOV_CIMA;
        }
        if (ObtemNumeroColisoesParedeBaixoPacman(jogo->pacman) < maior && lista[MOV_BAIXO] == -1)
        {
            maior = ObtemNumeroColisoesParedeBaixoPacman(jogo->pacman);
            indiceMaior = MOV_BAIXO;
        }
        if (ObtemNumeroColisoesParedeDireitaPacman(jogo->pacman) < maior && lista[MOV_DIREITA] == -1)
        {
            maior = ObtemNumeroColisoesParedeDireitaPacman(jogo->pacman);
            indiceMaior = MOV_DIREITA;
        }

        if (ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman) == maior && lista[MOV_ESQUERDA] == -1)
        {
            lista[MOV_ESQUERDA] = 2;
            empate = true;
        }
        if (ObtemNumeroColisoesParedeCimaPacman(jogo->pacman) == maior && lista[MOV_CIMA] == -1)
        {
            lista[MOV_CIMA] = 2;
            empate = true;
        }
        if (ObtemNumeroColisoesParedeBaixoPacman(jogo->pacman) == maior && lista[MOV_BAIXO] == -1)
        {
            lista[MOV_BAIXO] = 2;
            empate = true;
        }
        if (ObtemNumeroColisoesParedeDireitaPacman(jogo->pacman) == maior && lista[MOV_DIREITA] == -1)
        {
            lista[MOV_DIREITA] = 2;
            empate = true;
        }

        if (!empate)
        {
            for (i = 0; i < 4; i++)
            {
                if (lista[i] != 1)
                    lista[i] = 0;
            }

            lista[indiceMaior] = 1;
            return indiceMaior;
        }
        else
        {
            empate = false;

            for (i = 0; i < 4; i++)
            {
                if (lista[i] == 2)
                {
                    indiceMaior = i;
                    break;
                }
            }

            if (indiceMaior == MOV_ESQUERDA)
                maior = ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman);
            else if (indiceMaior == MOV_CIMA)
                maior = ObtemNumeroMovimentosCimaPacman(jogo->pacman);
            else if (indiceMaior == MOV_BAIXO)
                maior = ObtemNumeroMovimentosBaixoPacman(jogo->pacman);
            else if (indiceMaior == MOV_DIREITA)
                maior = ObtemNumeroMovimentosDireitaPacman(jogo->pacman);

            if (ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman) > maior && lista[MOV_ESQUERDA] == 2)
            {
                maior = ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman);
                indiceMaior = MOV_ESQUERDA;
            }
            if (ObtemNumeroMovimentosCimaPacman(jogo->pacman) > maior && lista[MOV_CIMA] == 2)
            {
                maior = ObtemNumeroMovimentosCimaPacman(jogo->pacman);
                indiceMaior = MOV_CIMA;
            }
            if (ObtemNumeroMovimentosBaixoPacman(jogo->pacman) > maior && lista[MOV_BAIXO] == 2)
            {
                maior = ObtemNumeroMovimentosBaixoPacman(jogo->pacman);
                indiceMaior = MOV_BAIXO;
            }
            if (ObtemNumeroMovimentosDireitaPacman(jogo->pacman) > maior && lista[MOV_DIREITA] == 2)
            {
                maior = ObtemNumeroMovimentosDireitaPacman(jogo->pacman);
                indiceMaior = MOV_DIREITA;
            }

            if (ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman) == maior && lista[MOV_ESQUERDA] == 2)
            {
                lista[MOV_ESQUERDA] = 3;
                empate = true;
            }
            if (ObtemNumeroMovimentosCimaPacman(jogo->pacman) == maior && lista[MOV_CIMA] == 2)
            {
                lista[MOV_CIMA] = 3;
                empate = true;
            }
            if (ObtemNumeroMovimentosBaixoPacman(jogo->pacman) == maior && lista[MOV_BAIXO] == 2)
            {
                lista[MOV_BAIXO] = 3;
                empate = true;
            }
            if (ObtemNumeroMovimentosDireitaPacman(jogo->pacman) == maior && lista[MOV_DIREITA] == 2)
            {
                lista[MOV_DIREITA] = 3;
                empate = true;
            }

            if (!empate)
            {
                for (i = 0; i < 4; i++)
                {
                    if (lista[i] != 1)
                        lista[i] = 0;
                }

                lista[indiceMaior] = 1;
                return indiceMaior;
            }
            else
            {
                if (lista[MOV_ESQUERDA] == 3)
                    indiceMaior = MOV_ESQUERDA;
                else if (lista[MOV_DIREITA] == 3)
                    indiceMaior = MOV_DIREITA;
                else if (lista[MOV_BAIXO] == 3)
                    indiceMaior = MOV_BAIXO;
                else if (lista[MOV_CIMA] == 3)
                    indiceMaior = MOV_CIMA;

                for (i = 0; i < 4; i++)
                {
                    if (lista[i] != 1)
                        lista[i] = 0;
                }

                lista[indiceMaior] = 1;
                return indiceMaior;
            }
        }
    }
}

void SalvaRanking(tJogo *jogo)
{
    int *lista = (int *)malloc(4 * sizeof(int));
    lista[0] = 0;
    lista[1] = 0;
    lista[2] = 0;
    lista[3] = 0;

    FILE *arquivoRank;
    arquivoRank = fopen("ranking.txt", "w");
    int prox;

    if (!arquivoRank)
    {
        printf("Erro ao abrir o arquivo ranking.txt\n");
        return;
    }

    int i;
    for (i = 0; i < 4; i++)
    {
        prox = MelhorMovimento(jogo, lista);
        if (prox == MOV_ESQUERDA)
            fprintf(arquivoRank, "a,%d,%d,%d\n", ObtemNumeroFrutasComidasEsquerdaPacman(jogo->pacman),
                    ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman), ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman));
        else if (prox == MOV_CIMA)
            fprintf(arquivoRank, "w,%d,%d,%d\n", ObtemNumeroFrutasComidasCimaPacman(jogo->pacman),
                    ObtemNumeroColisoesParedeCimaPacman(jogo->pacman), ObtemNumeroMovimentosCimaPacman(jogo->pacman));
        else if (prox == MOV_BAIXO)
            fprintf(arquivoRank, "s,%d,%d,%d\n", ObtemNumeroFrutasComidasBaixoPacman(jogo->pacman),
                    ObtemNumeroColisoesParedeBaixoPacman(jogo->pacman), ObtemNumeroMovimentosBaixoPacman(jogo->pacman));
        else if (prox == MOV_DIREITA)
            fprintf(arquivoRank, "d,%d,%d,%d\n", ObtemNumeroFrutasComidasDireitaPacman(jogo->pacman),
                    ObtemNumeroColisoesParedeDireitaPacman(jogo->pacman), ObtemNumeroMovimentosDireitaPacman(jogo->pacman));
    }

    free(lista);
    fclose(arquivoRank);
}

void LimpaMapa(tJogo *jogo)
{
    if (jogo == NULL)
        return;

    AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), ' ');

    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (jogo->fantasmas[i] == NULL)
            continue;
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
    int m, i, j, f;

    LimpaMapa(jogo);

    for (m = 0; m < ObtemNumeroMaximoMovimentosMapa(jogo->mapa); m++)
    {
        if (!EstaVivoPacman(jogo->pacman))
        {
            printf("Game over!\nPontuacao final: %d\n", ObtemPontuacaoAtualPacman(jogo->pacman));
            return;
        }

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

                int fantasma = 0, pacman = 0, bomba = 0;
                
                for (f = 0; f < 6; f++)
                {
                    if (jogo->fantasmas[f] == NULL)
                        continue;
                    
                    if (SaoIguaisPosicao(ObtemPosicaoFantasma(jogo->fantasmas[f]), posicao))
                    {
                        printf("%c", ObtemTipoFantasma(jogo->fantasmas[f]));
                        fantasma = 1;
                        break;
                    }
                }
                
                if (EstaVivoPacman(jogo->pacman) && SaoIguaisPosicao(ObtemPosicaoPacman(jogo->pacman), posicao))
                {
                    printf(">");
                    pacman = 1;
                }

                if (!fantasma && !pacman && jogo->bomba != NULL && SaoIguaisPosicao(ObtemPosicaoBomba(jogo->bomba), posicao) && EstaAtivada(jogo->bomba))
                {
                    printf("%d", TimerBomba(jogo->bomba));
                    bomba = 1;
                }
                
                if (!pacman && !fantasma && !bomba)
                {
                    printf("%c", ObtemItemMapa(jogo->mapa, posicao));
                }
                
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

    MoveFantasmas(jogo->fantasmas, jogo->mapa);

    int i = 0;

    tPacman *passado = ClonaPacman(jogo->pacman);
    MovePacman(jogo->pacman, jogo->mapa, comando);

    if (jogo->bomba != NULL)
    {
        if (TimerBomba(jogo->bomba) == 1)
            ExplodeBomba(jogo);
        
        if (EstaAtivada(jogo->bomba))
            ContaTempoBomba(jogo->bomba);
        
        if (SaoIguaisPosicao(ObtemPosicaoPacman(jogo->pacman), ObtemPosicaoBomba(jogo->bomba)))
            AtivaBomba(jogo->bomba);
    }

    for (i = 0; i < 6; i++)
    {
        if (jogo->fantasmas[i] == NULL)
            continue;

        if (EstaVivoPacman(jogo->pacman) && SaoIguaisPosicao(ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)))
        {
            if (EstaMatando(jogo->pacman) > 0)
            {
                DesalocaFantasma(jogo->fantasmas[i]);
                jogo->fantasmas[i] = NULL;
                MatouFantasma(jogo->pacman);
            }
            else
            {
                MataPacman(jogo->pacman);
                InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, "fim de jogo por encostar em um fantasma");
                break;
            }
        }
        else if (SaoIguaisPosicao(ObtemRastroFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)) && SaoIguaisPosicao(ObtemPosicaoFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(passado)))
        {
            if (EstaMatando(jogo->pacman) > 0)
            {
                DesalocaFantasma(jogo->fantasmas[i]);
                jogo->fantasmas[i] = NULL;
                MatouFantasma(jogo->pacman);
            }
            else
            {
                MataPacman(jogo->pacman);
                InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, "fim de jogo por encostar em um fantasma");
                break;
            }
        }
    }

    DesalocaPacman(passado);
}

void DesalocaJogo(tJogo *jogo)
{
    if (jogo == NULL)
        return;

    DesalocaMapa(jogo->mapa);
    DesalocaPacman(jogo->pacman);
    DesalocaFantasmas(jogo->fantasmas);
    DesalocaBomba(jogo->bomba);
    free(jogo);
}

void ExplodeBomba(tJogo *jogo)
{
    int i, j, f;

    for (i = ObtemLinhaPosicao(ObtemPosicaoBomba(jogo->bomba)) - 1; i <=  ObtemLinhaPosicao(ObtemPosicaoBomba(jogo->bomba)) + 1; i++)
    {
        for (j = ObtemColunaPosicao(ObtemPosicaoBomba(jogo->bomba)) - 1; j <= ObtemColunaPosicao(ObtemPosicaoBomba(jogo->bomba)) + 1; j++)
        {
            if (i == 0 || j == 0 || i == ObtemNumeroLinhasMapa(jogo->mapa) - 1 ||j == ObtemNumeroColunasMapa(jogo->mapa) - 1)
                continue;
            
            tPosicao *posicao = CriaPosicao(i,j);

            for (f = 0; f < f; f++)
            {
                if (jogo->fantasmas[f] == NULL)
                    continue;
                
                if (SaoIguaisPosicao(posicao, ObtemPosicaoFantasma(jogo->fantasmas[f])))
                    DesalocaFantasma(jogo->fantasmas[f]);
                    jogo->fantasmas[f] = NULL;
            }
        
            if (SaoIguaisPosicao(posicao, ObtemPosicaoPacman(jogo->pacman)))
                MataPacman(jogo->pacman);
            else if (EncontrouComidaMapa(jogo->mapa, posicao))
            {
                jogo->mapa->nFrutasAtual--;
            }
            
            AtualizaItemMapa(jogo->mapa, posicao, ' ');

            DesalocaPosicao(posicao);
        }
    }

    DesativaBomba(jogo->bomba);
}