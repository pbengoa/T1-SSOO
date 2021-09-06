#include <stdio.h>
#include <unistd.h>
#include "semaforo.h"
#include <stdlib.h>
#include "../file_manager/manager.h"

Semaforo* semaforo_init(int id, int distance,int delay, int parentId)
{
  Semaforo* semaforo = malloc(sizeof(Semaforo));
  semaforo -> parentId = parentId;
  semaforo -> distance = distance;
  semaforo -> id = id;
  semaforo -> delay = delay;
  semaforo -> color = 0;
  semaforo -> cambios = 0;
  return semaforo;
}

int main(int argc, char const *argv[])
{
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  printf("Arg 1: %s, arg 2: %s, arg3: %s\n", argv[1], argv[2], argv[3]);
  int distance = atoi(argv[1]);
  int delay = atoi(argv[2]);
  int parentId = atoi(argv[3]);
  Semaforo* semaforo = semaforo_init(getpid(), distance, delay, parentId);
  // printf("semaforo data: %d, %d, %d\n", semaforo->id,semaforo->delay, semaforo->parentId);
  int i = 0;
  while (i < 500)
  {
    sleep(semaforo -> delay);
    semaforo -> cambios ++;
    send_signal_with_int(semaforo ->parentId, semaforo->id);
    i++;
  }


  
  // while ()
  // {
  //   sleep(semaforo ->delay);
  //   kill(semaforo ->parentId, SIGINT);

  // }
}
