#ifndef FIRMAJUEGO_H_INCLUDED
#define FIRMAJUEGO_H_INCLUDED
#define IA_DIFICIL 2
#define NOMBRE_IA_DIFICIL "IA DIFICIL"
#define TIPO_ESPEJO 3
#define TIPO_REPETIR_TURNO 4
#include"firmaTdaLista.h"

typedef struct
{
    unsigned MAS_DOS;
    unsigned MAS_UNO;
    unsigned MENOS_UNO;
    unsigned MENOS_DOS;
    unsigned REPETIR_TURNO;
    unsigned ESPEJO;
} tCarta;

typedef struct
{
    unsigned puntaje;
    char nya[TAM_CAD_PL];
} tPlayer;


tCarta* IADificil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano);
int crearArchivo(const char*path);
#endif // FIRMAJUEGO_H_INCLUDED
