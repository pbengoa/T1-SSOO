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
    int semaforo1;
    int semaforo2;
    int semaforo3;
    int bodega;
    int position;
    int id;
};

typedef struct repartidor Repartidor;
Repartidor* repartidor_init(int semaforo1, int semaforo2, int semaforo3, int bodega);