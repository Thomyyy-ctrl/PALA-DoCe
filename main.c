#include <stdio.h>
#include <string.h>
#include "firmaJuego.h"
int main()
{
    //simulo que ya se cargo el jugador
    tPlayer jugador={0,0,SIN_EFECTO_NEGATIVO,JUGADOR_HUMANO,"Jony McJony"};
    tJugada movimientoGanador;
    //se empieza la partida con dificultad facil
    jugarDoce(&jugador,IA_DIFICIL,&movimientoGanador);
    return 0;
}
