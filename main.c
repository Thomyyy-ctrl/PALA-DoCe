#include <stdio.h>
#include <stdlib.h>
#include "firmaUtilitarias.h"
#include "firmaConexion.h"

int main(int arg,char** arg2)
{
    tConfigApi configuracion;
    srand(time(NULL));

    if(arg!=2)
        return ERROR;

    if(!obtenerConfiguracionApi(arg2[ARCHIVO_CONFIG],&configuracion))
        return ERROR;

    while(menu(&configuracion))
    {
        system(CLEAR);
    }
    system(CLEAR);

    return TODO_OK;
}
