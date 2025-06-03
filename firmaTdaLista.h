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

typedef struct
{
    char nombre[50];
    unsigned dni;
    int nota;
}tAlumno;

typedef tNodo * tLista;

void crearLista (tLista *lista);
int listaVacia (const tLista *lista);
int listaLlena (const tLista *lista);
void vaciarLista (tLista *lista);

//insertar a la lista
int insertarAlInicio(tLista *lista, const void *dato, unsigned tamDato);
int insertarAlFinal(tLista *lista, const void *dato, unsigned tamDato);
int insertarPosicionN(tLista *lista, const void *dato, unsigned tamDato, unsigned posicion);

//sacar de la lista
int sacarPrimero(tLista *lista,void *dato, unsigned tamDato);
int sacarUltimo (tLista *lista, void *dato, unsigned tamDato);
int sacarPosicionN (tLista *lista, void *dato, unsigned tamDato, unsigned posicion);
int sacarPorClaveOL (tLista *lista, void *dato, unsigned tamDato, const int cmp (const void *dato1, const void *dato2));
int sacarPorClaveUL (tLista *lista, void *dato, unsigned tamDato, const int cmp(const void *dato1, const void *dato2));
int eliminarDuplicadosPorClaveUL(tLista *lista, void *datoAccion, const int cmp(const void *dato1, const void *dato2), void accion(void *datoAccion, void *dato2),int realizarAccion);

//ver elementos de la lista
int verPrimero(const tLista *lista, void *dato, unsigned tamDato);
int verUltimo (const tLista *lista, void *dato, unsigned tamDato);
int verPosicionN (const tLista *lista, void *dato, unsigned tamDato, unsigned posicion);
int verPorClave(const tLista *lista, void *dato, unsigned tamDato, const void *clave, const int cmp(const void *dato1, const void *dato2));
void mostrarLista (const tLista *lista, const void imprimir(const void *dato));

//ordenamiento
void ordenarLista (tLista *lista, const int cmp(const void *dato1, const void *dato2));
int insertarOrdenado (tLista *lista, const void *dato, unsigned tamDato, const int cmp(const void *dato1, const void *dato2));
void ordenarListaSeleccion(tLista *lista, const int cmp(const void *dato1, const void *dato2));
tNodo **buscarMenorNodo(tNodo **reco, const int cmp(const void *dato1,const void *dato2));

//mapeo
void mappearLista (tLista *lista, const void mappeo(const void *dato, const void* dato1));
void imprimirAlumno (const void *dato);

#endif // FIRMATDALISTA_H_INCLUDED
