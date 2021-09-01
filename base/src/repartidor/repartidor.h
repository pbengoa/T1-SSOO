#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


struct repartidor
{
    int semaforo1;
    int semaforo2;
    int semaforo3;
    int bodega;
};

typedef struct repartidor Repartidor;
Repartidor* repartidor_init(int semaforo1, int semaforo2, int semaforo3, int bodega);