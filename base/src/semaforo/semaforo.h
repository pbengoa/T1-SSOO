#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct semaforo
{
    int id;
    int distance;
    int delay;
    int parentId;
    int color;//0 verde, 1 rojo
    int cambios;
};

typedef struct semaforo Semaforo;

Semaforo* semaforo_init(int id, int distance,int delay, int parentId);



