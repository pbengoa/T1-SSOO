#include <stdio.h>
#include <unistd.h>
#include "semaforo.h"
#include <stdlib.h>

Semaforo* semaforo_init(int id, int delay, int parentId)
{
  Semaforo* semaforo = malloc(sizeof(Semaforo));
  semaforo -> parentId = parentId;
  semaforo -> id = id;
  semaforo -> delay = delay;
  semaforo -> color = 1;
  return semaforo;
}

int main(int argc, char const *argv[])
{
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  printf("Arg 1: %s, arg 2: %s, arg3: %s\n", argv[1], argv[2], argv[3]);
  int id = atoi(argv[1]);
  int delay = atoi(argv[2]);
  int parentId = atoi(argv[3]);
  Semaforo* semaforo = semaforo_init(id, delay, parentId);
  printf("semaforo data: %d, %d, %d\n", semaforo->id,semaforo->delay, semaforo->parentId);
  sleep(1);
  kill(semaforo ->parentId, SIGINT);
  // while ()
  // {
  //   sleep(semaforo ->delay);
  //   kill(semaforo ->parentId, SIGINT);

  // }
}
