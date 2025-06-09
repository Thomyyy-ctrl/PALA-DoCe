#ifndef FIRMAUTILITARIAS_H_INCLUDED
#define FIRMAUTILITARIAS_H_INCLUDED

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#include "firmaJuego.h"
#include "firmaConexion.h"
#define ARCHIVO_CONFIG 1
#define TODO_OK 1
#define ERROR_ARCH 2
#define CARTAS 3


int menu(tConfigApi* configuracion);
char obtenerOpcionDeMenu();
unsigned obtenerOpcionDeDificultadIA();
int subirDatosApi(tConfigApi* configuracion);
int obtenerRanking(tLista* listaRanking,tConfigApi* configuracion);
void imprimirEncabezadoRanking();
int parsearJugadores(tRespuesta *res, tJugadorAPI *jugador);
void pausarConsola();

///AGREGADO:
int mostrarTablero(tCarta* manoJugador,tCarta* manoIA,tCarta* ultimaCartaJugada,tJugada jugada);
void ingresarJugadorNombreYApellido(tPlayer* jugador);
void mostrarManoIA(tCarta* c);
void mostrarCartaActual(tCarta* c);
void mostrarManoJugador(tCarta* c);
int obtenerCartaDeLaMano();
const int compararJugadoresApi(const void* a, const void* b);
const void mostrarJugadorApi(const void* dato, const void* dato1);
#endif // FIRMAUTILITARIAS_H_INCLUDED
