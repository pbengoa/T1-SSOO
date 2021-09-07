#include <stdio.h>
#include <unistd.h>
#include "semaforo.h"
#include <stdlib.h>
#include "../file_manager/manager.h"

Semaforo* semaforo;

Semaforo* semaforo_init(int id, int distance,int delay, int parentId, int num_created)
{
  Semaforo* semaforo = malloc(sizeof(Semaforo));
  semaforo -> parentId = parentId;
  semaforo -> distance = distance;
  semaforo -> id = id;
  semaforo -> delay = delay;
  semaforo -> color = 0;
  semaforo -> cambios = 0;
  semaforo -> num_created = num_created;
  return semaforo;
}
void handle_nothing(int sig){}

void handle_kill(int sig)
{
  // printf("MATANDO UN SEMAFORO\n");
    FILE *fptr;
    char *number = malloc(sizeof(int));
    sprintf(number, "%d", semaforo -> num_created);

    char *route = malloc(sizeof(char));
    route = "semaforo_" ;

    char path[strlen(route) + strlen(number) + 1];

    strcpy(path, route);
    strcat(path, number);

    char *termi = malloc(sizeof(char));
    termi = ".txt";

    char path2[strlen(path) + strlen(termi) + 1]; 

    strcpy(path2, path);
    strcat(path2, termi);
    // printf("ROUTEEEEE: %s\n", path2);
    fptr = fopen(path2,"w");
    fprintf(fptr,"%d", semaforo -> cambios);
    fclose(fptr);
    free(semaforo);

    exit(1);
}
void handle_alarm(int sig)
{
  send_signal_with_int(semaforo ->parentId, semaforo->id);
  // printf("paso chato %d\n", semaforo -> num_created);
  semaforo -> cambios ++;
  alarm(semaforo -> delay);
}

int main(int argc, char const *argv[])
{
  signal(SIGINT, handle_nothing);
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  // printf("Arg 1: %s, arg 2: %s, arg3: %s\n", argv[1], argv[2], argv[3]);
  int distance = atoi(argv[1]);
  int delay = atoi(argv[2]);
  int parentId = atoi(argv[3]);
  int number = atoi(argv[4]);
  semaforo = semaforo_init(getpid(), distance, delay, parentId, number);
  //int i = 0;
  signal(SIGABRT, handle_kill);
  alarm(semaforo -> delay);
  signal(SIGALRM, handle_alarm);
  while (1);
  // while (i < 50000)
  // {
  //   sleep(semaforo -> delay);
  //   send_signal_with_int(semaforo ->parentId, semaforo->id);
  //   printf("paso chato %d\n", semaforo -> num_created);
  //   semaforo -> cambios ++;
  //   i++;
  // }
}
