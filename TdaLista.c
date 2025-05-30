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

int insertarPosicionN (tLista *lista, const void *dato, unsigned tamDato, unsigned posicion)
{
    tNodo *nue;
    unsigned posLista=0;
    //por convencion, diremos que el primer nodo es el nodo 0
    if((int)posicion<0) //posicion invalida
        return 0;
    if(*lista==NULL) //si la lista esta vacia no hay posicion en la cual insertar
        return 0;
    while((*lista)->sig&&posLista<posicion)
    {
        lista=&(*lista)->sig;
        posLista++;
    }
    if(posLista!=posicion) //posicion invalida / no existe
        return 0;

    //creamos el nodo ya que sabemos que existe la posicion
    nue=malloc(sizeof(tNodo));
    if(nue==NULL)
        return 0;
    nue->dato=malloc(tamDato);
    if(nue->dato==NULL)
        return 0;

    memcpy(nue->dato,dato,tamDato);
    nue->tamDato=tamDato;
    //asignamos

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

int sacarPosicionN (tLista *lista, void *dato, unsigned tamDato, unsigned posicion)
{
    unsigned posLista=0;
    tNodo *elim; //la variable elim es inescapable, si o si debe usarse en este caso.
    if(*lista==NULL) //lista vacia
        return 0;

    if((int)posicion<0) //posicion negativa / invalida
        return 0;

    while((*lista)->sig&&posLista<posicion)
    {
        lista=&(*lista)->sig;
        posLista++;
    }

    if(posLista!=posicion) //la posicion no existia en la lista
        return 0;
    elim=*lista;

    //copiamos el dato
    memcpy(dato,(*lista)->dato,MIN(tamDato,(*lista)->tamDato));

    //primero actualizamos el nodo anterior para enlazarlo al siguiente nuevo
    *lista=(*lista)->sig;

    //liberamos el nodo
    free(elim->dato);
    free(elim);

    return 1;
}

int sacarPorClaveOL (tLista *lista, void *dato, unsigned tamDato, const int cmp (const void *dato1, const void *dato2))
{
    //util para listas ordenadas solamente
    //quita el primer elemento con la clave indicada
    tNodo *elim;
    if(*lista==NULL)
        return LISTA_VACIA; //lista estaba vacia

    while(*lista&&cmp((*lista)->dato,dato)<0)
        lista=&(*lista)->sig;

    if(*lista&&cmp((*lista)->dato,dato)!=0)
        return NO_ENCONTRADO; //el elemento no existe en la lista

    elim=*lista;

    memcpy(dato,elim->dato,MIN(tamDato,elim->tamDato));

    *lista=(*lista)->sig;

    free(elim);

    return OK;
}

int sacarPorClaveUL (tLista *lista, void *dato, unsigned tamDato, const int cmp(const void *dato1, const void *dato2))
{
    //sirve para listas desordenadas y ordenadas
    //quita el primer elemento encontrado con la clave indicada
    tNodo *elim;
    if(*lista==NULL)
        return LISTA_VACIA;

    while(*lista&&cmp((*lista)->dato,dato)!=0)
        lista=&(*lista)->sig;

    if(!*lista)
        return NO_ENCONTRADO;

    elim=*lista;
    *lista=elim->sig;

    memcpy(dato,elim->dato,MIN(tamDato,elim->tamDato));
    free(elim);
    return OK;
}

int eliminarDuplicadosPorClaveUL(tLista *lista, void *datoAccion, const int cmp(const void *dato1, const void *dato2), void accion(void *datoAccion, void *dato2),int realizarAccion)
{
    //elimina todos los duplicados especificados por datoAccion, lista desordenada u ordenada
    //permite realizar una accion utilizando datoClave y los que se vayan extrayendo de la lista
    unsigned char primerInstancia=1,nodoFueEliminado=0;
    if(*lista==NULL)
        return LISTA_VACIA;
    while(*lista)
    {
        nodoFueEliminado=0;
        if(cmp((*lista)->dato,datoAccion)==0)
        {
            if(!primerInstancia)
            {
                tNodo *elim;

                elim=*lista;
                if(realizarAccion)
                    accion(datoAccion,elim->dato);
                *lista=(*lista)->sig;
                free(elim);
                nodoFueEliminado=1;
            }
            else
                primerInstancia=0;
        }
        if(!nodoFueEliminado)
            lista=&(*lista)->sig;
    }
    return OK;
}

//funciones de vista

int verPrimero(const tLista *lista, void *dato, unsigned tamDato)
{
    if(*lista==NULL) //lista vacia
        return 0;
    memcpy(dato,(*lista)->dato,MIN(tamDato,(*lista)->tamDato));
    return 1;
}

int verUltimo (const tLista *lista, void *dato, unsigned tamDato)
{
    if(*lista==NULL)
        return 0;
    while((*lista)->sig)
        lista=&(*lista)->sig;
    memcpy(dato,(*lista)->dato,MIN(tamDato,(*lista)->tamDato));
    return 1;
}

int verPosicionN (const tLista *lista, void *dato, unsigned tamDato, unsigned posicion)
{
    unsigned posLista=0;
    if(*lista==NULL) //lista vacia
        return 0;

    if((int)posicion<0) //posicion negativa
        return 0;

    while((*lista)->sig&&posLista<posicion)
    {
        lista=&(*lista)->sig;
        posLista++;
    }

    if(posLista!=posicion) //la posicion no existia en la lista
        return 0;

    memcpy(dato,(*lista)->dato,MIN(tamDato,(*lista)->tamDato));
    return 1;
}


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

//primitivas de ordenamiento o relacionados
void ordenarLista (tLista *lista, const int cmp(const void *dato1, const void *dato2))
{
    //El algoritmo implementado es burbujeo, aclaro que seguramente no sea optimo, lo hice sin buscar nada online
    tNodo *nodoAnt=NULL,*nodoAct=*lista,*nodoSig=NULL,*auxDir=NULL;
    unsigned cantNodos=0,cortar=0,x=0,y;
    if(*lista==NULL)
        return;
    //ver la cantidad de nodos
    while(nodoAct!=NULL)
    {
        nodoAct=nodoAct->sig;
        cantNodos++;
    }
    if(cantNodos==1)
        return;
    nodoAct=*lista;
    while(!cortar&&x<cantNodos-1)
    {
        cortar=1;
        for(y=0; y<cantNodos-1; y++)
        {
            nodoSig=nodoAct->sig;
            if(cmp(nodoAct->dato,nodoSig->dato)>0)
            {
                cortar=0;
                if(nodoAct==*lista) //caso donde intentamos swapear los primeros dos elementos de la lista
                {
                    nodoAct->sig=nodoSig->sig;
                    nodoSig->sig=nodoAct;
                    *lista=nodoSig;
                }
                else if(nodoSig->sig==NULL) //caso donde intentamos swapear los ultimos dos elementos de la lista
                {
                    nodoSig->sig=nodoAct;
                    nodoAnt->sig=nodoSig;
                    nodoAct->sig=NULL;
                }
                else //caso donde intentamos swapear dos elementos intermedios de la lista
                {
                    nodoAnt->sig=nodoSig;
                    nodoAct->sig=nodoSig->sig;
                    nodoSig->sig=nodoAct;
                }
                //actualizamos los punteros
                auxDir=nodoAct;
                nodoAct=nodoSig;
                nodoSig=auxDir;
            }
            nodoAnt=nodoAct;
            nodoAct=nodoAct->sig;
        }
        nodoAnt=NULL;
        nodoSig=NULL;
        nodoAct=*lista;
    }
}

//version Limpia
void ordenarListaSeleccion(tLista *lista, const int cmp(const void *dato1, const void *dato2))
{
    tLista *dirMen, *subLis;
    tNodo *men;
    subLis = lista;
    while(*subLis)
    {
        dirMen = buscarMenorNodo(subLis, cmp);
        if(dirMen != subLis)
        {
            men = *dirMen;
            *dirMen = men->sig;
            men->sig = *subLis;
            *subLis = men;
        }
        subLis = &(*subLis)->sig;
    }
}

tNodo **buscarMenorNodo(tNodo **subLis, const int cmp(const void *dato1,const void *dato2))
{
    tNodo **menor;
    tNodo **reco=subLis;
    menor=reco;
    while(*reco)
    {
        if(cmp((*reco)->dato,(*menor)->dato)<0)
            menor=reco;
        reco=&(*reco)->sig;
    }
    return menor;
}

void imprimirAlumno (const void *dato)
{
    tAlumno *alumno=(tAlumno *)dato;
    printf("Nombre: %s, DNI: %d, Nota: %d\n",alumno->nombre,alumno->dni,alumno->nota);
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

void mappearLista (tLista *lista, const void mappeo(const void *dato))
{
    while(*lista)
    {
        mappeo((*lista)->dato);
        lista=&(*lista)->sig;
    }
}

int verPorClave(const tLista *lista, void *dato, unsigned tamDato, const void *clave, const int cmp(const void *dato1, const void *dato2))
{
    //te muestra la primera aparicion del nodo que cumpla la condicion recorrido de inicio a fin
    if(*lista==NULL)
        return 0;
    while(*lista&&cmp((*lista)->dato,clave)!=0)
        lista=&(*lista)->sig;
    if(*lista==NULL)
        return 0;
    memcpy(dato,(*lista)->dato,MIN(tamDato,(*lista)->tamDato));
    return 1;
}
