
# Estrategia



## Estructuras utilizadas
Se utilizaron las siguientes estructuras de datos:

- **Pila** 
Se uso pila para el historial de jugadas ya que permitia acceder facilmente a la ultima jugada hecha, lo cual venia perfecto para mostrar la ultima carta tirada y demas en la interfaz. Ademas no era necesario recorrer el historial, ni hacer nada que no fuese apilar (registrar una jugada), ver tope (para mostrar la ultima tirada), y luego a la hora de generar el informe solo debiamos invertir la pila usando una pila auxiliar. Pero y si debemos invertir la pila...

**¿Por qué no usamos lista para el historial?** Si bien es cierto que para hacer el informe de la partida se debia invertir la pila lo cual es costoso hasta cierto punto, el costo es menor que si fueses a insertar al final siempre con la lista para que el orden se mantenga y al final no tengas que invertirlo ya que en una situacion donde la partida se alargo mucho (100 turnos por ejemplo) si fuese una lista y fueses a insertar al final para registrar una jugada tendrias que recorrer los 100 nodos para hacerlo, y asi todas las veces que alguien jugara una carta desde ese punto en adelante, en cambio con pila, solo tenes que insertar al inicio (apilar) lo cual es mucho menos costoso. Por tanto, el costo de invertir la pila al final es mucho menor que el de mantener el orden desde un principio usando lista.

- **Lista simplemente enlazada**
Se utilizaron listas simples para la baraja principal y la baraja de usadas. En la gran mayoria de la ejecucion igualmente se trata a estas listas como si fueran pilas realizando operaciones de insertar al principio y sacar del principio, considerando eso..

**¿Por qué no usamos pila tambien para las barajas?** La razon es que para mezclar las cartas a la hora de resetear la baraja era necesario recorrer. Si bien es cierto que con pila se puede (y de hecho asi fue como se intentó hacer en un principio ya que ese comportamiento imita la vida real perfectamente) el resultado es poco optimo teniendo que hacer uso de multiples pilas y una logica rebuscada que se simplifica mucho utilizando listas, especificamente tener la posibilidad de insertar al final y sacar del final (cosa que en pila no es posible) son dos opciones que facilitan la logica de mezcla usando listas, por tanto se optó por el uso de estas.

## Structs y typedefs para el juego
- **typedef char tCarta**
El tipo tCarta representa una de las cartas del juego, internamente es solo un char, y guarda el codigo de la carta las cuales fueron definidas con macros:

| Código | Macro           | Descripción                         |
| ------ | --------------- | ----------------------------------- |
| `1`    | `MAS_UNO`       | El jugador obtiene 1 punto            |
| `2`    | `MAS_DOS`       | El jugador obtiene 2 puntos           |
| `-1`   | `MENOS_UNO`     | El oponente pierde 1 punto     |
| `-2`   | `MENOS_DOS`     | El oponente pierde 2 puntos     |
| `3`    | `ESPEJO`        | Nulifica y Refleja el efecto negativo recibido |
| `4`    | `REPETIR_TURNO` | Permite jugar un turno adicional    |
| `5`    | `CARTA_USADA`   | Marca una carta como ya jugada      |

Las manos de los jugadores están representadas mediante un vector de tCarta con 3 posiciones. Se eligió utilizar un vector en lugar de una lista por la facilidad que ofrece al recorrerlo, especialmente teniendo en cuenta que varias operaciones —como las decisiones de la IA— requieren analizar todas las cartas disponibles.

Además, el tamaño de la mano es fijo, por lo que no es necesario insertar ni eliminar elementos dinámicamente. En lugar de quitar una carta del vector al usarla, simplemente se la marca como utilizada asignándole el código CARTA_USADA. Esto evita operaciones costosas como desplazamientos de elementos, que serían necesarias en caso de modificar un vector de tamaño variable. Dado que el tamaño es pequeño y constante, esta solución es eficiente y simple.

- **tPlayer**

| Campo                          | Tipo       | Descripción                                                       |
| ------------------------------ | ---------- | ----------------------------------------------------------------- |
| `puntaje`                      | `char`     | Puntaje actual del jugador.                                       |
| `puntosPreviosAEfectoNegativo` | `char`     | Puntaje del jugador antes de recibir una carta negativa.          |
| `ultimaCartaNegativaRecibida`  | `char`     | Código de la última carta negativa recibida.                      |
| `idPlayer`                     | `unsigned` | Identificador del jugador (`JUGADOR_HUMANO` o `JUGADOR_IA`).      |
| `nya`                          | `char[]`   | Nombre y apellido del jugador (tamaño definido por `TAM_CAD_PL`). |

Muchos de los campos son autoexplicativos. En cuanto a puntosPreviosAEfectoNegativo y ultimaCartaNegativaRecibida estos se utilizan para implementar la funcionalidad de espejo de una manera sencilla, ademas, en el informe tambien es util poder reflejar si en un turno dado el jugador tenia algun efecto negativo aplicado. Por ultimo, idPlayer es un identificador rapido para saber si el jugadorActual es la IA o el humano, se define con macros sus valores, siendo estos:
JUGADOR_IA 0 y JUGADOR_HUMANO 1.

- **tJugada**

| Campo           | Tipo       | Descripción                                   |
| --------------- | ---------- | --------------------------------------------- |
| `cartaJugada`   | `tCarta`   | Carta que fue jugada durante el turno.        |
| `jugadorActual` | `tPlayer`  | Jugador que realizó la jugada.                |
| `jugadorRival`  | `tPlayer`  | Rival del jugador que realizó la jugada.      |
| `nroTurno`      | `unsigned` | Número del turno en que se realizó la jugada. |

Se utiliza para registrar cada jugada en el historial de jugadas, guarda toda la informacion que pueda ser relevante para cada linea del informe y tambien se utilizado en la interfaz. Por ultimo, cuando la partida termina, la ultima jugada registrada en el historial es el movimiento ganador, dentro esta la informacion necesaria para el registro en la API.


- **`Puntero a funcion: tIA`**

```c
typedef tCarta* (*tIA)(const tPila*, const tPlayer *, const tPlayer*, tCarta *mano, tCarta* manoJugador, tJugada jugada);
```

Se utilizó un puntero a función para representar la lógica de la IA de forma genérica. Esto permite asignar una única vez, al comienzo de la partida, la función correspondiente según la dificultad elegida (fácil, media o difícil). A lo largo del juego, el flujo simplemente invoca este puntero (`juegaIA`) sin necesidad de verificar constantemente qué IA debe ejecutarse, eliminando estructuras condicionales repetitivas y logrando un diseño más limpio, extensible y desacoplado.

### Flujo general de `jugarDoce`

La función `jugarDoce` coordina el desarrollo completo de una partida entre el jugador humano y la IA, manejando desde la inicialización hasta la determinación del ganador.

#### Pasos principales:

1. **Inicialización**: Se crean y mezclan las barajas, se reparten las manos iniciales, se define la dificultad de la IA y se selecciona aleatoriamente quién inicia.
2. **Ejecución de turnos**:
   - En cada turno, se invoca la acción correspondiente según el tipo de jugador (`juegaHumano` o la `juegaIA`).
   - Se aplica el efecto de la carta jugada.
   - Se registra la jugada en el historial y se mueve la carta a la baraja de descartes.
   - Se marca la carta en la mano del jugador como usada
   - Se repone la carta en la mano del jugador, mezclando el mazo si es necesario.
   - Se alterna el turno (excepto si una carta obliga a repetirlo).
3. **Finalización**:
   - Al llegar al puntaje máximo, se registra la jugada final, se muestra al ganador y se genera un informe de la partida.
   - Se libera memoria y se envía el resultado a través de la API.

Cabe destacar que la cantidad de puntos necesarias para ganar la partida tambien es definida por una macro (`MAX_PUNTOS`), durante el testing se redujo este limite a 6 puntos o incluso menos ya que las partidas llegaban a ser bastante largas. (La partida mas larga fue de 546 movimientos contra la IA Dificil).

Si en algun momento de la partida hay algun error por falta de memoria o demas motivos se liberará toda la memoria que fue usada hasta el momento y se informara devolviendo un codigo de error que luego será mostrado en pantalla junto con la explicacion del problema que hubo.

## Diagramas de decision de las IA's

Solo se mostraran las de las IA media y Dificil ya que la Facil elije cualquier carta de la mano y la tira.
- IA Media
![Diagrama de decisión IA dificultad Media](https://i.imgur.com/k0iYsox.png)

- IA Dificil
![Diagrama de decisión IA dificultad Dificil](https://i.imgur.com/pNHogYr.png)







