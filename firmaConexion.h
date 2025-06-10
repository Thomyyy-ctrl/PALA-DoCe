#ifndef FIRMACONEXION_H_INCLUDED
#define FIRMACONEXION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#define TAM_MAX_JSON 1000
#define TAM_CAD 100
#define CONF_API_FORMATO_CON_ESPACIOS "%s | %s"

typedef struct {
    char nombre[TAM_CAD];
    int gano;
} tJugadorAPI;

typedef struct {
    char *info;
    size_t tamInfo;
} tRespuesta;

typedef struct
{
    char urlApi[TAM_CAD];
    char codGrupo[TAM_CAD];
}tConfigApi;

void enviarDatosJSON(const void* elem1,const void* elem2);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
CURLcode peticionGET(tRespuesta *respuesta,const char *path);
CURLcode peticionPOST(tRespuesta *respuesta,const char* pathUrl,const char* jsonData);
int obtenerConfiguracionApi(char* path, tConfigApi* conf);

#endif // FIRMACONEXION_H_INCLUDED
