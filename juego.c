#include"firmaJuego.h"
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
    {
        perror("Ha ocurrido un error\n");
        return 0;
    }
    fwrite(card, sizeof(tCarta), sizeof(card)/ sizeof(tCarta),arch);
    fclose(arch);
    return 1;
}

int bajarArchLista(const char* path, tLista *pl)
{
    tCarta card;
    FILE*arch = fopen(path, "rb");
    if(!arch)
    {
        perror("Ha ocurrido un error\n");
        return 0;
    }
    fread(&card, sizeof(tCarta),1, arch);
    insertarAlFinal(pl,&card,sizeof(tCarta));
    while(!feof(arch))
    {
        fread(&card, sizeof(tCarta),1, arch);
        insertarAlFinal(pl,&card,sizeof(tCarta));
    }
    fclose(arch);
    return 1;
}

int jugarDoce(tPlayer *jugadorHumano, unsigned char dificultad, tJugada *movimientoGanador)
{
    //deberia recibir el jugador con su nombre, tipo de jugador (HUMANO) seteado, y puntaje en 0
    //dificultad es un unsigned char que representa el nivel de la IA, 0 facil, 1 medio, 2 dificil (definido por macro)
    tCarta manoHumano[TAM_MANO],manoCpu[TAM_MANO],*cartaJugada,*manoActual;
    unsigned char repetirTurno=0;
    tLista barajaPrincipal, barajaUsadas;
    tIA juegaIA;
    tJugada jugada;
    tPila historialJugadas;
    tPlayer jugadorCpu= {0,0,SIN_EFECTO_NEGATIVO,JUGADOR_IA,"IA"};
    tPlayer *jugadorActual=NULL,*jugadorContrario=NULL;
    //seteamos el generador de numeros random
    srand(time(NULL));
    //creamos las dos barajas principales
    crearLista(&barajaPrincipal);
    crearLista(&barajaUsadas);
    //creamos la lista del historial de jugadas
    crearPila(&historialJugadas);

    //esto de crearElArchivo en realidad seria del main creeria...
    crearArchivo(PATH_CARTAS);
    if(!bajarArchLista(PATH_CARTAS,&barajaPrincipal))
        return ERROR_ARCH_CARTAS;
    mezclarBaraja(&barajaPrincipal);

    //seteamos la IA en base a la dificultad seleccionada
    juegaIA=setearIA(dificultad);

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
    while(jugadorCpu.puntaje<MAX_PUNTOS&&jugadorHumano->puntaje<MAX_PUNTOS)
    {
        //se realiza la jugada
        if(jugadorActual->idPlayer==JUGADOR_HUMANO)
            cartaJugada=juegaHumano(&historialJugadas,jugadorHumano,&jugadorCpu,manoHumano);
        else
            cartaJugada=juegaIA(&historialJugadas,jugadorHumano,&jugadorCpu,manoCpu);
        //se aplica el efecto
        switch(*cartaJugada)
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
            repetirTurno=1;
        }
        break;
        case ESPEJO:
        {
            if(jugadorActual->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
            {
                if(jugadorActual->puntaje!=jugadorActual->puntosPreviosAEfectoNegativo)
                    jugadorActual->puntaje=jugadorActual->puntosPreviosAEfectoNegativo;
                jugadorContrario->puntaje+=jugadorActual->ultimaCartaNegativaRecibida;
                if(jugadorContrario->puntaje<0)
                    jugadorContrario->puntaje=0;
            }
        }
        break;
        default:
            puts("Carta sin efecto, funcion no implementada");
            break;
        }
        //se registra la jugada en el historial
        jugada.cartaJugada=*cartaJugada;
        jugada.jugador=*jugadorActual;
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
        //reponemos la carta en la mano del jugador que tiro recien
        if(reponerCarta(manoActual,&barajaPrincipal)==BARAJA_VACIA)
        {
            if(reponerBarajaPrincipal(&barajaPrincipal,&barajaUsadas)==SIN_MEMORIA_JUEGO)
            {
                liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
                return SIN_MEMORIA_JUEGO;
            }
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
    //guardamos el movimiento ganador para devolverlo
    verTope(&historialJugadas,movimientoGanador,sizeof(tJugada));
    mostrarGanador(movimientoGanador);
    if(generarInforme(&historialJugadas)!=TODO_OK_JUEGO)
        puts("Error al generar el informe de la partida");
    liberarMemoriaEstructuras(&barajaPrincipal,&barajaUsadas,&historialJugadas);
    return TODO_OK_JUEGO;
}

void mostrarGanador(tJugada *movimientoGanador)
{
    puts("==============PARTIDA FINALIZADA==================");
    printf("Ganador: %s con %d puntos en el turno nro %d con la carta %s.\n",movimientoGanador->jugador.nya,movimientoGanador->jugador.puntaje,movimientoGanador->nroTurno,obtenerNombreCarta(movimientoGanador->cartaJugada));
    if(movimientoGanador->jugador.idPlayer==JUGADOR_IA)
        puts("==========MAS SUERTE LA PROXIMA :/===============");
    else
        printf("===========FELICITACIONES %s==============\n\n",movimientoGanador->jugador.nya);
    system("pause");
    system("cls");
}

int generarInforme (tPila *historialJugadas)
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
    while(desapilar(&pilaInvertida,&jugadaBuffer,sizeof(tJugada)))
        fprintf(informe,"TURNO: %d | %s tiro %s | Puntos del jugador: %d | Efectos negativos activos: %s\n",jugadaBuffer.nroTurno,jugadaBuffer.jugador.nya,obtenerNombreCarta(jugadaBuffer.cartaJugada),jugadaBuffer.jugador.puntaje,obtenerNombreCarta(jugadaBuffer.jugador.ultimaCartaNegativaRecibida));
    fclose(informe);
    return TODO_OK_JUEGO;
}
char puntosUltimoEfectoNegativo (tPila *historialJugadas, tPlayer *jugadorContrario)
{
    tJugada jugada;
    tPila pilaAux;
    crearPila(&pilaAux);
    char puntosARestaurar;
    do
    {
        desapilar(historialJugadas,&jugada,sizeof(tJugada));
        if(!apilar(&pilaAux,&jugada,sizeof(tJugada)))
        {
            vaciarPila(&pilaAux);
            return SIN_MEMORIA_JUEGO;
        }
    }
    while(jugada.jugador.idPlayer!=jugadorContrario->idPlayer&&!pilaVacia(historialJugadas));
    verTope(&pilaAux,&jugada,sizeof(tJugada));
    if(jugada.jugador.idPlayer!=jugadorContrario->idPlayer) //significa que no habia puntos para restaurar
        puntosARestaurar=0;
    if(jugada.cartaJugada==MENOS_DOS)
        puntosARestaurar=-1*MENOS_DOS;
    else if (jugada.cartaJugada==MENOS_UNO)
        puntosARestaurar=-1*MENOS_UNO;
    while(desapilar(&pilaAux,&jugada,sizeof(tJugada))) //mientras la pila no este vacia
    {
        if(!apilar(historialJugadas,&jugada,sizeof(tJugada))) //si fallo el apilado
        {
            vaciarPila(&pilaAux);
            return SIN_MEMORIA_JUEGO;
        }
    }
    return puntosARestaurar;
}
int mezclarBaraja(tLista *baraja)
{
    tLista barajaAux;
    tCarta bufferCarta;
    unsigned mezclas=3,x,y; //a mayor numero mejor mezclado queda pero mas procesamiento es requerido
    crearLista(&barajaAux);
    for(x=0; x<mezclas; x++)
    {
        for(y=0; y<TAM_BARAJA; y++)
        {
            if(numeroAleatorioEnRango(0,1)==0)
                sacarPrimero(baraja,&bufferCarta,sizeof(tCarta));
            else
                sacarUltimo(baraja,&bufferCarta,sizeof(tCarta));
            if(!insertarAlInicio(&barajaAux,&bufferCarta,sizeof(tCarta)))
            {
                vaciarLista(&barajaAux);
                return SIN_MEMORIA_JUEGO;
            }
        }
        if(!reponerBarajaPrincipal(baraja,&barajaAux))
        {
            vaciarLista(&barajaAux);
            return SIN_MEMORIA_JUEGO;
        }
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

tCarta *juegaHumano (const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano)
{
    //esto seria reemplazado por la UI que se haga, pero para testing rapido va, lo importante es que me devuelva
    //un puntero a la carta que se va a jugar
    tJugada ultimaJugada;
    int opcion;
    do
    {
        printf("================TURNO-%s==================\n",humano->nya);
        printf("Puntos de la IA: %d\n",IA->puntaje);
        printf("Puntos del jugador: %d\n",humano->puntaje);
        if(!pilaVacia(historialJugadas))
        {
            verTope(historialJugadas,&ultimaJugada,sizeof(tJugada));
            printf("Ultima jugada: %s tiro %s\n",ultimaJugada.jugador.nya,obtenerNombreCarta(ultimaJugada.cartaJugada));
            if(humano->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
            {
                printf("Ultima carta negativa recibida: %s\n",obtenerNombreCarta(humano->ultimaCartaNegativaRecibida));
                printf("Puntos perdidos en esta tanda de turnos por efecto negativo: %d\n",humano->puntosPreviosAEfectoNegativo-humano->puntaje);
            }
            printf("Nro de movimiento actual: %d\n",ultimaJugada.nroTurno+1);
        }
        else
        {
            printf("Nro de movimiento actual: 1\n");
            puts("Aun no se hicieron jugadas!");
        }
        puts("--------------------------------------");
        puts("Cartas Disponibles: ");
        mostrarMano(mano);
        puts("--------------------------------------");
        printf("Ingrese su jugada: ");
        scanf("%d",&opcion);
        if(opcion<1||opcion>TAM_MANO)
        {
            printf("Elija una opcion valida (1 a 3)\n");
            fflush(stdin);
            system("pause");
            system("cls");
        }
    }
    while(opcion<1||opcion>3);
    if(*(mano+(opcion-1))==ESPEJO&&humano->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*humano->ultimaCartaNegativaRecibida,IA->nya,humano->puntosPreviosAEfectoNegativo-humano->puntaje,humano->nya);
    system("pause");
    system("cls");
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
tIA setearIA(unsigned char dificultad)
{
    switch(dificultad)
    {
    case IA_FACIL:
        return IAFacil;
        break;
    default:
        return IAFacil;
    }
}

tCarta* IAFacil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano)
{
    //devuelve una carta cualquiera de las que tiene en mano
    tJugada ultJugada;
    tCarta *cartaTirada=mano+numeroAleatorioEnRango(0,2);
    puts("====================TURNO-IA======================");
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
    {
        printf("Nro de movimiento actual: 1\n");
        puts("Aun no se hicieron jugadas!");
    }
    else
    {
        printf("Ultima Jugada: %s tiro %s\n",ultJugada.jugador.nya,obtenerNombreCarta(ultJugada.cartaJugada));
        if(IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        {
            printf("Ultima carta negativa recibida: %s\n",obtenerNombreCarta(IA->ultimaCartaNegativaRecibida));
            printf("Puntos perdidos en esta tanda de turnos por efecto negativo: %d\n",IA->puntosPreviosAEfectoNegativo-IA->puntaje);
        }
        printf("Nro de movimiento actual: %d\n",ultJugada.nroTurno+1);
    }
    printf("Puntos del jugador Humano: %d\n",humano->puntaje);
    printf("Puntos de la IA: %d\n",IA->puntaje);
    printf("IA Facil decidio tirar: %s\n",obtenerNombreCarta(*cartaTirada));
    if(*cartaTirada==ESPEJO&&IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*IA->ultimaCartaNegativaRecibida,humano->nya,IA->puntosPreviosAEfectoNegativo-IA->puntaje,IA->nya);
    puts("==========================================");
    return cartaTirada;
}

int numeroAleatorioEnRango(int minimo, int maximo)
{
    return rand() % (maximo - minimo + 1) + minimo;
}

void insertarCartaEnMano(tCarta *mano, const tCarta *cartaAInsertar, unsigned pos)
{
    //inserta la carta recibida en la posicion indicada en la mano

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
        return "MAS UNO";
        break;
    case MENOS_UNO:
        return "MENOS UNO";
        break;
    case MAS_DOS:
        return "MAS DOS";
        break;
    case MENOS_DOS:
        return "MENOS DOS";
        break;
    case REPETIR_TURNO:
        return "REPETIR TURNO";
        break;
    case ESPEJO:
        return "ESPEJO";
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
