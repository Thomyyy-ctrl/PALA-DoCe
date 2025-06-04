#ifndef FIRMATDALISTA_H_INCLUDED
#define FIRMATDALISTA_H_INCLUDED
#define MIN(a,b) ((a)>(b)?(b):(a))
#define NO_ENCONTRADO -1
#define LISTA_VACIA -2
#define OK 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct sNodo{
    void *dato;
    unsigned tamDato;
    struct sNodo *sig;
}tNodo;

typedef tNodo * tLista;

void crearLista (tLista *lista);
int listaVacia (const tLista *lista);
int listaLlena (const tLista *lista);
void vaciarLista (tLista *lista);

//insertar a la lista
int insertarAlInicio(tLista *lista, const void *dato, unsigned tamDato);
int insertarAlFinal(tLista *lista, const void *dato, unsigned tamDato);
int insertarOrdenado (tLista *lista, const void *dato, unsigned tamDato, const int cmp(const void *dato1, const void *dato2));

//sacar de la lista
int sacarPrimero(tLista *lista,void *dato, unsigned tamDato);
int sacarUltimo (tLista *lista, void *dato, unsigned tamDato);

//ver elementos de la lista
void mostrarLista (const tLista *lista, const void imprimir(const void *dato));
//mappeo de lista
void mappearLista (tLista *lista, const void mappeo(const void *dato, const void* dato1));


#endif // FIRMATDALISTA_H_INCLUDED
