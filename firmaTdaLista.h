#ifndef FIRMATDALISTA_H_INCLUDED
#define FIRMATDALISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMO(x,y) (x<=y ? x :y)
typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo *sig;
}tNodo;

typedef tNodo* tLista;

void crearLista(tLista*pl);
int listaLlena(tLista*pl, unsigned cantBytes);
int listaVacia(tLista*pl);
int ponerAlInicioLista(tLista*pl, const void* dato, unsigned cantBytes);
int ponerAlFinalLista(tLista*pl, const void* dato, unsigned cantBytes);
int sacarDelInicio(tLista*pl, void* dato, unsigned cantBytes);
int verPrimerElemento(const tLista*pl, void* dato, unsigned cantBytes);
void vaciarLista(tLista*pl);

#endif // FIRMATDALISTA_H_INCLUDED
