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
//        case ESPEJO:
//        {
//            if(jugadorActual->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
//            {
//                if(jugadorActual->puntaje < jugadorActual->puntosPreviosAEfectoNegativo)
//                    jugadorActual->puntaje=jugadorActual->puntosPreviosAEfectoNegativo;
//                jugadorContrario->puntaje+=jugadorActual->ultimaCartaNegativaRecibida;
//                if(jugadorContrario->puntaje<0)
//                    jugadorContrario->puntaje=0;
//            }
//        }
//        break;




        case ESPEJO:
        {
            if(jugadorActual->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO)
            {
                tCarta efectoAReflejar = jugadorActual->ultimaCartaNegativaRecibida;

                printf("¡ESPEJO ACTIVADO! %s refleja %s hacia %s\n",
                       jugadorActual->nya,
                       obtenerNombreCarta(efectoAReflejar),
                       jugadorContrario->nya);

                // Restaurar puntos del jugador que usa ESPEJO
                if(jugadorActual->puntaje < jugadorActual->puntosPreviosAEfectoNegativo)
                {
                    printf("%s recupera %d puntos (de %d a %d)\n",
                           jugadorActual->nya,
                           jugadorActual->puntosPreviosAEfectoNegativo - jugadorActual->puntaje,
                           jugadorActual->puntaje,
                           jugadorActual->puntosPreviosAEfectoNegativo);
                    jugadorActual->puntaje = jugadorActual->puntosPreviosAEfectoNegativo;
                }

                // Preparar al oponente para recibir el efecto reflejado
                jugadorContrario->puntosPreviosAEfectoNegativo = jugadorContrario->puntaje;
                jugadorContrario->ultimaCartaNegativaRecibida = efectoAReflejar;

                // Aplicar el efecto reflejado al oponente
                switch(efectoAReflejar)
                {
                    case MENOS_UNO:
                        jugadorContrario->puntaje--;
                        if(jugadorContrario->puntaje < 0) jugadorContrario->puntaje = 0;
                        printf("%s pierde 1 punto por el ESPEJO reflejado (puntos: %d)\n",
                               jugadorContrario->nya, jugadorContrario->puntaje);
                        break;

                    case MENOS_DOS:
                        jugadorContrario->puntaje -= 2;
                        if(jugadorContrario->puntaje < 0) jugadorContrario->puntaje = 0;
                        printf("%s pierde 2 puntos por el ESPEJO reflejado (puntos: %d)\n",
                               jugadorContrario->nya, jugadorContrario->puntaje);
                        break;
                }

                printf("¡Ahora %s puede usar ESPEJO para devolver el efecto!\n", jugadorContrario->nya);

                // Limpiar el efecto del jugador que usó ESPEJO
                jugadorActual->ultimaCartaNegativaRecibida = SIN_EFECTO_NEGATIVO;
            }
            else
            {
                printf("%s usó ESPEJO pero no hay efecto negativo para reflejar\n", jugadorActual->nya);
                // La carta se usa igual, simplemente no hace nada
            }
        }
        break;

        // Modificación en el cambio de turno - agregar solo UNA línea
        if(!repetirTurno)
        {
            // NUEVA LÍNEA: Solo limpiar si no es ESPEJO para permitir cadenas
            if(jugadorActual->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO)
            {
                // Si el jugador no usó ESPEJO en su turno, el efecto expira
                // (esto evita que los efectos duren para siempre)
            }

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





        default:
            puts("Carta sin efecto, funcion no implementada");
            break;
        }
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
    //aca se podria llamar a la api con el movimiento ganador, o sino devuelvo el movimiento ganador y ya esa llamada se hace afuera
    return TODO_OK_JUEGO;
}

void mostrarGanador(tJugada *movimientoGanador)
{
    puts("==============PARTIDA FINALIZADA==================");
    printf("Ganador: %s con %d puntos en el turno nro %d con la carta %s.\n",movimientoGanador->jugadorActual.nya,movimientoGanador->jugadorActual.puntaje,movimientoGanador->nroTurno,obtenerNombreCarta(movimientoGanador->cartaJugada));
    if(movimientoGanador->jugadorActual.idPlayer==JUGADOR_IA)
        puts("==========MAS SUERTE LA PROXIMA :/===============");
    else
        printf("===========FELICITACIONES %s==============\n\n",movimientoGanador->jugadorActual.nya);
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
    //[Turno 1] IA FACIL jugó MAS UNO -> IA: 1 | Jony McJony: 0 | Efecto activo: Ninguno

    while(desapilar(&pilaInvertida,&jugadaBuffer,sizeof(tJugada)))
        fprintf(informe,"[TURNO %d] %s jugo %s -> %s: %d | %s: %d | Efecto Activo: %s\n",jugadaBuffer.nroTurno,
                jugadaBuffer.jugadorActual.nya,obtenerNombreCarta(jugadaBuffer.cartaJugada),jugadaBuffer.jugadorActual.nya,jugadaBuffer.jugadorActual.puntaje,
                jugadaBuffer.jugadorRival.nya,jugadaBuffer.jugadorRival.puntaje,obtenerNombreCarta(jugadaBuffer.jugadorActual.ultimaCartaNegativaRecibida));
    fprintf(informe,"-----------------------------\nGanador: %s | Puntos: %d\nPerdedor: %s | Puntos: %d",jugadaBuffer.jugadorActual.nya,jugadaBuffer.jugadorActual.puntaje,
            jugadaBuffer.jugadorRival.nya,jugadaBuffer.jugadorRival.puntaje);
    fclose(informe);
    return TODO_OK_JUEGO;
}

//int mezclarBaraja(tLista *baraja)
//{
//    tLista barajaAux;
//    tCarta bufferCarta;
//    unsigned mezclas=3,x,y; //a mayor numero mejor mezclado queda pero mas procesamiento es requerido
//    crearLista(&barajaAux);
//    for(x=0; x<mezclas; x++)
//    {
//        for(y=0; y<TAM_BARAJA; y++)
//        {
//            if(numeroAleatorioEnRango(0,1)==0)
//                sacarPrimero(baraja,&bufferCarta,sizeof(tCarta));
//            else
//                sacarUltimo(baraja,&bufferCarta,sizeof(tCarta));
//            if(!insertarAlInicio(&barajaAux,&bufferCarta,sizeof(tCarta)))
//            {
//                vaciarLista(&barajaAux);
//                return SIN_MEMORIA_JUEGO;
//            }
//        }
//        if(!reponerBarajaPrincipal(baraja,&barajaAux))
//        {
//            vaciarLista(&barajaAux);
//            return SIN_MEMORIA_JUEGO;
//        }
//    }
//    return TODO_OK_JUEGO;
//}

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

        // Usar en la función juegaHumano - agregar después de mostrar puntos:
        if(humano->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO)
        {
            printf("*** PUEDES USAR ESPEJO PARA REFLEJAR %s ***\n",
                   obtenerNombreCarta(humano->ultimaCartaNegativaRecibida));
        }


        if(!pilaVacia(historialJugadas))
        {
            verTope(historialJugadas,&ultimaJugada,sizeof(tJugada));
            printf("Ultima jugada: %s tiro %s\n",ultimaJugada.jugadorActual.nya,obtenerNombreCarta(ultimaJugada.cartaJugada));
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
//tIA setearIA(unsigned char dificultad, char *nombreIa)
//{
//    switch(dificultad)
//    {
//    case IA_FACIL:
//    {
//        strcpy(nombreIa,NOMBRE_IA_FACIL);
//        return IAFacil;
//        break;
//    }
//    case IA_MEDIO:
//    {
//        strcpy(nombreIa,NOMBRE_IA_MEDIO);
//        return IAMedio;
//    }
//    break;
//    default:
//        strcpy(nombreIa,NOMBRE_IA_FACIL);
//        return IAFacil;
//    }
//}

tIA setearIA(unsigned char dificultad, char *nombreIa)
{
    switch(dificultad)
    {
    case IA_FACIL:
        strcpy(nombreIa, NOMBRE_IA_FACIL);
        return IAFacil;
        break;
    case IA_MEDIO:
        strcpy(nombreIa, NOMBRE_IA_MEDIO);
        return IAMedio;
        break;
    case IA_DIFICIL:  // <- AGREGAR ESTE CASO
        strcpy(nombreIa, NOMBRE_IA_DIFICIL);
        return IADificil;
        break;
    default:
        strcpy(nombreIa, NOMBRE_IA_FACIL);
        return IAFacil;
    }
}

tCarta* IAFacil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano)
{
    //devuelve una carta cualquiera de las que tiene en mano
    tJugada ultJugada;
    tCarta *cartaTirada=cartaRandom(mano);
    printf("====================TURNO-%s======================\n",IA->nya);
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
    {
        printf("Nro de movimiento actual: 1\n");
        puts("Aun no se hicieron jugadas!");
    }
    else
    {
        printf("Ultima Jugada: %s tiro %s\n",ultJugada.jugadorActual.nya,obtenerNombreCarta(ultJugada.cartaJugada));
        if(IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        {
            printf("Ultima carta negativa recibida: %s\n",obtenerNombreCarta(IA->ultimaCartaNegativaRecibida));
            printf("Puntos perdidos en esta tanda de turnos por efecto negativo: %d\n",IA->puntosPreviosAEfectoNegativo-IA->puntaje);
        }
        printf("Nro de movimiento actual: %d\n",ultJugada.nroTurno+1);
    }
    printf("Puntos del jugador Humano: %d\n",humano->puntaje);
    printf("Puntos de la IA: %d\n",IA->puntaje);
    printf("%s decidio tirar: %s\n",IA->nya,obtenerNombreCarta(*cartaTirada));
    if(*cartaTirada==ESPEJO&&IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*IA->ultimaCartaNegativaRecibida,humano->nya,IA->puntosPreviosAEfectoNegativo-IA->puntaje,IA->nya);
    puts("==========================================");
    return cartaTirada;
}

//version sin mensajes en pantalla para ver razonamiento
tCarta* IAMedio(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano)
{
    tJugada ultJugada;
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
            {
                cartaTirada=cartaQueSumaMasPuntos(mano);
            }
            else
            {
                if(iaPoseeNoSacarPuntos==VERDADERO) //tira la primera que no sea de sacar puntos para no desperdiciarla
                {
                    cartaTirada=cartaQueNoEsDeTipo(mano,TIPO_NEGATIVO);
                }
                else //tira cualquiera
                {
                    cartaTirada=cartaRandom(mano);
                }
            }
        }
        else
        {
            if(iaPoseeNoSacarPuntos==VERDADERO) //tira la primera que no sea de sacar puntos para no desperdiciarla
            {
                cartaTirada=cartaQueNoEsDeTipo(mano,TIPO_NEGATIVO);
            }
            else //tira cualquiera
            {
                cartaTirada=cartaRandom(mano);
            }
        }
    }
    else
    {
        if(iaCercaDeGanar==VERDADERO)
        {
            if(iaPoseeSumarPuntos==VERDADERO)//tira la carta que mas puntos sume
            {
                cartaTirada=cartaQueSumaMasPuntos(mano);
            }
            else
            {
                if(iaPoseeSacarPuntos==VERDADERO) //tira la carta negativa que mas puntos reste sin desperdiciar puntos
                {
                    cartaTirada=cartaNegativaMasAdecuada(mano,humano->puntaje);
                }
                else //tira cualquiera
                {
                    cartaTirada=cartaRandom(mano);
                }
            }
        }
        else
        {
            if(iaPoseeSacarPuntos==VERDADERO)
            {
                cartaTirada=cartaNegativaMasAdecuada(mano,humano->puntaje);
            }
            else
            {
                cartaTirada=cartaRandom(mano);
            }
        }
    }
    if(cartaTirada==NULL)
    {
        cartaTirada=mano;
    }
    printf("====================TURNO-%s======================\n",IA->nya);
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
    {
        printf("Nro de movimiento actual: 1\n");
        puts("Aun no se hicieron jugadas!");
    }
    else
    {
        printf("Ultima Jugada: %s tiro %s\n",ultJugada.jugadorActual.nya,obtenerNombreCarta(ultJugada.cartaJugada));
        if(IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        {
            printf("Ultima carta negativa recibida: %s\n",obtenerNombreCarta(IA->ultimaCartaNegativaRecibida));
            printf("Puntos perdidos en esta tanda de turnos por efecto negativo: %d\n",IA->puntosPreviosAEfectoNegativo-IA->puntaje);
        }
        printf("Nro de movimiento actual: %d\n",ultJugada.nroTurno+1);
    }
    printf("Puntos del jugador Humano: %d\n",humano->puntaje);
    printf("Puntos de la IA: %d\n",IA->puntaje);
    printf("%s decidio tirar: %s\n",IA->nya,obtenerNombreCarta(*cartaTirada));
    if(*cartaTirada==ESPEJO&&IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*IA->ultimaCartaNegativaRecibida,humano->nya,IA->puntosPreviosAEfectoNegativo-IA->puntaje,IA->nya);
    puts("==========================================");
    return cartaTirada;
}

/* VERSION CON RAZONAMIENTO (mensajes en pantalla) - PARA DEBUGGING
tCarta* IAMedio(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano)
{
    tJugada ultJugada;
    //elije la carta
    tCarta *cartaTirada;
    unsigned char oponenteCeroPuntos,iaCercaDeGanar,iaPoseeSumarPuntos,iaPoseeNoSacarPuntos,iaPoseeSacarPuntos;
    //obtenemos la informacion necesaria para tomar la decision
    oponenteCeroPuntos=humano->puntaje==0?VERDADERO:FALSO;
    iaCercaDeGanar=IA->puntaje>=MAX_PUNTOS-IA_PUNTOS_CERCA_GANAR?1:0;
    iaPoseeSumarPuntos=existeTipoDeCartaEnMano(mano,TIPO_POSITIVO);
    iaPoseeSacarPuntos=existeTipoDeCartaEnMano(mano,TIPO_NEGATIVO);
    iaPoseeNoSacarPuntos=existeNoTipoDeCartaEnMano(mano,TIPO_NEGATIVO);
    puts("================================================");
    puts("Mano de la IA: ");
    mostrarMano(mano);
    puts("================================================");
    puts("=========RAZONAMIENTO DE LA IA MEDIO============");
    if(oponenteCeroPuntos==VERDADERO)
    {
        puts("Ia ha detectado que el oponente tiene cero puntos");
        if(iaCercaDeGanar==VERDADERO)
        {
            puts("Ia ha detectado que esta cerca de ganar");
            if(iaPoseeSumarPuntos==VERDADERO) //tira la carta que mas puntos le sume
            {
                puts("Ia ha detectado que tiene cartas de sumar puntos");
                puts("Ia intento tirar su carta mas positiva");
                cartaTirada=cartaQueSumaMasPuntos(mano);
            }
            else
            {
                puts("Ia ha detectado que no posee cartas de sumar puntos");
                if(iaPoseeNoSacarPuntos==VERDADERO) //tira la primera que no sea de sacar puntos para no desperdiciarla
                {
                    puts("Ia ha detectado que posee cartas que no sean de sacar puntos");
                    puts("Ia intento tirar una carta que no es negativa");
                    cartaTirada=cartaQueNoEsDeTipo(mano,TIPO_NEGATIVO);
                }
                else //tira cualquiera
                {
                    puts("Ia ha detectado que no posee cartas que no sean de sacar puntos");
                    puts("Ia tiro cualquier carta de la mano");
                    cartaTirada=cartaRandom(mano);
                }
            }
        }
        else
        {
            puts("Ia ha detectado que no esta cerca de ganar");
            if(iaPoseeNoSacarPuntos==VERDADERO) //tira la primera que no sea de sacar puntos para no desperdiciarla
            {
                puts("Ia ha detectado que posee cartas que no son negativas");
                puts("Ia tiro una carta no negativa para no desperdiciar");
                cartaTirada=cartaQueNoEsDeTipo(mano,TIPO_NEGATIVO);
            }
            else //tira cualquiera
            {
                puts("Ia ha detectado que no posee cartas no negativas");
                puts("Ia tiro cualquier carta de la mano");
                cartaTirada=cartaRandom(mano);
            }
        }
    }
    else
    {
        puts("Ia ha detectado que el oponente posee puntos");
        if(iaCercaDeGanar==VERDADERO)
        {
            puts("Ia ha detectado que esta cerca de ganar");
            if(iaPoseeSumarPuntos==VERDADERO)//tira la carta que mas puntos sume
            {
                puts("Ia ha detectado que posee cartas de sumar puntos");
                puts("Ia intento tirar su carta mas positiva");
                cartaTirada=cartaQueSumaMasPuntos(mano);
            }
            else
            {
                puts("Ia ha detectado que no posee cartas de sumar puntos");
                if(iaPoseeSacarPuntos==VERDADERO) //tira la carta negativa que mas puntos reste sin desperdiciar puntos
                {
                    puts("Ia ha detectado que posee cartas de sacar puntos");
                    puts("Ia intento tirar la carta negativa mas adecuada de su mano");
                    cartaTirada=cartaNegativaMasAdecuada(mano,humano->puntaje);
                }
                else //tira cualquiera
                {
                    puts("Ia ha detectado que no posee cartas de sacar puntos");
                    puts("Ia ha decidido tirar cualquier carta");
                    cartaTirada=cartaRandom(mano);
                }
            }
        }
        else
        {
            puts("Ia ha detectado que no esta cerca de ganar");
            if(iaPoseeSacarPuntos==VERDADERO)
            {
                puts("Ia ha detectado que posee cartas de sacar puntos");
                puts("Ia ha tirado su carta negativa mas adecuada");
                cartaTirada=cartaNegativaMasAdecuada(mano,humano->puntaje);
            }
            else
            {
                puts("Ia ha detectado que no posee cartas de sacar puntos");
                puts("Ia ha decidido tirar cualquier carta");
                cartaTirada=cartaRandom(mano);
            }
        }
    }
    if(cartaTirada==NULL)
    {
        puts("Esto no debio haber pasado, hubo algun error.");
        cartaTirada=mano;
    }
    printf("Ia decidio tirar: %s\n",obtenerNombreCarta(*cartaTirada));
    puts("================================================");
    printf("====================TURNO-%s======================\n",IA->nya);
    if(!verTope(historialJugadas,&ultJugada,sizeof(tJugada)))
    {
        printf("Nro de movimiento actual: 1\n");
        puts("Aun no se hicieron jugadas!");
    }
    else
    {
        printf("Ultima Jugada: %s tiro %s\n",ultJugada.jugadorActual.nya,obtenerNombreCarta(ultJugada.cartaJugada));
        if(IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        {
            printf("Ultima carta negativa recibida: %s\n",obtenerNombreCarta(IA->ultimaCartaNegativaRecibida));
            printf("Puntos perdidos en esta tanda de turnos por efecto negativo: %d\n",IA->puntosPreviosAEfectoNegativo-IA->puntaje);
        }
        printf("Nro de movimiento actual: %d\n",ultJugada.nroTurno+1);
    }
    printf("Puntos del jugador Humano: %d\n",humano->puntaje);
    printf("Puntos de la IA: %d\n",IA->puntaje);
    printf("%s decidio tirar: %s\n",IA->nya,obtenerNombreCarta(*cartaTirada));
    if(*cartaTirada==ESPEJO&&IA->ultimaCartaNegativaRecibida!=SIN_EFECTO_NEGATIVO)
        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",-1*IA->ultimaCartaNegativaRecibida,humano->nya,IA->puntosPreviosAEfectoNegativo-IA->puntaje,IA->nya);
    puts("==========================================");
    return cartaTirada;
}*/

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

////unsigned char cartaEsDeTipo(tCarta carta, char tipoCarta)
////{
////    //clasifica los tipos, devuelve verdadero si la carta pasada es del tipo indicado
////    if(tipoCarta==TIPO_NEGATIVO)
////    {
////        if(carta==MENOS_UNO||carta==MENOS_DOS)
////            return VERDADERO;
////    }
////    else if(tipoCarta==TIPO_POSITIVO)
////    {
////        if(carta==MAS_DOS||carta==MAS_UNO)
////            return VERDADERO;
////    }
////    return FALSO;
////}
unsigned char cartaEsDeTipo(tCarta carta, char tipoCarta)
{
    if(tipoCarta == TIPO_NEGATIVO) {
        if(carta == MENOS_UNO || carta == MENOS_DOS)
            return VERDADERO;
    }
    else if(tipoCarta == TIPO_POSITIVO) {
        if(carta == MAS_DOS || carta == MAS_UNO)
            return VERDADERO;
    }
    else if(tipoCarta == TIPO_ESPEJO) {  // <- AGREGAR
        if(carta == ESPEJO)
            return VERDADERO;
    }
    else if(tipoCarta == TIPO_REPETIR_TURNO) {  // <- AGREGAR
        if(carta == REPETIR_TURNO)
            return VERDADERO;
    }
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

//FUNCIÓN NIVEL DIFICIL
// Función para verificar si existe carta ESPEJO en mano
unsigned char existeTipoDeCartaEnMano_Espejo(const tCarta *mano) {
    unsigned x;
    for (x = 0; x < TAM_MANO; x++) {
        if (*mano == ESPEJO) {
            return VERDADERO;
        }
        mano++;
    }
    return FALSO;
}

// Función para verificar si existe carta REPETIR_TURNO en mano
unsigned char existeTipoDeCartaEnMano_RepetirTurno(const tCarta *mano) {
    unsigned x;
    for (x = 0; x < TAM_MANO; x++) {
        if (*mano == REPETIR_TURNO) {
            return VERDADERO;
        }
        mano++;
    }
    return FALSO;
}

// Obtener carta ESPEJO de la mano
tCarta* obtenerCartaEspejo(tCarta *mano) {
    unsigned x;
    for (x = 0; x < TAM_MANO; x++) {
        if (*mano == ESPEJO) {
            return mano;
        }
        mano++;
    }
    return NULL;
}

// Obtener carta REPETIR_TURNO de la mano
tCarta* obtenerCartaRepetirTurno(tCarta *mano) {
    unsigned x;
    for (x = 0; x < TAM_MANO; x++) {
        if (*mano == REPETIR_TURNO) {
            return mano;
        }
        mano++;
    }
    return NULL;
}

// Contar cuántas cartas "buenas" tiene en mano para la situación actual
unsigned char contarCartasBuenas(const tCarta *mano, char puntajeIA, char puntajeOponente) {
    unsigned x;
    unsigned char count = 0;

    for (x = 0; x < TAM_MANO; x++) {
        // Cartas siempre buenas
        if (*mano == MAS_UNO || *mano == MAS_DOS || *mano == REPETIR_TURNO) {
            count++;
        }
        // ESPEJO es buena si hay efecto negativo pendiente
        else if (*mano == ESPEJO) {
            // Asumimos que si llama a esta función, ya verificó si hay efecto negativo
            count++;
        }
        // Cartas negativas son buenas si el oponente tiene puntos
        else if ((*mano == MENOS_UNO || *mano == MENOS_DOS) && puntajeOponente > 0) {
            count++;
        }
        mano++;
    }

    return count;
}

// Encontrar carta que puede hacer ganar inmediatamente
tCarta* cartaQuePuedeGanar(tCarta *mano, char puntajeActual) {
    unsigned x;
    int puntosNecesarios = MAX_PUNTOS - puntajeActual;

    for (x = 0; x < TAM_MANO; x++) {
        if (*mano == MAS_DOS && puntosNecesarios <= 2) {
            return mano;
        }
        else if (*mano == MAS_UNO && puntosNecesarios <= 1) {
            return mano;
        }
        mano++;
    }

    return NULL;
}

// Seleccionar la carta óptima para sumar puntos según la situación
tCarta* cartaOptimaSumarPuntos(tCarta *mano, char puntajeActual) {
    unsigned x;
    tCarta *mejorCarta = NULL;
    int puntosNecesarios = MAX_PUNTOS - puntajeActual;

    // Si está muy cerca de ganar, priorizar no desperdiciar
    if (puntosNecesarios <= 2) {
        // Buscar primero MAS_UNO si solo necesita 1 punto
        if (puntosNecesarios == 1) {
            for (x = 0; x < TAM_MANO; x++) {
                if (*mano == MAS_UNO) {
                    return mano;
                }
                mano++;
            }
            // Si no tiene MAS_UNO, usar MAS_DOS
            mano -= TAM_MANO; // Resetear puntero
        }

        // Buscar MAS_DOS
        for (x = 0; x < TAM_MANO; x++) {
            if (*mano == MAS_DOS) {
                return mano;
            }
            mano++;
        }
    }

    // Situación normal: priorizar la carta que más puntos sume
    return cartaQueSumaMasPuntos(mano);
}

// Actualizar la función setearIA para incluir IA Difícil
tIA setearIA_Actualizado(unsigned char dificultad, char *nombreIa) {
    switch(dificultad) {
        case IA_FACIL: {
            strcpy(nombreIa, NOMBRE_IA_FACIL);
            return IAFacil;
        }
        case IA_MEDIO: {
            strcpy(nombreIa, NOMBRE_IA_MEDIO);
            return IAMedio;
        }
        case IA_DIFICIL: {
            strcpy(nombreIa, NOMBRE_IA_DIFICIL);  // Necesitas definir esta constante
            return IADificil;
        }
        default:
            strcpy(nombreIa, NOMBRE_IA_FACIL);
            return IAFacil;
    }
}

///MEJORA
tCarta* IADificil(const tPila *historialJugadas, const tPlayer *humano, const tPlayer*IA, tCarta *mano)
{
    tJugada ultJugada;
    tCarta *cartaTirada = NULL;
    puts("MANO IA:\n");
    mostrarMano(mano);
    // Variables de análisis del estado del juego
    unsigned char oponenteCeroPuntos = (humano->puntaje == 0) ? VERDADERO : FALSO;
    unsigned char iaCercaDeGanar = (IA->puntaje >= MAX_PUNTOS - IA_PUNTOS_CERCA_GANAR) ? VERDADERO : FALSO;
    unsigned char oponenteCercaDeGanar = (humano->puntaje >= MAX_PUNTOS - IA_PUNTOS_CERCA_GANAR) ? VERDADERO : FALSO;
    unsigned char iaRecibioEfectoNegativo = (IA->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO) ? VERDADERO : FALSO;

    // Análisis de cartas disponibles en mano
    unsigned char tieneEspejo = existeTipoDeCartaEnMano(mano, TIPO_ESPEJO);
    unsigned char tieneRepetirTurno = existeTipoDeCartaEnMano(mano, TIPO_REPETIR_TURNO);
    unsigned char tieneSumarPuntos = existeTipoDeCartaEnMano(mano, TIPO_POSITIVO);
    unsigned char tieneSacarPuntos = existeTipoDeCartaEnMano(mano, TIPO_NEGATIVO);

    // PRIORIDAD 1: Usar ESPEJO si recibió efecto negativo
    if (iaRecibioEfectoNegativo && tieneEspejo) {
        cartaTirada = obtenerCartaEspejo(mano);
    }

    // PRIORIDAD 2: Si el oponente está cerca de ganar, ser agresivo
    else if (oponenteCercaDeGanar) {
        // SIEMPRE usar repetir turno si lo tiene - control total del juego
        if (tieneRepetirTurno) {
            cartaTirada = obtenerCartaRepetirTurno(mano);
        }
        // Luego sacar puntos al oponente
        else if (tieneSacarPuntos && !oponenteCeroPuntos) {
            cartaTirada = cartaNegativaMasAdecuada(mano, humano->puntaje);
        }
        // Si no puede ser agresivo, intentar ganar rápido
        else if (tieneSumarPuntos) {
            cartaTirada = cartaQueSumaMasPuntos(mano);
        }
    }

    // PRIORIDAD 3: Si la IA está cerca de ganar, priorizar victoria
    else if (iaCercaDeGanar) {
        // Intentar ganar inmediatamente
        if (tieneSumarPuntos) {
            tCarta *mejorCarta = cartaQuePuedeGanar(mano, IA->puntaje);
            if (mejorCarta != NULL) {
                cartaTirada = mejorCarta;
            } else {
                cartaTirada = cartaQueSumaMasPuntos(mano);
            }
        }
        // Si no puede ganar directamente, usar REPETIR_TURNO para más oportunidades
        else if (tieneRepetirTurno) {
            cartaTirada = obtenerCartaRepetirTurno(mano);
        }
        // Evitar desperdiciar cartas negativas
        else if (!oponenteCeroPuntos && tieneSacarPuntos) {
            cartaTirada = cartaNegativaMasAdecuada(mano, humano->puntaje);
        }
    }

    // PRIORIDAD 4: Juego estratégico normal
    else {
        // NUEVA LÓGICA: Si el oponente tiene puntos y la IA tiene REPETIR_TURNO, usarlo para mantener control
        if (tieneRepetirTurno && humano->puntaje > 0 && (tieneSumarPuntos || tieneSacarPuntos)) {
            cartaTirada = obtenerCartaRepetirTurno(mano);
        }
        // Si oponente tiene 0 puntos, no desperdiciar cartas negativas
        else if (oponenteCeroPuntos) {
            if (tieneSumarPuntos) {
                cartaTirada = cartaOptimaSumarPuntos(mano, IA->puntaje);
            }
            else if (tieneRepetirTurno) {
                cartaTirada = obtenerCartaRepetirTurno(mano);
            }
            else {
                // Evitar cartas negativas
                cartaTirada = cartaQueNoEsDeTipo(mano, TIPO_NEGATIVO);
            }
        }
        // Juego normal - PRIORIZAR REPETIR TURNO si tiene cartas útiles
        else {
            // Si tiene REPETIR_TURNO y al menos una carta útil adicional, usarlo
            if (tieneRepetirTurno && (tieneSacarPuntos || tieneSumarPuntos)) {
                cartaTirada = obtenerCartaRepetirTurno(mano);
            }
            // Luego priorizar sacar puntos al oponente
            else if (tieneSacarPuntos) {
                cartaTirada = cartaNegativaMasAdecuada(mano, humano->puntaje);
            }
            // Luego sumar puntos propios
            else if (tieneSumarPuntos) {
                cartaTirada = cartaOptimaSumarPuntos(mano, IA->puntaje);
            }
            // Último recurso: REPETIR_TURNO aunque no tenga cartas muy útiles
            else if (tieneRepetirTurno) {
                cartaTirada = obtenerCartaRepetirTurno(mano);
            }
        }
    }

    // Fallback: si no se seleccionó ninguna carta por algún error lógico
    if (cartaTirada == NULL) {
        cartaTirada = mano; // Primera carta disponible
    }

    // Mostrar información del turno (similar a las otras IAs)
    printf("====================TURNO-%s======================\n", IA->nya);
    if (!verTope(historialJugadas, &ultJugada, sizeof(tJugada)))
    {
        printf("Nro de movimiento actual: 1\n");
        puts("Aun no se hicieron jugadas!");
    }
    else {
        printf("Ultima Jugada: %s tiro %s\n", ultJugada.jugadorActual.nya, obtenerNombreCarta(ultJugada.cartaJugada));
        if (IA->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO) {
            printf("Ultima carta negativa recibida: %s\n", obtenerNombreCarta(IA->ultimaCartaNegativaRecibida));
            printf("Puntos perdidos en esta tanda de turnos por efecto negativo: %d\n", IA->puntosPreviosAEfectoNegativo - IA->puntaje);
        }
        printf("Nro de movimiento actual: %d\n", ultJugada.nroTurno + 1);
    }
    printf("Puntos del jugador Humano: %d\n", humano->puntaje);
    printf("Puntos de la IA: %d\n", IA->puntaje);
    printf("%s decidio tirar: %s\n", IA->nya, obtenerNombreCarta(*cartaTirada));
    if (*cartaTirada == ESPEJO && IA->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO) {
        printf("ESPEJO: Se le restaran %d puntos a %s!\nSe le sumaran %d puntos a %s\n",
               -1 * IA->ultimaCartaNegativaRecibida, humano->nya,
               IA->puntosPreviosAEfectoNegativo - IA->puntaje, IA->nya);
    }
    puts("==========================================");

    return cartaTirada;
}
// OPCIONAL: Función para mostrar si el jugador puede usar ESPEJO
void mostrarOpcionEspejo(const tPlayer *jugador)
{
    if(jugador->ultimaCartaNegativaRecibida != SIN_EFECTO_NEGATIVO)
    {
        printf("*** %s puede usar ESPEJO para reflejar %s ***\n",
               jugador->nya,
               obtenerNombreCarta(jugador->ultimaCartaNegativaRecibida));
    }
}
