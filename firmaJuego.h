#ifndef FIRMAJUEGO_H_INCLUDED
#define FIRMAJUEGO_H_INCLUDED

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

int crearArchivo(const char*path);

#endif // FIRMAJUEGO_H_INCLUDED
