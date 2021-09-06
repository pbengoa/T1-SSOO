#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


struct repartidor
{
    // quizas agregar tambien color de cada semaforo, entonces cuando se crea
    // el repartidor se le da el color actual de los semaforos

    // tengo que tener la distancia a cada semaforo, y en que turno pase cada semaforo
    int id;
    int num_created;
    int color_s1;
    int color_s2;
    int color_s3;
    int pos_s1;
    int pos_s2;
    int pos_s3;
    int bodega;
    int position;
    // en que momento pasamos los semaforos
    int pasamos_1;
    int pasamos_2;
    int pasamos_3;
    int llegamos;
};

typedef struct repartidor Repartidor;
Repartidor* repartidor_init(
    int id,
    int color_s1,
    int color_s2,
    int color_s3,
    int pos_s1,
    int pos_s2,
    int pos_s3,
    int bodega,
    int num_created
    );