#ifndef TBOMBA_H_
#define TBOMBA_H_

#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"
#include "tPacman.h"
#include "tFantasma.h"

typedef struct tBomba
{
    tPosicao *posicao;
    int timer;
    bool ativada;

} tBomba;


tBomba *CriaBomba(tPosicao *posicao);

void AtivaBomba(tBomba *bomba);

void DesativaBomba(tBomba *bomba);

bool EstaAtivada(tBomba *bomba);

int TimerBomba(tBomba *bomba);

void ContaTempoBomba(tBomba *bomba);

tPosicao *ObtemPosicaoBomba(tBomba *bomba);

void DesalocaBomba(tBomba *bomba);

#endif
