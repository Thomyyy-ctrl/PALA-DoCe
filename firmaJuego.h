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

///PUNTOS A ALCANZAR POR PARTIDA
#define MAX_PUNTOS 12

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
#define JUGADOR_IA 0
#define JUGADOR_HUMANO 1
#define IA_FACIL 0
#define NOMBRE_IA_FACIL "IA FACIL"
#define IA_MEDIO 1
#define NOMBRE_IA_MEDIO "IA MEDIO"
#define IA_DIFICIL 2
#define NOMBRE_IA_DIFICIL "IA DIFICIL"
#define IA_PUNTOS_CERCA_GANAR 4 //indica a cuantos puntos de la meta se considera cerca de ganar
#define TAM_NOMBRE_CARTA 20
#define SIN_MEMORIA_JUEGO -3
#define BARAJA_VACIA 0
#define TODO_OK_JUEGO 1
#define TAM_BARAJA 40
#define SIN_EFECTO_NEGATIVO -12
#define TIPO_POSITIVO 7 //hablamos de MAS UNO y MAS DOS
#define TIPO_NEGATIVO -7 //hablamos de MENOS DOS y MENOS UNO
#define VERDADERO 1
#define FALSO 0

#include "firmaConexion.h"
typedef char tCarta;

typedef struct
{
    char puntaje;
    char puntosPreviosAEfectoNegativo; //refleja el puntaje previo
    char ultimaCartaNegativaRecibida; //refleja la carta negativa en si
    unsigned idPlayer; //setear en JUGADOR_IA o JUGADOR_HUMANO acordemente
    char nya[TAM_CAD_PL];
} tPlayer;

typedef struct
{
    tCarta cartaJugada;
    tPlayer jugadorActual;
    tPlayer jugadorRival;
    unsigned nroTurno;
}tJugada;

typedef tCarta* (*tIA)(const tPila*, const tPlayer *, const tPlayer*, tCarta *mano,tCarta* manoJugador, tJugada jugada);

int crearArchivo(const char*path);

int bajarArchLista(const char* path, tLista *pl);

int jugarDoce(tPlayer *jugadorHumano, unsigned char dificultad, tJugada *movimientoGanador,tConfigApi* configuracion);
unsigned char robarCartaOAbandonar ();
void mostrarMensajeRendicion (tPlayer *jugador);
void mostrarCarta (const void *dato);
void insertarCartaEnMano(tCarta *mano, const tCarta *cartaAInsertar, unsigned pos);
tIA setearIA(unsigned char dificultad, char *nombreIa);
tCarta* IAFacil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano, tCarta*manoJugador, tJugada jugada);
tCarta* IAMedio(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano, tCarta*manoJugador, tJugada jugada);
tCarta* IADificil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano,tCarta*manoJugador,tJugada jugada);
tCarta *juegaHumano (const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano, tCarta*manoIA, tJugada jugada);
int numeroAleatorioEnRango(int minimo, int maximo);
void repartirCartasInicial (tLista *barajaPrincipal,tLista *barajaUsadas, tCarta *manoCpu, tCarta *manoHumano);
const char *obtenerNombreCarta (tCarta carta);
void mostrarMano(const tCarta *mano);
char reponerCarta (tCarta *mano, tLista *barajaPrincipal);
void liberarMemoriaEstructuras (tLista *barajaPrincipal, tLista *barajaUsadas, tPila *historialJugadas);
char reponerBarajaPrincipal (tLista *barajaPrincipal, tLista *barajaUsadas);
int mezclarBaraja(tLista *baraja);
int generarInforme (tPila *historialJugadas, unsigned char jugadorAbandonoPartida);
void mostrarGanador(tJugada *movimientoGanador);
void aplicarEfecto (tCarta cartaJugada, tPlayer *jugadorActual, tPlayer *jugadorContrario, unsigned char *repetirTurno);
unsigned char existeTipoDeCartaEnMano (const tCarta *mano, char tipoCarta);
unsigned char cartaEsDeTipo(tCarta carta, char tipoCarta);
unsigned char existeNoTipoDeCartaEnMano (const tCarta *mano, char tipoCarta);
tCarta *cartaQueNoEsDeTipo (tCarta *mano, char tipoCarta);
tCarta *cartaQueSumaMasPuntos(tCarta *mano);
tCarta *cartaRandom(tCarta *mano);
tCarta *cartaNegativaMasAdecuada(tCarta *mano, char puntaje);

tCarta* cartaQuePuedeGanar(tCarta *mano, char puntajeActual) ;
tCarta* obtenerCartaEspejo(tCarta *mano);
tCarta* obtenerCartaRepetirTurno(tCarta *mano);

void mostrarErrorDoce(char codError);
#endif // FIRMAJUEGO_H_INCLUDED
