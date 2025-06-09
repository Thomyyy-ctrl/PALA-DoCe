#include"firmaJuego.h"
#include "firmaUtilitarias.h"
#include "firmaConexion.h"
int crearArchivo(const char*path)
{
    tCarta card[] =  ///6 de +2
    {
        MAS_DOS,MAS_DOS,MAS_DOS,MAS_DOS,MAS_DOS,MAS_DOS,
        ///10 de +1
        MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,MAS_UNO,
        ///8 de -1
        MENOS_UNO,MENOS_UNO,MENOS_UNO,MENOS_UNO,MENOS_UNO,MENOS_UNO,MENOS_UNO,MENOS_UNO,
        ///6 de -2
        MENOS_DOS,MENOS_DOS,MENOS_DOS,MENOS_DOS,MENOS_DOS,MENOS_DOS,
        ///6 repetir turno
        REPETIR_TURNO,REPETIR_TURNO,REPETIR_TURNO,REPETIR_TURNO,REPETIR_TURNO,REPETIR_TURNO,
        ///4 espejo
        ESPEJO,ESPEJO,ESPEJO,ESPEJO
    };
    FILE*arch = fopen(path, "wb");
    if(!arch)
        return ERROR_ARCH_CARTAS;
    fwrite(card, sizeof(tCarta), sizeof(card)/ sizeof(tCarta),arch);
    fclose(arch);
    return TODO_OK_JUEGO;
}

int bajarArchLista(const char* path, tLista *pl)
{
    tCarta card;
    FILE*arch = fopen(path, "rb");
    if(!arch)
        return ERROR_ARCH_CARTAS;
    fread(&card, sizeof(tCarta),1, arch);
    while(!feof(arch))
    {
        if(!insertarAlFinal(pl,&card,sizeof(tCarta)))
        {
            fclose(arch);
            return SIN_MEMORIA_JUEGO;
        }
        fread(&card, sizeof(tCarta),1, arch);
    }
    fclose(arch);
    return TODO_OK_JUEGO;
}

int jugarDoce(tPlayer *jugadorHumano, unsigned char dificultad, tJugada *movimientoGanador,tConfigApi* configuracion)
{
    //deberia recibir el jugador con su nombre, tipo de jugador (HUMANO) seteado, y puntaje en 0
    //dificultad es un unsigned char que representa el nivel de la IA, 0 facil, 1 medio, 2 dificil (definido por macro)
    tCarta manoHumano[TAM_MANO],manoCpu[TAM_MANO],*cartaJugada,*manoActual;
    unsigned char repetirTurno=0,jugadorAbandonoPartida=FALSO;
    tLista barajaPrincipal, barajaUsadas;
    tIA juegaIA;
    tJugada jugada;
    tPila historialJugadas;
    tPlayer jugadorCpu= {0,0,SIN_EFECTO_NEGATIVO,JUGADOR_IA,"IA"};
    tPlayer *jugadorActual=NULL,*jugadorContrario=NULL;
    //creamos las dos barajas principales
    crearLista(&barajaPrincipal);
    crearLista(&barajaUsadas);
    //creamos la lista del historial de jugadas
    crearPila(&historialJugadas);
    //cargamos las cartas
    if(crearArchivo(PATH_CARTAS)!=TODO_OK_JUEGO)
        return ERROR_ARCH_CARTAS; ///no hace falta liberar memoria ya que no se cargaron las estructuras
    if(bajarArchLista(PATH_CARTAS,&barajaPrincipal)!=TODO_OK_JUEGO)
        return ERROR_ARCH_CARTAS; ///no hace falta liberar memoria ya que no se cargaron las estructuras
    //hacemos la mezcla inicial
    mezclarBaraja(&barajaPrincipal);

    //seteamos la IA en base a la dificultad seleccionada
    juegaIA=setearIA(dificultad,jugadorCpu.nya);

    //repartimos las cartas iniciales
    repartirCartasInicial(&barajaPrincipal,&barajaUsadas,manoCpu,manoHumano);
    if(numeroAleatorioEnRango(0,1)==1)
    {
        jugadorActual=&jugadorCpu;
        jugadorContrario=jugadorHumano;
        manoActual=manoCpu;
    }
    else
    {
        jugadorActual=jugadorHumano;
        jugadorContrario=&jugadorCpu;
        manoActual=manoHumano;
    }
    //indicamos el numero de jugada
    jugada.nroTurno=1;
    jugada.jugadorActual.puntaje=0;
    jugada.jugadorRival.puntaje=0;
    while(jugadorAbandonoPartida==FALSO&&jugadorCpu.puntaje<MAX_PUNTOS&&jugadorHumano->puntaje<MAX_PUNTOS)
    {
        //se realiza la jugada
        if(jugadorActual->idPlayer==JUGADOR_HUMANO)
            cartaJugada=juegaHumano(&historialJugadas,jugadorHumano,&jugadorCpu,manoHumano,manoCpu,jugada);
        else
            cartaJugada=juegaIA(&historialJugadas,jugadorHumano,&jugadorCpu,manoCpu,manoHumano,jugada);

        //se aplica el efecto
        aplicarEfecto(*cartaJugada,jugadorActual,jugadorContrario,&repetirTurno);
        //se registra la jugada en el historial ( tanto el estado del jugador actual como el de su rival )
        jugada.cartaJugada=*cartaJugada;
        jugada.jugadorActual=*jugadorActual;
        jugada.jugadorRival=*jugadorContrario;
        if(!apilar(&historialJugadas,&jugada,sizeof(tJugada)))
        {
            liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
            return SIN_MEMORIA_JUEGO;
        }
        //insertamos la carta jugada en la baraja de usadas
        if(!insertarAlInicio(&barajaUsadas,cartaJugada,sizeof(tCarta)))
        {
            liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
            return SIN_MEMORIA_JUEGO;
        }
        //marcamos la carta jugada como usada
        *cartaJugada=CARTA_USADA;
        if(jugadorActual == &jugadorCpu)
        {
            if(jugadorActual->puntaje>=MAX_PUNTOS)
            {
                mostrarTablero(manoHumano,manoCpu,&jugada.cartaJugada,jugada);
                Sleep(1000);
            }
            else
            {
                mostrarTablero(manoHumano,manoCpu,&jugada.cartaJugada,jugada);
                printf("\n\t  La IA ROBA UNA CARTA...");
                Sleep(2000);
            }

        }
        else
        {
            if(jugadorActual->puntaje>=MAX_PUNTOS)
            {
                mostrarTablero(manoHumano,manoCpu,&jugada.cartaJugada,jugada);
                Sleep(2000);
            }
            else
            {
                mostrarTablero(manoHumano,manoCpu,&jugada.cartaJugada,jugada);
                fflush(stdin);
                jugadorAbandonoPartida=robarCartaOAbandonar();
                fflush(stdin);
            }

        }

        if(jugadorAbandonoPartida==FALSO)
        {
            //reponemos la carta en la mano del jugador que tiro recien
            if(reponerCarta(manoActual,&barajaPrincipal)==BARAJA_VACIA)
            {
                if(reponerBarajaPrincipal(&barajaPrincipal,&barajaUsadas)==SIN_MEMORIA_JUEGO)
                {
                    liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
                    return SIN_MEMORIA_JUEGO;
                }
                system(CLEAR);
                printf("\n\t  MEZCLANDO EL MAZO...");
                Sleep(1500);
                if(mezclarBaraja(&barajaPrincipal)==SIN_MEMORIA_JUEGO)
                {
                    liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
                    return SIN_MEMORIA_JUEGO;
                }
                reponerCarta(manoActual,&barajaPrincipal);
            }
            //cambiamos de jugador
            if(!repetirTurno)
            {
                jugadorActual->ultimaCartaNegativaRecibida=SIN_EFECTO_NEGATIVO;
                if(jugadorActual->idPlayer==JUGADOR_IA)
                {
                    jugadorActual=jugadorHumano;
                    jugadorContrario=&jugadorCpu;
                    manoActual=manoHumano;
                }
                else
                {
                    jugadorActual=&jugadorCpu;
                    jugadorContrario=jugadorHumano;
                    manoActual=manoCpu;
                }
            }
            else
                repetirTurno=0;
            //aumentamos el turno
            jugada.nroTurno++;
        }
    }

    if(jugadorAbandonoPartida==VERDADERO)
    {
        //Si el jugador abandona la partida lo indicamos con un mensaje, generamos el informe, pero no enviaremos
        //nada a la API ya que no se considera a ninguno como ganador porque la partida no llego a desarollarse.
        /// se adoptó este criterio, soy consciente de que existen otros.
        //en el informe indicamos que el jugador abandono la partida.
        mostrarMensajeRendicion(jugadorHumano);
        if(generarInforme(&historialJugadas,jugadorAbandonoPartida)!=TODO_OK_JUEGO)
        {
            liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
            return ERROR_ARCH_INFORME;
        }
        liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
        return TODO_OK_JUEGO;

    }
    //guardamos el movimiento ganador para devolverlo
    verTope(&historialJugadas,movimientoGanador,sizeof(tJugada));
    mostrarGanador(movimientoGanador);
    if(generarInforme(&historialJugadas,jugadorAbandonoPartida)!=TODO_OK_JUEGO)
    {
        liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
        return ERROR_ARCH_INFORME;
    }
    liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);

    //Subo el resultado de la partida del jugador contra la IA
    enviarDatosJSON(movimientoGanador,configuracion);

    return TODO_OK_JUEGO;
}

unsigned char robarCartaOAbandonar ()
{
    char carLeido;
    puts("PRESIONE ENTER PARA ROBAR UNA CARTA O INGRESE 'X' PARA ABANDONAR LA PARTIDA");
    carLeido=getchar();
    if(carLeido=='x'||carLeido=='X')
        return VERDADERO;
    return FALSO;
}

void aplicarEfecto (tCarta cartaJugada, tPlayer *jugadorActual, tPlayer *jugadorContrario, unsigned char *repetirTurno)
{
    switch(cartaJugada)
    {
    case MAS_DOS:
    {
        jugadorActual->puntaje+=2;
    }
    break;
    case MENOS_DOS:
    {
        jugadorContrario->puntosPreviosAEfectoNegativo=jugadorContrario->puntaje;
        jugadorContrario->ultimaCartaNegativaRecibida=MENOS_DOS;
        (jugadorContrario->puntaje)-=2;
        if(jugadorContrario->puntaje<0)
            jugadorContrario->puntaje=0;
    }
    break;
    case MAS_UNO:
    {
        jugadorActual->puntaje++;
    }
    break;
    case MENOS_UNO:
    {
        jugadorContrario->puntosPreviosAEfectoNegativo=jugadorContrario->puntaje;
        jugadorContrario->ultimaCartaNegativaRecibida=MENOS_UNO;
        jugadorContrario->puntaje--;
        if(jugadorContrario->puntaje<0)
            jugadorContrario->puntaje=0;
    }
    break;
    case REPETIR_TURNO:
    {
        *repetirTurno=1;
    }
    break;
    case ESPEJO:
    {
        //modificacion hecha por laion (recuperado de github)
        if(jugadorActual->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO)
        {
            tCarta efectoAReflejar = jugadorActual->ultimaCartaNegativaRecibida;
            // Restaurar puntos del jugador que usa ESPEJO
            if(jugadorActual->puntaje < jugadorActual->puntosPreviosAEfectoNegativo)
                jugadorActual->puntaje = jugadorActual->puntosPreviosAEfectoNegativo;

            // Preparar al oponente para recibir el efecto reflejado
            jugadorContrario->puntosPreviosAEfectoNegativo = jugadorContrario->puntaje;
            jugadorContrario->ultimaCartaNegativaRecibida = efectoAReflejar;

            // Aplicar el efecto reflejado al oponente
            switch(efectoAReflejar)
            {
            case MENOS_UNO:
            {
                jugadorContrario->puntaje--;
                if(jugadorContrario->puntaje < 0)
                    jugadorContrario->puntaje = 0;
                break;
            }
            case MENOS_DOS:
            {
                jugadorContrario->puntaje -= 2;
                if(jugadorContrario->puntaje < 0)
                    jugadorContrario->puntaje = 0;
                break;
            }
            }
            // Limpiar el efecto del jugador que usó ESPEJO
            //jugadorActual->ultimaCartaNegativaRecibida = SIN_EFECTO_NEGATIVO;
            ///esta linea de arriba se hace mas adelante, no aca.
        }
    }
    break;
    default:
        puts("Carta sin efecto, funcion no implementada");
        break;
    }
}

void mostrarMensajeRendicion (tPlayer *jugador)
{
    system(CLEAR);
    printf("\n\t     ==============PARTIDA FINALIZADA==================\n");
    printf("El jugador %s ha decidido abandonar la partida. :(\n",jugador->nya);
    puts("===============================================================");
    pausarConsola();
    system(CLEAR);
}

void mostrarGanador(tJugada *movimientoGanador)
{
    system(CLEAR);
    printf("\n\t     ==============PARTIDA FINALIZADA==================\n");
    printf("\n\t Ganador: %s con %d puntos en el turno nro %d con la carta %s.\n",movimientoGanador->jugadorActual.nya,movimientoGanador->jugadorActual.puntaje,movimientoGanador->nroTurno,obtenerNombreCarta(movimientoGanador->cartaJugada));
    if(movimientoGanador->jugadorActual.idPlayer==JUGADOR_IA)
        printf("\n\t     ==========MAS SUERTE LA PROXIMA (:/)==============\n\n\n\n");
    else
        printf("\n\t     =============FELICITACIONES %s==============\n\n\n\n",movimientoGanador->jugadorActual.nya);
    pausarConsola();
    system(CLEAR);
}

int generarInforme (tPila *historialJugadas, unsigned char jugadorAbandonoPartida)
{
    FILE *informe;
    tPila pilaInvertida;
    time_t t = time(NULL);
    tJugada jugadaBuffer;
    struct tm *tm_info = localtime(&t);
    //creamos el nombre del header
    char nombreInforme[TAM_NOMBRE_INFORME]=HEADER_INFORME;
    char bufferFecha[TAM_BUFFER_FECHA];
    strftime(bufferFecha, sizeof(bufferFecha), "%Y-%m-%d-%H-%M.txt", tm_info);
    strcat(nombreInforme,bufferFecha);
    //creamos el informe
    informe=fopen(nombreInforme,"wt");
    if(informe==NULL)
        return ERROR_ARCH_INFORME;
    //invertimos el historial de jugadas para que se lea desde el principio hasta el final
    crearPila(&pilaInvertida);
    while(desapilar(historialJugadas,&jugadaBuffer,sizeof(tJugada)))
    {
        if(!apilar(&pilaInvertida,&jugadaBuffer,sizeof(tJugada)))
        {
            vaciarPila(&pilaInvertida);
            fclose(informe);
            return SIN_MEMORIA_JUEGO;
        }
    }
    //escribimos el informe
    //[Turno 1] IA FACIL jugó MAS UNO -> IA: 1 | Jony McJony: 0 | Efecto activo: Ninguno

    while(desapilar(&pilaInvertida,&jugadaBuffer,sizeof(tJugada)))
        fprintf(informe,"[TURNO %d] %s jugo %s -> %s: %d | %s: %d | Efecto Activo: %s\n",jugadaBuffer.nroTurno,
                jugadaBuffer.jugadorActual.nya,obtenerNombreCarta(jugadaBuffer.cartaJugada),jugadaBuffer.jugadorActual.nya,jugadaBuffer.jugadorActual.puntaje,
                jugadaBuffer.jugadorRival.nya,jugadaBuffer.jugadorRival.puntaje,obtenerNombreCarta(jugadaBuffer.jugadorActual.ultimaCartaNegativaRecibida));
    if(jugadorAbandonoPartida==FALSO)
        fprintf(informe,"-----------------------------\nGanador: %s | Puntos: %d\nPerdedor: %s | Puntos: %d",jugadaBuffer.jugadorActual.nya,jugadaBuffer.jugadorActual.puntaje,
            jugadaBuffer.jugadorRival.nya,jugadaBuffer.jugadorRival.puntaje);
    else
        fprintf(informe,"-----------------------------\nEl Jugador %s abandono la partida.\n",jugadaBuffer.jugadorActual.nya);
    fclose(informe);
    return TODO_OK_JUEGO;
}

//Mezclar baraja hecho por Laion (recuperado de github).
///MENSAJE EXPLICATIVO: Básicamente se me ocurrió en vez de dejarlo en un 50%
///Las probabilidades de insercción, lo llevé a un 33% para tener mayor variacion
///Sin generar tanto overhead, podríamos haberlo hecho con un limite decreciente en base
///a que sabemos que tenemos 40 nodos(cartas) y podiamos ir insertando por posición
///Pero me resultó más óptimo y con un menor overhead esta versión :)
int mezclarBaraja(tLista *baraja)
{
    tLista barajaAux;
    tCarta bufferCarta;
    unsigned x, decision;
    crearLista(&barajaAux);
    // Una sola pasada, más eficiente que las 3 originales
    for(x = 0; x < TAM_BARAJA; x++)
    {
        // Generamos un número entre 0 y 5 para más variabilidad
        decision = numeroAleatorioEnRango(0, 5);
        // Extraemos de diferentes formas según la decisión
        if(decision <= 2)  // 50% probabilidad (0,1,2)
            sacarPrimero(baraja, &bufferCarta, sizeof(tCarta));
        else  // 50% probabilidad (3,4,5)
            sacarUltimo(baraja, &bufferCarta, sizeof(tCarta));

        // Insertamos también de forma variada
        if(decision % 3 == 0)  // 33% probabilidad (0,3)
            insertarAlInicio(&barajaAux, &bufferCarta, sizeof(tCarta));
        else  // 66% probabilidad (1,2,4,5)
            insertarAlFinal(&barajaAux, &bufferCarta, sizeof(tCarta));

        // Verificación de memoria simplificada
        if(!listaVacia(baraja) && listaVacia(&barajaAux))
        {
            vaciarLista(&barajaAux);
            return SIN_MEMORIA_JUEGO;
        }
    }

    // Devolvemos las cartas a la baraja original
    if(!reponerBarajaPrincipal(baraja, &barajaAux))
    {
        vaciarLista(&barajaAux);
        return SIN_MEMORIA_JUEGO;
    }

    return TODO_OK_JUEGO;
}


void liberarMemoriaEstructuras (tLista *barajaPrincipal, tLista *barajaUsadas, tPila *historialJugadas)
{
    vaciarLista(barajaPrincipal);
    vaciarLista(barajaUsadas);
    vaciarPila(historialJugadas);
}

char reponerBarajaPrincipal (tLista *barajaPrincipal, tLista *barajaUsadas)
{
    tCarta carta;
    while(!listaVacia(barajaUsadas))
    {
        sacarPrimero(barajaUsadas,&carta,sizeof(tCarta));
        if(!insertarAlInicio(barajaPrincipal,&carta,sizeof(tCarta)))
            return SIN_MEMORIA_JUEGO;
    }
    return TODO_OK_JUEGO;
}

char reponerCarta (tCarta *mano, tLista *barajaPrincipal)
{
    tCarta cartaRepuesto;
    while(*mano!=CARTA_USADA)
        mano++;
    if(!sacarPrimero(barajaPrincipal,&cartaRepuesto,sizeof(tCarta)))
        return BARAJA_VACIA;
    *mano=cartaRepuesto;
    return TODO_OK_JUEGO;
}

void repartirCartasInicial (tLista *barajaPrincipal,tLista *barajaUsadas, tCarta *manoCpu, tCarta *manoHumano)
{
    unsigned char x;
    tCarta carta;
    for(x=0; x<TAM_MANO*2; x++)
    {
        sacarPrimero(barajaPrincipal,&carta,sizeof(tCarta));
        insertarAlInicio(barajaUsadas,&carta,sizeof(tCarta));
        //para simular una repartida de cartas intercalada, una para cpu otra para humano y asi hasta que ambos tengan 3
        if(x%2==0)
        {
            memcpy(manoCpu,&carta,sizeof(tCarta));
            manoCpu++;
        }
        else
        {
            memcpy(manoHumano,&carta,sizeof(tCarta));
            manoHumano++;
        }
    }
}

tCarta *juegaHumano (const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano,tCarta* manoIA, tJugada jugada)
{
    tJugada ultimaJugada;
    tCarta cartaJugada= {'0'};
    int opcion;
    if(!pilaVacia(historialJugadas))
    {
        verTope(historialJugadas,&ultimaJugada,sizeof(tJugada));
        mostrarTablero(mano,manoIA,&ultimaJugada.cartaJugada,jugada);
    }
    else
        mostrarTablero(mano,manoIA,&cartaJugada,jugada);
    printf("\n\t     ================TURNO-%s==================\n\n",humano->nya);
    printf("\n(SELECCIONAR LAS CARTAS CON EL MOUSE)");

    opcion=obtenerCartaDeLaMano();

//    if(*(mano+(opcion-1))==ESPEJO && humano->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
//        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*humano->ultimaCartaNegativaRecibida,IA->nya,humano->puntosPreviosAEfectoNegativo-humano->puntaje,humano->nya);
    printf("\n");
    return mano+(opcion-1);
}
void mostrarMano(const tCarta *mano)
{
    unsigned char x;
    for(x=0; x<TAM_MANO; x++)
    {
        printf("%d. %s\n",x+1,obtenerNombreCarta(*mano));
        mano++;
    }
}
tIA setearIA(unsigned char dificultad, char *nombreIa)
{
    switch(dificultad)
    {
    case IA_FACIL:
    {
        strcpy(nombreIa,NOMBRE_IA_FACIL);
        return IAFacil;
        break;
    }
    case IA_MEDIO:
    {
        strcpy(nombreIa,NOMBRE_IA_MEDIO);
        return IAMedio;
    }
    break;
    case IA_DIFICIL:
    {
        strcpy(nombreIa,NOMBRE_IA_DIFICIL);
        return IADificil;
    }
    break;
    default:
        strcpy(nombreIa,NOMBRE_IA_FACIL);
        return IAFacil;
    }
}

tCarta* IAFacil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano,tCarta*manoJugador,tJugada jugada)
{
    //devuelve una carta cualquiera de las que tiene en mano
    tJugada ultJugada;
    tCarta ultCartaJugada= {'0'};
    tCarta *cartaTirada=cartaRandom(mano);
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
    {
        mostrarTablero(manoJugador,mano,&ultCartaJugada,jugada);
    }
    else
    {
        mostrarTablero(manoJugador,mano,&ultJugada.cartaJugada,jugada);
    }

//    if(*cartaTirada==ESPEJO&&IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
//        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*IA->ultimaCartaNegativaRecibida,humano->nya,IA->puntosPreviosAEfectoNegativo-IA->puntaje,IA->nya);
    printf("\n\t  ====================TURNO-%s======================\n",IA->nya);
    Sleep(2000);
    return cartaTirada;
}

tCarta* IAMedio(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano,tCarta*manoJugador,tJugada jugada)
{
    tJugada ultJugada;
    tCarta ultCartaJugada= {'0'};
    //elije la carta
    tCarta *cartaTirada;
    unsigned char oponenteCeroPuntos,iaCercaDeGanar,iaPoseeSumarPuntos,iaPoseeNoSacarPuntos,iaPoseeSacarPuntos;
    //obtenemos la informacion necesaria para tomar la decision
    oponenteCeroPuntos=humano->puntaje==0?VERDADERO:FALSO;
    iaCercaDeGanar=IA->puntaje>=MAX_PUNTOS-IA_PUNTOS_CERCA_GANAR?1:0;
    iaPoseeSumarPuntos=existeTipoDeCartaEnMano(mano,TIPO_POSITIVO);
    iaPoseeSacarPuntos=existeTipoDeCartaEnMano(mano,TIPO_NEGATIVO);
    iaPoseeNoSacarPuntos=existeNoTipoDeCartaEnMano(mano,TIPO_NEGATIVO);
    if(oponenteCeroPuntos==VERDADERO)
    {
        if(iaCercaDeGanar==VERDADERO)
        {
            if(iaPoseeSumarPuntos==VERDADERO) //tira la carta que mas puntos le sume
                cartaTirada=cartaQueSumaMasPuntos(mano);
            else
            {
                if(iaPoseeNoSacarPuntos==VERDADERO) //tira la primera que no sea de sacar puntos para no desperdiciarla
                    cartaTirada=cartaQueNoEsDeTipo(mano,TIPO_NEGATIVO);
                else //tira cualquiera
                    cartaTirada=cartaRandom(mano);
            }
        }
        else
        {
            if(iaPoseeNoSacarPuntos==VERDADERO) //tira la primera que no sea de sacar puntos para no desperdiciarla
                cartaTirada=cartaQueNoEsDeTipo(mano,TIPO_NEGATIVO);
            else //tira cualquiera
                cartaTirada=cartaRandom(mano);
        }
    }
    else
    {
        if(iaCercaDeGanar==VERDADERO)
        {
            if(iaPoseeSumarPuntos==VERDADERO)//tira la carta que mas puntos sume
                cartaTirada=cartaQueSumaMasPuntos(mano);
            else
            {
                if(iaPoseeSacarPuntos==VERDADERO) //tira la carta negativa que mas puntos reste sin desperdiciar puntos
                    cartaTirada=cartaNegativaMasAdecuada(mano,humano->puntaje);
                else //tira cualquiera
                    cartaTirada=cartaRandom(mano);
            }
        }
        else
        {
            if(iaPoseeSacarPuntos==VERDADERO)
                cartaTirada=cartaNegativaMasAdecuada(mano,humano->puntaje);
            else
                cartaTirada=cartaRandom(mano);
        }
    }
    if(cartaTirada==NULL)
        cartaTirada=mano;
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
        mostrarTablero(manoJugador,mano,&ultCartaJugada,jugada);
    else
        mostrarTablero(manoJugador,mano,&ultJugada.cartaJugada,jugada);
    printf("\n\t  ====================TURNO-%s======================\n",IA->nya);
    Sleep(2000);
    return cartaTirada;
}

tCarta* IADificil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano,tCarta*manoJugador,tJugada jugada)
{
    tJugada ultJugada;
    tCarta *cartaTirada = NULL;
    tCarta ultCartaJugada= {'0'};
    mostrarMano(mano);
    unsigned char oponenteCeroPuntos = (humano->puntaje == 0) ? VERDADERO : FALSO;
    unsigned char iaCercaDeGanar = (IA->puntaje >= MAX_PUNTOS - IA_PUNTOS_CERCA_GANAR) ? VERDADERO : FALSO;
    unsigned char oponenteCercaDeGanar = (humano->puntaje >= MAX_PUNTOS - IA_PUNTOS_CERCA_GANAR) ? VERDADERO : FALSO;
    unsigned char iaRecibioEfectoNegativo = (IA->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO) ? VERDADERO : FALSO;
    unsigned char tieneEspejo = existeTipoDeCartaEnMano(mano, ESPEJO);
    unsigned char tieneRepetirTurno = existeTipoDeCartaEnMano(mano, REPETIR_TURNO);
    unsigned char tieneSumarPuntos = existeTipoDeCartaEnMano(mano, TIPO_POSITIVO);
    unsigned char tieneSacarPuntos = existeTipoDeCartaEnMano(mano, TIPO_NEGATIVO);

    // PRIORIDAD 1: Usar ESPEJO si recibió efecto negativo
    if (iaRecibioEfectoNegativo && tieneEspejo)
    {
        cartaTirada = obtenerCartaEspejo(mano);
    }

    // PRIORIDAD 2: Si el oponente está cerca de ganar, ser agresivo
    else if (oponenteCercaDeGanar)
    {
        // SIEMPRE usar repetir turno si lo tiene - control total del juego
        if (tieneRepetirTurno)
            cartaTirada = obtenerCartaRepetirTurno(mano);
        // Luego sacar puntos al oponente
        else if (tieneSacarPuntos && !oponenteCeroPuntos)
            cartaTirada = cartaNegativaMasAdecuada(mano, humano->puntaje);
        // Si no puede ser agresivo, intentar ganar rápido
        else if (tieneSumarPuntos)
            cartaTirada = cartaQueSumaMasPuntos(mano);

    }

    // PRIORIDAD 3: Si la IA está cerca de ganar, priorizar victoria
    else if (iaCercaDeGanar)
    {
        // Intentar ganar inmediatamente
        if (tieneSumarPuntos)
        {
            tCarta *mejorCarta = cartaQuePuedeGanar(mano, IA->puntaje);
            if (mejorCarta != NULL)
                cartaTirada = mejorCarta;
            else
                cartaTirada = cartaQueSumaMasPuntos(mano);
        }
        // Si no puede ganar directamente, usar REPETIR_TURNO para más oportunidades
        else if (tieneRepetirTurno)
            cartaTirada = obtenerCartaRepetirTurno(mano);
        // Evitar desperdiciar cartas negativas
        else if (!oponenteCeroPuntos && tieneSacarPuntos)
            cartaTirada = cartaNegativaMasAdecuada(mano, humano->puntaje);
    }

    // PRIORIDAD 4: Juego estratégico normal
    else
    {
        // NUEVA LÓGICA: Si el oponente tiene puntos y la IA tiene REPETIR_TURNO, usarlo para mantener control
        if (tieneRepetirTurno && humano->puntaje > 0 && (tieneSumarPuntos || tieneSacarPuntos))
            cartaTirada = obtenerCartaRepetirTurno(mano);
        // Si oponente tiene 0 puntos, no desperdiciar cartas negativas
        else if (oponenteCeroPuntos)
        {
            if (tieneSumarPuntos)
                cartaTirada = cartaQueSumaMasPuntos(mano);
            else if (tieneRepetirTurno)
                cartaTirada = obtenerCartaRepetirTurno(mano);
            else
                // Evitar cartas negativas
                cartaTirada = cartaQueNoEsDeTipo(mano, TIPO_NEGATIVO);
        }
        // Juego normal - PRIORIZAR REPETIR TURNO si tiene cartas útiles
        else
        {
            // Si tiene REPETIR_TURNO y al menos una carta útil adicional, usarlo
            if (tieneRepetirTurno && (tieneSacarPuntos || tieneSumarPuntos))
                cartaTirada = obtenerCartaRepetirTurno(mano);
            // Luego priorizar sacar puntos al oponente
            else if (tieneSacarPuntos)
                cartaTirada = cartaNegativaMasAdecuada(mano, humano->puntaje);
            // Luego sumar puntos propios
            else if (tieneSumarPuntos)
                cartaTirada = cartaQueSumaMasPuntos(mano);
            // Último recurso: REPETIR_TURNO aunque no tenga cartas muy útiles
            else if (tieneRepetirTurno)
                cartaTirada = obtenerCartaRepetirTurno(mano);
        }
    }
    if(cartaTirada==NULL)
        cartaTirada=cartaRandom(mano);
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
        mostrarTablero(manoJugador,mano,&ultCartaJugada,jugada);
    else
        mostrarTablero(manoJugador,mano,&ultJugada.cartaJugada,jugada);
    printf("\n\t  ====================TURNO-%s======================\n",IA->nya);
    Sleep(2000);
    return cartaTirada;

    return cartaTirada;
}

tCarta* cartaQuePuedeGanar(tCarta *mano, char puntajeActual)
{
    unsigned x;
    int puntosNecesarios = MAX_PUNTOS - puntajeActual;

    for (x = 0; x < TAM_MANO; x++)
    {
        if (*mano == MAS_DOS && puntosNecesarios <= 2)
        {
            return mano;
        }
        else if (*mano == MAS_UNO && puntosNecesarios <= 1)
        {
            return mano;
        }
        mano++;
    }

    return NULL;
}

tCarta* obtenerCartaEspejo(tCarta *mano)
{
    unsigned x;
    for (x = 0; x < TAM_MANO; x++)
    {
        if (*mano == ESPEJO)
        {
            return mano;
        }
        mano++;
    }
    return NULL;
}

tCarta* obtenerCartaRepetirTurno(tCarta *mano)
{
    unsigned x;
    for (x = 0; x < TAM_MANO; x++)
    {
        if (*mano == REPETIR_TURNO)
        {
            return mano;
        }
        mano++;
    }
    return NULL;
}

tCarta *cartaNegativaMasAdecuada(tCarta *mano, char puntaje)
{
    //dependiendo del puntaje devuelve la direccion de la carta que mas puntos saque sin desperdicio o null si no hay cartas negativas
    unsigned x;
    tCarta *masAdecuada=NULL;
    for(x=0; x<TAM_MANO; x++)
    {
        if(cartaEsDeTipo(*mano,TIPO_NEGATIVO)==VERDADERO)
        {
            if(masAdecuada==NULL)
                masAdecuada=mano;
            else if(puntaje+*mano>=0) ///si bien es una suma, las cartas negativas valen -n, asi que seria una resta
            {
                if(puntaje+*mano<puntaje+*masAdecuada)
                    masAdecuada=mano;
            }
        }
        mano++;
    }
    return masAdecuada;
}

tCarta *cartaRandom(tCarta *mano)
{
    return mano+numeroAleatorioEnRango(0,TAM_MANO-1);
}

tCarta *cartaQueNoEsDeTipo (tCarta *mano, char tipoCarta)
{
    //devuelve la direccion de la primera carta de la mano que no sea del tipo especificado, si no encuentra ninguna devuelve NULL
    unsigned x;
    for(x=0; x<TAM_MANO; x++)
    {
        if(cartaEsDeTipo(*mano,tipoCarta)==FALSO)
            return mano;
        mano++;
    }
    return NULL;
}

tCarta *cartaQueSumaMasPuntos(tCarta *mano)
{
    //devuelve la direccion a la carta que mas suma puntos que haya dell mano, seria la "MAS POSITIVA" o null si no hay ninguna
    unsigned x;
    tCarta *mayorCarta=NULL;
    for(x=0; x<TAM_MANO; x++)
    {
        if(cartaEsDeTipo(*mano,TIPO_POSITIVO)==VERDADERO)
        {
            if(mayorCarta==NULL||*mayorCarta<*mano)
                mayorCarta=mano;
        }
        mano++;
    }
    return mayorCarta;
}

unsigned char existeNoTipoDeCartaEnMano (const tCarta *mano, char tipoCarta)
{
    //devuelve verdadero si existe al menos una carta en la mano que NO sea del tipo indicado, sino devuelve falso
    unsigned x;
    for(x=0; x<TAM_MANO; x++)
    {
        if(cartaEsDeTipo(*mano,tipoCarta)==FALSO)
            return VERDADERO;
        mano++;
    }
    return FALSO;
}

unsigned char existeTipoDeCartaEnMano (const tCarta *mano, char tipoCarta)
{
    //devuelve verdadero si existe al menos una carta en la manoq eu sea del tipo indicado, sino devuelve falso
    unsigned x;
    for(x=0; x<TAM_MANO; x++)
    {
        if(cartaEsDeTipo(*mano,tipoCarta)==VERDADERO)
            return VERDADERO;
        mano++;
    }
    return FALSO;
}

unsigned char cartaEsDeTipo(tCarta carta, char tipoCarta)
{
    //clasifica los tipos, devuelve verdadero si la carta pasada es del tipo indicado
    if(tipoCarta==TIPO_NEGATIVO)
    {
        if(carta==MENOS_UNO||carta==MENOS_DOS)
            return VERDADERO;
    }
    else if(tipoCarta==TIPO_POSITIVO)
    {
        if(carta==MAS_DOS||carta==MAS_UNO)
            return VERDADERO;
    }
    else if(tipoCarta==carta) //casos donde tipo es REPETIR_TURNO o ESPEJO, es decir, donde no queres verlo por grupo
        return VERDADERO;

    return FALSO;
}

int numeroAleatorioEnRango(int minimo, int maximo)
{
    return rand() % (maximo - minimo + 1) + minimo;
}

void mostrarCarta (const void *dato)
{
    tCarta *carta=(tCarta*)dato;
    puts(obtenerNombreCarta(*carta));
}

const char *obtenerNombreCarta (tCarta carta)
{
    switch(carta)
    {
    case MAS_UNO:
        return "+1";
        break;
    case MENOS_UNO:
        return "-1";
        break;
    case MAS_DOS:
        return "+2";
        break;
    case MENOS_DOS:
        return "-2";
        break;
    case REPETIR_TURNO:
        return "RepTurno";
        break;
    case ESPEJO:
        return "Espejo";
        break;
    case CARTA_USADA:
        return "CARTA USADA";
        break;
    case SIN_EFECTO_NEGATIVO:
        return "SIN EFECTO NEGATIVO";
        break;
    default:
        return "NO IDENTIFICADO";
        break;
    }
}

void mostrarErrorDoce(char codError)
{
    switch(codError)
    {
    case SIN_MEMORIA_JUEGO:
        puts("Se cerro el juego por falta de memoria en el sistema.");
        break;
    case ERROR_ARCH_CARTAS:
        puts("Hubo un problema al intentar crear/leer el archivo de cartas.");
        break;
    case ERROR_ARCH_INFORME:
        puts("Hubo un problema al intentar crear el archivo de informe de la partida.");
        break;
    default:
        puts("Hubo un error desconocido durante el juego.");
        break;
    };
    pausarConsola();
}
