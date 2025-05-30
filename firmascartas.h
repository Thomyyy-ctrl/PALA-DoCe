#ifndef FIRMASCARTAS_H_INCLUDED
#define FIRMASCARTAS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_CAD_PL 1000

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

int bajarArchLista(const char* path);//tLista *pl);

#endif // FIRMASCARTAS_H_INCLUDED
