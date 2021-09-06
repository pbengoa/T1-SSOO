#include <stdio.h>
#include <unistd.h>
#include "repartidor.h"
#include <stdlib.h>
#include <signal.h>

#include "../file_manager/manager.h"
Repartidor* repartidor;

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
  repartidor -> id = id;

  repartidor -> color_s1 = color_s1;
  repartidor -> color_s2 = color_s2;
  repartidor -> color_s3 = color_s3;

  repartidor -> pos_s1 = pos_s1;
  repartidor -> pos_s2 = pos_s2;
  repartidor -> pos_s3 = pos_s3;
  repartidor -> bodega = bodega;

  repartidor -> position = 0;
  printf("CREADO EL GIL\n");
  return repartidor;
}

void handler_change_light(int sig, siginfo_t *siginfo, void *context)
{
  printf("CAMBIANDO LA LUZ&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  int number_received = siginfo->si_value.sival_int;
  if (number_received == 1)
  {
    if (repartidor -> color_s1 == 0)
    {
      repartidor -> color_s1 = 1;
    }
    else
    {
      repartidor -> color_s1 = 0;
    }
  }
  else if (number_received == 2)
  {
    if (repartidor -> color_s2 == 0)
    {
      repartidor -> color_s2 = 1;
    }
    else
    {
      repartidor -> color_s2 = 0;
    }
  }
  else if (number_received == 3)
  {
    if (repartidor -> color_s3 == 0)
    {
      repartidor -> color_s3 = 1;
    }
    else
    {
      repartidor -> color_s3 = 0;
    }
  }
}

int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  int color_s1 = atoi(argv[1]);
  int color_s2 = atoi(argv[2]);
  int color_s3 = atoi(argv[3]);

  int pos_s1 = atoi(argv[4]);
  int pos_s2 = atoi(argv[5]);
  int pos_s3 = atoi(argv[6]);
  int bodega = atoi(argv[7]);

  printf("VALORES: %d, %d, %d, %d, %d, %d, %d\n", 
  color_s1, 
  color_s2, 
  color_s3, 
  pos_s1,
  pos_s2,
  pos_s3,
  bodega);

  repartidor = repartidor_init(
    getpid(), 
    color_s1, 
    color_s2,
    color_s3,
    pos_s1,
    pos_s2,
    pos_s3,
    bodega
    );
  connect_sigaction(SIGUSR1, handler_change_light);
  while (repartidor ->position < repartidor ->bodega)
  {
    printf("REPATIDOR %d POSITION: %d\n", getpid(), repartidor->position);
    sleep(1);
    if (repartidor -> position + 1 == repartidor ->pos_s1 && repartidor -> color_s1 == 0)
    {
      repartidor -> position ++;
    }
    else if (repartidor -> position + 1 == repartidor ->pos_s2 && repartidor -> color_s2 == 0)
    {
      repartidor -> position ++;
    }
    else if (repartidor -> position + 1 == repartidor ->pos_s3 && repartidor -> color_s3 == 0)
    {
      repartidor -> position ++;
    }
    else if (repartidor -> position + 1 != repartidor ->pos_s1 
    && repartidor -> position + 1 != repartidor ->pos_s2
    && repartidor -> position + 1 != repartidor ->pos_s3)
    {
      repartidor -> position ++;
    }
    //else if (repartidor -> position == repartidor ->bodega)
    //{
      // escribir archivo
      // hacer un kill
    //}

    // hay que revisar la pos y la luz
    // avanzar

    // atento para recibir señal
    
  }
  // mandar señal termino
  printf("HE TERMINADO\n");
  
}
