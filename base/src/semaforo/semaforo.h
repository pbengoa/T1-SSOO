#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct semaforo
{
    int id;
    int delay;
    int parentId;
};

typedef struct semaforo Semaforo;

Semaforo* semaforo_init(int id, int delay, int parentId);



