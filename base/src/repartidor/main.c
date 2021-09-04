#include <stdio.h>
#include <unistd.h>
#include "repartidor.h"
#include <stdlib.h>

Repartidor* repartidor_init(
    int id,
    int color_s1,
    int color_s2,
    int color_s3,
    int pos_s1,
    int pos_s2,
    int pos_s3,
    int bodega
  )
{
  Repartidor* repartidor = malloc(sizeof(Repartidor));
  repartidor -> pos_s1 = pos_s1;
  repartidor -> pos_s2 = pos_s2;
  repartidor -> pos_s3 = pos_s3;
  repartidor -> bodega = bodega;
  repartidor -> position = 0;
  return repartidor;
}

int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  // crear el rapartidor y luego hacer un while position
  // sea menor que bodega
}
