#ifndef FIRMATDAPILA_H_INCLUDED
#define FIRMATDAPILA_H_INCLUDED
#define PILADINAMICA_H_INCLUDED
#define MIN_PILA(a,b) ((a)<(b)?(a):(b))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNodoPila
{
    void *dato;
    unsigned tam;
    struct sNodoPila *sig;
}tNodoPila;

typedef tNodoPila* tPila;


void crearPila (tPila *pila);
int desapilar (tPila *pila, void *dato, unsigned tam);
int apilar (tPila *pila, const void *dato, unsigned tam);
int verTope (const tPila *pila, void *dato, unsigned tam);
int pilaLlena (const tPila *pila);
int pilaVacia (const tPila *pila);
void vaciarPila (tPila *pila);


#endif // FIRMATDAPILA_H_INCLUDED
