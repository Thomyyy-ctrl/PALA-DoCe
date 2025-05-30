#include "firmaUtilitarias.h"
#include <windows.h>
#include <time.h>
#include <stdio.h>
int menu()
{
    char opcion;
    printf("\n\t[A]Jugar\n\t[B]Ver ranking\n\t[C]Salir");
    opcion = obtenerOpcionDeMenu();
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

            if(x>=2&& x<=9 && y == 0)
            {
                return 'A';
            }
            if(x>=2&& x<=22 && y == 2)
            {
                return 'B';
            }
            if(x>=2&& x<=9 && y == 4)
            {
                return 'C';
            }
        }
    }
}

void mostrar()
{
        printf("                                                                      =+=           ++++++++++++++++++++++            =+=\n");
    printf("                                                                   =+=   =+         +=     |      |    =++          =+   =+=\n");
    printf("                                                                 =+=      =+        +=     |      |    =+          =+      =+=\n");
    printf("                                                               =+= \\      =+        ++    ||     ||   =+         =+    /    =+=\n");
    printf("                                                            =+=      \\     =+       ++=    |     |    ++        =+    /       =+=\n");
    printf("                                                          +=  \\      \\     =+        =    |     |    ++       =+    /     /    =+=\n");
    printf("                                                        +=      \\     \\     =+       +=+   |     |  ++       =+    /     /     =+\n");
    printf("                                                          =+      \\     \\    =+       +=   |    |   =       =+    /     /     =+\n");
    printf("                                                            =+      \\    \\   =+        ++  |    |  =+      =+    /     /    =+\n");
    printf("                                                              =+      \\    =+=           ===========        =+= /     /   =+\n");
    printf("                                                                =+     \\ =+=                                   =+=   /   =+\n");
    printf("                                                                  =+   =+=                                         =+= =+\n");
    printf("                                                                     =+                                               \n");
    printf("\n\n\n");
    printf("                                                                        ===============                 ++++++++++++++++++=\n");
    printf("                                                                        =             +=               +==+    \\    \\     =+\n");
    printf("                                                                       =+             +=              =+  =+    \\   \\      *+\n");
    printf("                                                                       =               ==             =    =+   \\    \\      +\n");
    printf("                                                                      ==               ++             +     =    \\    \\      +\n");
    printf("                                                                      +=                ++             =*   =*   \\     \\     +=\n");
    printf("                                                                     +==                 =+             =+   +=   \\     \\     =+\n");
    printf("                                                                     ++                  +=              =    +==================\n");
    printf("                                                                    +=                    +=             +=   =+               +=\n");
    printf("                                                                    ========================              =+  =+              ==\n");
    printf("                                                                                                            =*****************\n");
    printf("\n\n\n");
    printf("                                            =+=  =+                          ===============================*                            +==+\n");
    printf("                                         =+=      =+                         =                              *                           +=    =+=  \n");
    printf("                                       =+=         =+                        =                              *                          +=         =+= \n");
    printf("                                    =+=             =+                       =                              *                         +=              =+= \n");
    printf("                                 =+=                 =+                      =                              *                        +=                  =+= \n");
    printf("                               =+=                    =+                     =                              *                       +=                      =+= \n");
    printf("                             =+=                       =+                    =                              *                      +=                         =+\n");
    printf("                          =+=                           =+                   =                              *                     +=                         =+\n");
    printf("                          =+                             =+                  =                              *                    +=                         =+\n");
    printf("                           =+                             =+                 =                              *                   +=                         =+\n");
    printf("                            =+                             =+                =                              *                  +=                         =+\n");
    printf("                             =+                             =+               =                              *                 +=                         =+ \n");
    printf("                              =+                             =+              =                              *                +=                         =+ \n");
    printf("                               =+                             =+             =                              *               +=                         =+ \n ");
    printf("                               =+                             =+            =                              *              +=                         =+\n");
    printf("                                 =+                             =+           =                              *             +=                         =+\n");
    printf("                                  =+                            +=           =                              *            +=                         =+\n");
    printf("                                   =+                        =+=             =                              *             =+=                      =+\n");
    printf("                                    =+                    =+=                =+++++++++++++++++++++++++++++++                =+=                  =+\n");
    printf("                                     =+                =+=                                                                      =+=              =+\n");
    printf("                                      =+            =+=                                                                            =+=          =+\n");
    printf("                                       =+        =+=                                                                                  =+=      =+ \n");
    printf("                                        =+    =+=                                                                                        =+=  =+ \n");
    printf("                                          +=+=                                                                                              =+=  \n");
}
