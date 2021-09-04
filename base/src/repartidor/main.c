#include <stdio.h>
#include <unistd.h>
#include "repartidor.h"
#include <stdlib.h>

Repartidor* repartidor_init(int semaforo1, int semaforo2, int semaforo3, int bodega)
{
  Repartidor* repartidor = malloc(sizeof(Repartidor));
  repartidor -> semaforo1 = semaforo1;
  repartidor -> semaforo2 = semaforo2;
  repartidor -> semaforo3 = semaforo3;
  repartidor -> bodega = bodega;
  repartidor -> position = 0;
}

int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  // crear el rapartidor y luego hacer un while position
  // sea menor que bodega
}
