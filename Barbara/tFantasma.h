#ifndef TFANTASMA_H_
#define TFANTASMA_H_

#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"

typedef struct tFantasma
{
    /* Posição atual do fantasma (linha,coluna) */
    tPosicao *posicaoAtual;

    /* Posição antiga do fantasma (linha,coluna) */
    tPosicao *posicaoAntiga;

    /* Letra correspondente*/
    char tipo;

    /* Direção atual*/
    int direcao;

    /*Verdadeiro de fantasma estiver sobrepondo uma comida*/
    bool pisaFruta;
} tFantasma;

/**
 * Procura e cria a lista de fastasmas. Caso dê erro na alocação da estrutura,
 * retorna NULL.
 * Aloca os fantasmas inexistentes como NULL.
 * Caso não dê erros, retorna o ponteiro de ponteiro para os tFantasma alocados.
 * \param mapa mapa para buscar fantasmas
 */
tFantasma **CriaFantasmas(tMapa *mapa);

/**
 * Aloca fantasma dinamicamente. Caso dê erro na alocação de alguma estrutura tFantasma,
 * retorna NULL.
 * Caso a posicao seja NULL, retorna NULL.
 * Caso não dê erros, retorna o ponteiro para ponteiro para os tFantasma alocados.
 * \param posicao posicao do fantasma
 * \param tipo caractere do fantasma
 */
tFantasma *AlocaFantasma(tPosicao *posicao, char tipo);

/**
 * Retorna a posição do fantasma.
 *
 * \param fantasma fantasma
 */
tPosicao *ObtemPosicaoFantasma(tFantasma *fantasma);

/**
 * Retorna a posição anterior do fantasma.
 *
 * \param fantasma fantasma
 */
tPosicao *ObtemRastroFantasma(tFantasma *fantasma);

/**
 * Retorna se o fantasma esta ou não em cima de fruta.
 *
 * \param fantasma fantasma
 */
bool PisaEmFruta(tFantasma *fantasma);

/**
 * Torna o pisaFruta falso.
 *
 * \param fantasma fantasma
 */
void SaiDaFruta(tFantasma *fantasma);

/**
 * Retorna o tipo do fantasma.
 *
 * \param fantasma fantasma
 */
char ObtemTipoFantasma(tFantasma *fantasma);

/**
 * Função que irá mover o fantasma no mapa, atualizando sua posição.
 * Dado o fantasma e o mapa, a posição do fantasma é atualizada.
 * Se o fantasma encontrou uma parede ele inverte sua direcao.
 *
 * \param fantasma fantasma
 * \param mapa o mapa que contem o fantasma
 */
void MoveFantasma(tFantasma *fantasma, tMapa *mapa);

/**
 * Função que irá mover todos os fantasma no mapa, atualizando sua posição.
 * Dado os fantasmas e o mapa, a posição de cada fantasma existente é atualizada.
 *
 * \param fantasmas lista de fantasmaa
 * \param mapa o mapa que contem os fantasmas
 */
void MoveFantasmas(tFantasma **fantasmas, tMapa *mapa);

/**
 * Caso o fantasma seja diferente de NULL, libera o espaço
 * alocado para a estrutura tFantasma
 *
 * \param fantasma fantasma
 */
void DesalocaFantasma(tFantasma *fantasma);

/**
 * Libera o espaço alocado para a lista de fantasmas
 *
 * \param fantasmas lista de fantasmas
 */
void DesalocaFantasmas(tFantasma **fantasmas);

#endif
