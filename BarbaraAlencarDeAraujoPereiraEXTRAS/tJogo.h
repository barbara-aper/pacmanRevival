#ifndef TJOGO_H_
#define TJOGO_H_

#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"
#include "tPacman.h"
#include "tFantasma.h"
#include "tBomba.h"

typedef struct tJogo
{
    tPacman *pacman;
    tFantasma **fantasmas;
    tMapa *mapa;
    tBomba *bomba;
} tJogo;

/**
 * Cria todos os componentes de tJogo
 *
 * \param caminho caminho do arquivo com as configurações do mapa
 */
tJogo *CriaJogo(const char *caminhoConfig);

/**
 * Gera o arquivo de inicializacao
 *
 * \param jogo jogo
 */
void SalvaInicializacao(tJogo *jogo);

/**
 * Gera o arquivo de estatisticas
 *
 * \param jogo jogo
 */
void SalvaEstatisticas(tJogo *jogo);

/**
 * Chama a funcao SalvaTrilhaPacman
 *
 * \param jogo jogo
 */
void SalvaTrilha(tJogo *jogo);

/**
 * Gera o arquivo de resumo
 *
 * \param jogo jogo
 */
void SalvaResumo(tJogo *jogo);

/**
 * Gera o arquivo de ranking
 *
 * \param jogo jogo
 */
void SalvaRanking(tJogo *jogo);

/**
 * Tira os elementos que se movimentam do mapa
 *
 * \param jogo jogo
 */
void LimpaMapa(tJogo *jogo);

/**
 * Le todas as jogadas pela entrada padrao e gera saida
 *
 * \param jogo jogo
 */
void RealizaJogo(tJogo *jogo);

/**
 * Realiza cada iteração do jogo
 *
 * \param jogo jogo
 * \param comando o comando do movimento
 */
void AndaJogo(tJogo *jogo, COMANDO comando);

void ExplodeBomba(tJogo *jogo);

/**
 * Libera o espaço alocado para o jogo
 *
 * \param jogo jogo
 */
void DesalocaJogo(tJogo *jogo);

#endif
