#include "firmaTdaPila.h"
void crearPila (tPila *pila) {
    *pila=NULL;
}

int pilaVacia (const tPila *pila) {
    //devuelve 1 cuando la pila esta vacia, 0 si no lo esta
    return *pila==NULL;
}

int apilar (tPila *pila, const void *dato, unsigned tam) {
    //verificar
    tNodoPila *nue;
    nue=(tNodoPila*)malloc(sizeof(tNodoPila));
    if(nue==NULL)
        return 0;

    nue->dato=malloc(tam);
    if(nue->dato==NULL) {
        free(nue);
        return 0;
    }

    memcpy(nue->dato,dato,tam);

    nue->tam=tam;
    nue->sig=*pila;
    *pila=nue;
    return 1;
}

int desapilar (tPila *pila, void *dato, unsigned tam)
{
    tNodoPila *elim;
    if(*pila==NULL)
        return 0;

    memcpy(dato,(*pila)->dato,MIN_PILA((*pila)->tam,tam));

    elim=*pila;
    (*pila)=elim->sig;
    free(elim);

    return 1;
}

//pilaLlena debe estar, pero no necesariamente va a ser util, es por temas de compatibilidad
int pilaLlena (const tPila *pila)
{
    //siempre devuelve 0, indica que siempre hay espacio.
    return 0;
}

int verTope (const tPila *pila, void *dato, unsigned tam)
{
    if(*pila==NULL)
        return 0;

    memcpy(dato,(*pila)->dato,MIN_PILA(tam,(*pila)->tam));

    return 1;
}

void vaciarPila (tPila *pila)
{
    while(*pila)
    {
        tNodoPila *elim=*pila;

        *pila=elim->sig;
        free(elim->dato);
        free(elim);
    }
}

