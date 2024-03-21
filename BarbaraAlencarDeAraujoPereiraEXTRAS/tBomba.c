#include "tBomba.h"

tBomba *CriaBomba(tPosicao *posicao)
{
    if (posicao == NULL)
        return NULL;
    
    tBomba *bomba = (tBomba *) malloc(sizeof(tBomba));
    bomba->timer = 3;
    bomba->ativada = false;
    bomba->posicao = posicao;

    return bomba;
}

void AtivaBomba(tBomba *bomba)
{
    if (bomba != NULL)
        bomba->ativada = true;
}

bool EstaAtivada(tBomba *bomba)
{
    return bomba->ativada;
}

void DesativaBomba(tBomba *bomba)
{
    bomba->ativada = 0;
}

int TimerBomba(tBomba *bomba)
{
    return bomba->timer;
}

void ContaTempoBomba(tBomba *bomba)
{
    bomba->timer--;
}

tPosicao *ObtemPosicaoBomba(tBomba *bomba)
{
    return bomba->posicao;
}

void DesalocaBomba(tBomba *bomba)
{
    if (bomba == NULL)
        return;

    DesalocaPosicao(bomba->posicao);
    free(bomba);
}
