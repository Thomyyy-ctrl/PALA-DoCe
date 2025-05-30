#include"firmascartas.h"
int crearArchivo(const char*path)
{
    tCarta card[] = {///6 de +2
                    {1,0,0,0,0,0},
                    {1,0,0,0,0,0},
                    {1,0,0,0,0,0},
                    {1,0,0,0,0,0},
                    {1,0,0,0,0,0},
                    {1,0,0,0,0,0},
                    ///10 de +1
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,0},
                    ///8 de -1
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    {0,0,1,0,0,0},
                    ///6 de -2
                    {0,0,0,1,0,0},
                    {0,0,0,1,0,0},
                    {0,0,0,1,0,0},
                    {0,0,0,1,0,0},
                    {0,0,0,1,0,0},
                    {0,0,0,1,0,0},
                    ///6 repetir turno
                    {0,0,0,0,1,0},
                    {0,0,0,0,1,0},
                    {0,0,0,0,1,0},
                    {0,0,0,0,1,0},
                    {0,0,0,0,1,0},
                    {0,0,0,0,1,0},
                    ///4 espejo
                    {0,0,0,0,0,1},
                    {0,0,0,0,0,1},
                    {0,0,0,0,0,1},
                    {0,0,0,0,0,1}
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

int bajarArchLista(const char* path)//tLista *pl)
{
    int cont = 0;
    tCarta card;
    FILE*arch = fopen(path, "rb");
    if(!arch)
    {
        perror("Ha ocurrido un error\n");
        return 0;
    }
    fread(&card, sizeof(tCarta),1, arch);
    while(!feof(arch))
    {
        fread(&card, sizeof(tCarta),1, arch);
        cont++;
        printf("%d;%d;%d;%d;%d;%d\n",card.MAS_DOS, card.MAS_UNO,card.MENOS_UNO,card.MENOS_DOS,card.REPETIR_TURNO, card.ESPEJO);
    }
    printf("la cantidad de elementos es de %d\n", cont);
    fclose(arch);
    return 1;
}
