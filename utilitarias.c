#include "firmaUtilitarias.h"
#include "firmaConexion.h"
#include "firmaJuego.h"
#include "firmaTdaLista.h"
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int menu(tConfigApi* configuracion)
{
    char opcion;
    tLista listaRanking;
    unsigned opcionDificultad;
    int flag;
    char estadoDoce;

    tPlayer jugador;
    tJugada movimientoGanador;

    crearLista(&listaRanking);
    printf("\n\t- Jugar\n\n\t- Ver ranking\n\n\t- Salir\n");
    opcion = obtenerOpcionDeMenu();
    switch(opcion)
    {
        case 'A':
            {
                system(CLEAR);
                /// SELECCION DE LA DIFICULTAD:
                printf("\nDificultad de la IA:\n");
                printf("\n\t- FACIL\n\n\t- MEDIO\n\n\t- DIFICIL\n");
                opcionDificultad = obtenerOpcionDeDificultadIA();
                system(CLEAR);
                /// INGRESO DEL NOMBRE DEL JUGADOR:
                ingresarJugadorNombreYApellido(&jugador);
                system(CLEAR);
                estadoDoce=jugarDoce(&jugador,opcionDificultad,&movimientoGanador,configuracion);
                if(estadoDoce!=TODO_OK_JUEGO)
                    mostrarErrorDoce(estadoDoce);
                flag = 1;
                break;
            }
        case 'B':
            {
                system(CLEAR);
                ///Obtenemos los datos de la Api para realizar el ranking.
                if(obtenerRanking(&listaRanking, configuracion))
                {
                    if(!listaVacia(&listaRanking))///verifico si la lista esta vacia, sino lo esta muestro el ranking.
                    {
                        imprimirEncabezadoRanking();
                        mappearLista(&listaRanking, mostrarJugadorApi);//modificar
                    }
                    else
                        printf("No hay datos cargados en el ranking.\n");
                }
                printf("\nPresionar enter para avanzar al menu...");
                fflush(stdout);
                getchar();
                vaciarLista(&listaRanking);
                flag = 1;
                break;
            }
        case 'C':
            {
                flag = 0;
                break;
            }
        default:
            break;
    }
    return flag;
}
void ingresarJugadorNombreYApellido(tPlayer* jugador)
{
    jugador->puntaje=0;
    jugador->puntosPreviosAEfectoNegativo=0;
    jugador->ultimaCartaNegativaRecibida= SIN_EFECTO_NEGATIVO;
    jugador->idPlayer=JUGADOR_HUMANO;
    printf("\nIngresa tu nombre y apellido: ");
    fflush(stdin);
    fgets(jugador->nya, sizeof(jugador->nya), stdin);
    jugador->nya[strcspn(jugador->nya, "\n")] = '\0';
}

int mostrarTablero(tCarta* manoJugador,tCarta* manoIA,tCarta* ultimaCartaJugada, tJugada jugada)
{
    system(CLEAR);
    printf("\t     #===========================================#\n");
    printf("\t                  TABLERO DE JUEGO\n");
    printf("\t     #===========================================#\n\n\n");
    mostrarManoIA(manoIA);
    mostrarCartaActual(ultimaCartaJugada);
    mostrarManoJugador(manoJugador);
    printf("\n\tPUNTOS JUGADOR: %d\n",(jugada.jugadorActual.idPlayer == JUGADOR_HUMANO)?jugada.jugadorActual.puntaje : jugada.jugadorRival.puntaje);
    printf("\tPUNTOS IA: %d\n\n",(jugada.jugadorActual.idPlayer == JUGADOR_IA) ? jugada.jugadorActual.puntaje : jugada.jugadorRival.puntaje);
    return TODO_OK;
}

void mostrarManoIA(tCarta* c)
{
    // Primera línea superior
    printf("\t\t");
    for (int i = 0; i < CARTAS; i++) {
        if (c[i]!='0' && c[i] != CARTA_USADA)
            printf("+----------+ ");
        else
            printf("             ");
    }
    printf("\tCartas de la IA\n");
    printf("\t\t");
    // Línea del contenido
    for (int i = 0; i < CARTAS; i++) {
        if (c[i]!='0' && c[i] != CARTA_USADA)
           //printf("| %-8s | ",obtenerNombreCarta(c[i]));
            printf("| %-8s | "," ");
        else
            printf("             ");
    }
    printf("\n");
    printf("\t\t");
    // Línea inferior
    for (int i = 0; i < CARTAS; i++) {
        if (c[i]!='0' && c[i] != CARTA_USADA)
            printf("+----------+ ");
        else
            printf("             ");
    }
    printf("\n");
}
void mostrarManoJugador(tCarta* c)
{
    // Primera línea superior
    printf("\t\t");
    for (int i = 0; i < CARTAS; i++) {
        if (c[i]!='0' && c[i] != CARTA_USADA)
            printf("+----------+ ");
        else
            printf("             ");
    }
    printf("\tCartas de la IA\n");
    printf("\t\t");
    // Línea del contenido
    for (int i = 0; i < CARTAS; i++) {
        if (c[i]!='0' && c[i] != CARTA_USADA)
            printf("| %-8s | ",obtenerNombreCarta(c[i]));
        else
            printf("             ");
    }
    printf("\n");
    printf("\t\t");
    // Línea inferior
    for (int i = 0; i < CARTAS; i++) {
        if (c[i]!='0' && c[i] != CARTA_USADA)
            printf("+----------+ ");
        else
            printf("             ");
    }
    printf("\n");
}

void mostrarCartaActual(tCarta* c)
{
    printf("\n");
    if (*c != '0')
    {
        printf("\t\t\t\t+----------+\tUltima Carta\n");
        printf("\t\t\t\t| %-8s | \n",obtenerNombreCarta(*c));
        printf("\t\t\t\t+----------+\n");
    }
    else
    {
        printf("             \n");
        printf("             \n");
        printf("             \n");
    }
    printf("\n");
}
int obtenerCartaDeLaMano()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD eventos;
    INPUT_RECORD ir;
    int x,y;

    // Habilita la captura de eventos de mouse
    SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

    while (1)
    {
        ReadConsoleInput(hIn, &ir, 1, &eventos);
        // Si se detecta un evento de mouse y se presiona el botón izquierdo
        if (ir.EventType == MOUSE_EVENT && ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && ir.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)
        {
            x = ir.Event.MouseEvent.dwMousePosition.X;
            y = ir.Event.MouseEvent.dwMousePosition.Y;
            //printf("%d,%d\n",x,y);
            if(y >=13 && y <=15)
            {
                if(x>=16 && x<=27)
                {
                    return 1;
                }
                if(x>=29 && x<=40)
                {
                    return 2;
                }
                if(x>=42 && x<=53)
                {
                    return 3;
                }
            }
        }
    }
}

char obtenerOpcionDeMenu()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD eventos;
    INPUT_RECORD ir;
    int x,y;

    // Habilita la captura de eventos de mouse
    SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

    while (1)
    {
        ReadConsoleInput(hIn, &ir, 1, &eventos);
        // Si se detecta un evento de mouse y se presiona el botón izquierdo
        if (ir.EventType == MOUSE_EVENT && ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && ir.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)
        {
            x = ir.Event.MouseEvent.dwMousePosition.X;
            y = ir.Event.MouseEvent.dwMousePosition.Y;
            //printf("x: %d  y: %d\n", x,y);
            if(x>=9 && x<=16 && y == 1)
            {
                return 'A';
            }
            if(x>=9&& x<=21 && y == 3)
            {
                return 'B';
            }
            if(x>=9&& x<=15 && y == 5)
            {
                return 'C';
            }
        }
    }
}
unsigned obtenerOpcionDeDificultadIA()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD eventos;
    INPUT_RECORD ir;
    int x,y;

    // Habilita la captura de eventos de mouse
    SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

    while (1)
    {
        ReadConsoleInput(hIn, &ir, 1, &eventos);
        // Si se detecta un evento de mouse y se presiona el botón izquierdo
        if (ir.EventType == MOUSE_EVENT && ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && ir.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)
        {
            x = ir.Event.MouseEvent.dwMousePosition.X;
            y = ir.Event.MouseEvent.dwMousePosition.Y;
            //printf("x: %d  y: %d\n", x,y);
            if(x>=8 && x<=14 && y == 3)
            {
                return 0;
            }
            if(x>=8&& x<=14 && y == 5)
            {
                return 1;
            }
            if(x>=8&& x<=16 && y == 7)
            {
                return 2;
            }
        }
    }
}

int obtenerRanking(tLista* lista,tConfigApi* configuracion)
{
    CURLcode res;
    tJugadorAPI jugador;
    tRespuesta resAPI  = {NULL, 0};
    char pathGet[TAM_MAX_JSON];
    snprintf(pathGet, sizeof(pathGet), "%s/%s", configuracion->urlApi, configuracion->codGrupo);

    res = peticionGET(&resAPI, pathGet);
    if (res != CURLE_OK){
        printf("Error en la solicitud a la API.\n");
        return ERROR;
    }
    else{
        while(parsearJugadores(&resAPI, &jugador))
            insertarOrdenado(lista, &jugador, sizeof(tJugadorAPI), compararJugadoresApi); //modificar
    }
    free(resAPI.info);
    return TODO_OK;
}
void imprimirEncabezadoRanking()
{
    printf("-------------------------------------------------------\n");
    printf("| %-8s | %-15s | %-20s |\n","Pos","Nombre","Total Partidas Ganadas");
    printf("-------------------------------------------------------\n");

}
const void mostrarJugadorApi(const void* dato, const void* dato1)
{
    tJugadorAPI* a =(tJugadorAPI*)dato;
    int* b= (int*)dato1;
    printf("\n  %-8d   %-15s   %-20d  \n",*b,a->nombre,a->gano);
}
const int compararJugadoresApi(const void* a, const void* b)
{
    tJugadorAPI * a1 = (tJugadorAPI*)a;
    tJugadorAPI * b1 = (tJugadorAPI*)b;
    return b1->gano - a1->gano ;
}
int parsearJugadores(tRespuesta *res, tJugadorAPI *jugador)
{
    //modificado porque nuestra API enviaba la respuesta en formato partidasGanadas , nombreJugador
    //la API del TP lo envia como nombreJugador , partidasGanadas.
    //revisar cambios, creo que estan correctos - Nico
    char *p = strrchr(res->info, '}');

    if(!p)
        return 0;

    *p = '\0';
    p = strrchr(res->info, ':');
    sscanf(p + 1, "%u", &jugador->gano);
    p = strrchr(res->info, ',');

    p--;
    *p = '\0';
    p = strrchr(res->info, ':');
    sscanf(p + 2, "%s", jugador->nombre);
    p = strrchr(res->info, '{');

    *p = '\0';
    return 1;
}

void pausarConsola() {
    printf("Presiona Enter para continuar...");
    while (getchar() != '\n');  // Espera a que aprieten Enter
}
