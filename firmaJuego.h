#ifndef FIRMAJUEGO_H_INCLUDED
#define FIRMAJUEGO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "firmaTdaLista.h"
#include "firmaTdaPila.h"
#define TAM_CAD_PL 50
#define TAM_NOMBRE_INFORME 90
#define TAM_BUFFER_FECHA 25
#define MAX_PUNTOS 6
#define HEADER_INFORME "informe-juego_"
#define PATH_CARTAS "cartas.dat"
#define ERROR_ARCH_CARTAS -1
#define ERROR_ARCH_INFORME -6
#define TAM_MANO 3
#define MAS_UNO 1
#define MAS_DOS 2
#define MENOS_UNO -1
#define MENOS_DOS -2
#define ESPEJO 3
#define REPETIR_TURNO 4
#define CARTA_USADA 5
#define CARTA_NULA -5
#define JUGADOR_IA 0
#define JUGADOR_HUMANO 1
#define IA_FACIL 0
#define IA_MEDIO 1
#define IA_DIFICIL 2
#define TAM_NOMBRE_CARTA 20
#define SIN_MEMORIA_JUEGO -3
#define BARAJA_VACIA 0
#define TODO_OK_JUEGO 1
#define TAM_BARAJA 40
#define SIN_EFECTO_NEGATIVO -12

typedef char tCarta;

typedef struct
{
    char puntaje;
    char puntosPreviosAEfectoNegativo; //refleja el puntaje pre
    char ultimaCartaNegativaRecibida; //refleja la carta negativa en si
    unsigned idPlayer; //setear en JUGADOR_IA o JUGADOR_HUMANO acordemente
    char nya[TAM_CAD_PL];
} tPlayer;

typedef struct
{
    tCarta cartaJugada;
    tPlayer jugador;
    unsigned nroTurno;
}tJugada;

typedef tCarta* (*tIA)(const tPila*, const tPlayer *, const tPlayer*, tCarta *mano);

int crearArchivo(const char*path);

int bajarArchLista(const char* path, tLista *pl);

int jugarDoce(tPlayer *jugadorHumano, unsigned char dificultad, tJugada *movimientoGanador);
void mostrarCarta (const void *dato);
void insertarCartaEnMano(tCarta *mano, const tCarta *cartaAInsertar, unsigned pos);
tIA setearIA(unsigned char dificultad);
tCarta* IAFacil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano);
tCarta *juegaHumano (const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano);
int numeroAleatorioEnRango(int minimo, int maximo);
void repartirCartasInicial (tLista *barajaPrincipal,tLista *barajaUsadas, tCarta *manoCpu, tCarta *manoHumano);
const char *obtenerNombreCarta (tCarta carta);
void mostrarMano(const tCarta *mano);
char reponerCarta (tCarta *mano, tLista *barajaPrincipal);
void liberarMemoriaEstructuras (tLista *barajaPrincipal, tLista *barajaUsadas, tPila *historialJugadas);
char reponerBarajaPrincipal (tLista *barajaPrincipal, tLista *barajaUsadas);
int mezclarBaraja(tLista *baraja);
char puntosUltimoEfectoNegativo (tPila *historialJugadas, tPlayer *jugadorContrario);
int generarInforme (tPila *historialJugadas);
void mostrarGanador(tJugada *movimientoGanador);


#endif // FIRMAJUEGO_H_INCLUDED
