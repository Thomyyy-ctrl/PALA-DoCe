#include"firmaslistas.h"



void crearLista(tLista*pl)
{
    *pl = NULL;
}

int listaLlena(tLista*pl, unsigned cantBytes)
{
    tNodo*nue = (tNodo*)malloc(sizeof(tNodo));
    void * info = malloc(cantBytes);
    free(nue);
    free(info);
    return info == NULL || nue == NULL;
}

int listaVacia(tLista*pl)
{
    return *pl == NULL;
}

int ponerAlInicioLista(tLista*pl, const void* dato, unsigned cantBytes)
{
    tNodo * nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(!nueNodo || !(nueNodo->info = malloc(cantBytes)))
    {
        free(nueNodo);
        return 0; ///SIN_MEM
    }
    memcpy(nueNodo->info, dato, cantBytes);
    nueNodo->tamInfo = cantBytes;
    nueNodo->sig = *pl;
    *pl = nueNodo;
    return 1;
}

int ponerAlFinalLista(tLista*pl, const void* dato, unsigned cantBytes)
{
    tNodo * nueNodo;
    while(*pl)
        pl = &(*pl)->sig;
    nueNodo = (tNodo*)malloc(sizeof(tNodo));
    if(!nueNodo || !(nueNodo->info = malloc(cantBytes)))
    {
        free(nueNodo);
        return 0; ///SIN_MEM
    }
    memcpy(nueNodo->info, dato, cantBytes);
    nueNodo->tamInfo = cantBytes;
    nueNodo->sig = NULL;
    *pl = nueNodo;
    return 1;
}

int sacarDelInicio(tLista*pl, void* dato, unsigned cantBytes)
{
    tNodo*nodoElim = *pl;
    if(*pl == NULL)
        return 0; ///Lista vacias
    *pl = nodoElim->sig;
    memcpy(dato, nodoElim->info, MINIMO(cantBytes, nodoElim->tamInfo));
    free(nodoElim->info);
    free(nodoElim);
    return 1;
}

int verPrimerElemento(const tLista*pl, void* dato, unsigned cantBytes)
{
    if(*pl == NULL)
        return 0; ///Lista vacias
    memcpy(dato, (*pl)->info, MINIMO(cantBytes, (*pl)->tamInfo));
    return 1;
}

void vaciarLista(tLista*pl)
{
    tNodo*nodoElim;
    while(*pl)
    {
        nodoElim = *pl;
        *pl = nodoElim->sig;
        free(nodoElim->info);
        free(nodoElim);
    }
}
