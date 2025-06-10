# **DoCe**  
**Grupo: Pala**  

## **Antes de Jugar**  

### **1) Instalar y Configurar cURL**  
El juego requiere la biblioteca **cURL** para funcionar correctamente.  

#### **Windows**  
- Descargar cURL desde [aquí](https://curl.se/windows/).  
- Descomprimir el archivo ZIP en una carpeta que no vayan a eliminar.  
- Agregar la ruta de la carpeta `bin` dentro de la variable de entorno **PATH**.  

#### **Configuración en Code::Blocks**  
- **Settings > Compiler > Linker Settings**  
  - Agregar la ubicación de `lib\libcurl.a` y `lib\libcurl.dll.a`.  
  - En **Other linker options**, agregar:  
    ```
    -lcurl
    ```  
- **Settings > Compiler > Search directories**  
  - **Compiler y Resource compiler:** Agregar la ubicación de `include`.  
  - **Linker:** Agregar la ubicación de `lib`.  

### **2) Configurar `configuracion.txt`**  
Antes de jugar, es necesario editar el archivo `configuracion.txt` y reemplazar los valores entre corchetes (`[...]`) por los valores deseados:  
```
[URL de tu API]|[CODIGO DE GRUPO]
```  

### **3) Especificar el Archivo de Configuración en Code::Blocks**  
En **Project -> Set programs' arguments...**, agregar el nombre del archivo de configuración.  
Ejemplo:  
```
configuracion.txt
```  

---

## **¿Cómo Jugar?**  

Al iniciar el juego, aparecerá un menú con 3 opciones, que se seleccionan con el **CURSOR DEL MOUSE**:  
- **- Jugar**  
- **- Ver ranking del equipo**  
- **- Salir**  

### **- Jugar**  
1. Elegir la dificultad con el **CURSOR DEL MOUSE**:  
   - **- Facil**  
   - **- Media**
   - **- Dificil**   
2. Ingresar el nombre del jugador con el teclado.
3. Se muestra la interfaz del tablero con su mano de cartas.
4. Comienza el juego contra la **IA**.
   - el mazo cuenta con 40 cartas.Por ende, Si no quedan mas cartas que tomar se barajan las cartas lanzadas y se sigue con el juego.
   - Cada jugador tendrá un mano de cartas.
   - El jugador debe seleccionar la carta que quiera lanzar con el **CURSOR DEL MOUSE**.  
   - Una vez lanzada la carta debe presionar **ENTER** para tomar otra carta del tope del mazo o ingresar **X** con el teclado para abandonar la partida  
   - La IA jugará automáticamente después de cada turno del jugador y tomara una carta del tope del mazo tambien.  
   - El juego continúa hasta que alguno de los dos gane la partida, llegando a la cantidad de 12 puntos.  
5. Se mostrara quien gano y con que carta.  
6. Al finalizar, se generará un archivo de informe con el formato:  
   ```
   informe-juego_AAAA-MM-DD-HH-MM.txt
   ```  
   Este archivo contendrá:  
   - Turno.  
   - Jugador a quien le corresponde lanzar ese turno.  
   - Carta lanzada por el jugador.  
   - Puntajes de los jugadores despues de haber lanzado la carta.
   - El efecto activo hasta ese turno. 
7. Los resultados se enviarán a la **API**.  

### **- Ver ranking del equipo**  
Muestra el **RANKING** de quienes han jugado en orden, según la información que hay en la API.

### **- Salir**  
Finaliza la ejecución del juego.

---
