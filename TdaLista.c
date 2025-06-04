#include "firmaTdaLista.h"
void crearLista (tLista *lista)
{
    *lista=NULL;
}

int listaLlena(const tLista *lista)
{
    //diremos que la lista nunca esta llena
    return 0;
}

int listaVacia (const tLista *lista)
{
    //devuelve 1 si la lista esta vacia y 0 sino.
    return *lista==NULL;
}

void vaciarLista (tLista *lista)
{
    while(*lista!=NULL)
    {
        tNodo *aux;

        aux=(*lista);
        *lista=aux->sig;
        free(aux->dato);
        free(aux);
    }
}

//funciones de insercion
int insertarAlInicio(tLista *lista, const void *dato, unsigned tamDato)
{
    tNodo *nue;
    nue=malloc(sizeof(tNodo));
    if(nue==NULL)
        return 0;

    nue->dato=malloc(tamDato);
    if(nue->dato==NULL)
        return 0;

    memcpy(nue->dato,dato,tamDato);
    nue->tamDato=tamDato;

    nue->sig=*lista;
    *lista=nue;

    return 1;
}

int insertarAlFinal (tLista *lista, const void *dato, unsigned tamDato)
{
    tNodo *nue;
    nue=malloc(sizeof(tNodo));
    if(nue==NULL)
        return 0;

    nue->dato=malloc(tamDato);
    if(nue->dato==NULL)
    {
        free(nue);
        return 0;
    }

    memcpy(nue->dato,dato,tamDato);
    nue->tamDato=tamDato;

    nue->sig=NULL;

    //lista apunta a la direccion del siguiente nodo, si este llega a ser el ultimo apunta a la direccion donde se guarda un null
    //entonces hacer *lista daria NULL, pero como no estamos desreferenciandolo no daria seg fault, de esta forma nos ahorramos
    //la verificacion de el caso donde la lista este vacia.

    while(*lista) //mientras *lista no sea el ultimo nodo
        lista=&(*lista)->sig; //usamos el puntero lista para recorrer

    *lista=nue;

    return 1;
}

int insertarOrdenado (tLista *lista, const void *dato, unsigned tamDato, const int cmp(const void *dato1, const void *dato2))
{
    tNodo *nue;
    nue=malloc(sizeof(tNodo));
    if(nue==NULL) // falta mem
        return 0;
    nue->dato=malloc(tamDato);
    if(nue->dato==NULL) //falta mem
        return 0;

    memcpy(nue->dato,dato,tamDato);
    nue->tamDato=tamDato;

    while(*lista&&cmp((*lista)->dato,nue->dato)<0)
        lista=&(*lista)->sig;

    nue->sig=*lista;
    *lista=nue;
    return 1;
}

//funciones de sacar
int sacarPrimero(tLista *lista,void *dato, unsigned tamDato)
{
    tNodo *elim;
    if(*lista==NULL) //lista vacia
        return 0;
    elim=*lista;
    memcpy(dato,elim->dato,MIN(tamDato,elim->tamDato));
    *lista=elim->sig;
    free(elim->dato);
    free(elim);
    return 1;
}

int sacarUltimo (tLista *lista, void *dato, unsigned tamDato)
{
    tNodo *elim;
    if(*lista==NULL) //lista vacia
        return 0;
    while((*lista)->sig) //avanzamos hasta que apuntemos al ultimo nodo
        lista=&(*lista)->sig;
    elim=*lista;

    memcpy(dato,(*lista)->dato,MIN(tamDato,(*lista)->tamDato)); //copiamos los datos

    free(elim->dato);
    free(elim); //liberamos el nodo

    *lista=NULL;

    return 1;
}




//funciones de vista

void mostrarLista (const tLista *lista, const void imprimir(const void *dato))
{
    puts("---------------LISTA---------------------");
    if(*lista==NULL)
    {
        puts("La lista se encuentra vacia.");
        return;
    }
    while(*lista)
    {
        imprimir((*lista)->dato);
        lista=&(*lista)->sig;
    }
    puts("----------------------------------------");
}

//mappeo de lista
void mappearLista (tLista *lista, const void mappeo(const void *dato, const void* dato1))
{
    int i=1;
    while(*lista)
    {
        mappeo((*lista)->dato,&i);
        lista=&(*lista)->sig;
        i++;
    }
}
