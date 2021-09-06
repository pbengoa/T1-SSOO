#include <stdio.h>
#include <unistd.h>
#include "repartidor.h"
#include <stdlib.h>
#include <signal.h>
#include <string.h>

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
    int bodega,
    int num_created
  )
{
  Repartidor* repartidor = malloc(sizeof(Repartidor));
  repartidor -> id = id;
  repartidor -> num_created = num_created;

  repartidor -> color_s1 = color_s1;
  repartidor -> color_s2 = color_s2;
  repartidor -> color_s3 = color_s3;

  repartidor -> pos_s1 = pos_s1;
  repartidor -> pos_s2 = pos_s2;
  repartidor -> pos_s3 = pos_s3;
  repartidor -> bodega = bodega;

  repartidor -> pasamos_1 = -1;
  repartidor -> pasamos_2 = -1;
  repartidor -> pasamos_3 = -1;
  repartidor -> llegamos = -1;

  repartidor -> position = 0;
  return repartidor;
}

void handler_change_light(int sig, siginfo_t *siginfo, void *context)
{
  int number_received = siginfo->si_value.sival_int;
  printf("cambio de color: %d\n", number_received);
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

void handle_kill(int sig)
{
    printf("HOMICIDIO\n");
    FILE *fptr;
    char *number = malloc(sizeof(int));
    sprintf(number, "%d", repartidor -> num_created);

    char *route = malloc(sizeof(char));
    route = "repartidor_" ;

    char path[strlen(route) + strlen(number) + 1];

    strcpy(path, route);
    strcat(path, number);

    char *termi = malloc(sizeof(char));
    termi = ".txt";

    char path2[strlen(path) + strlen(termi) + 1]; 

    strcpy(path2, path);
    strcat(path2, termi);
    printf("ROUTEEEEE: %s\n", path2);
    fptr = fopen(path2,"w");
    fprintf(fptr,"%d,%d,%d,%d",
    repartidor -> pasamos_1, 
    repartidor -> pasamos_2, 
    repartidor -> pasamos_3,
    repartidor -> llegamos
    );
    fclose(fptr);
    exit(1);
}

void handle_kill_2(int sig)
{
    printf("HOMICIDIO\n");
    FILE *fptr;
    char *number = malloc(sizeof(int));
    sprintf(number, "%d", repartidor -> num_created);

    char *route = malloc(sizeof(char));
    route = "repartidor_" ;

    char path[strlen(route) + strlen(number) + 1];

    strcpy(path, route);
    strcat(path, number);

    char *termi = malloc(sizeof(char));
    termi = ".txt";

    char path2[strlen(path) + strlen(termi) + 1]; 

    strcpy(path2, path);
    strcat(path2, termi);
    printf("ROUTEEEEE: %s\n", path2);
    fptr = fopen(path2,"w");
    fprintf(fptr,"%d,%d,%d,%d",
    repartidor -> pasamos_1, 
    repartidor -> pasamos_2, 
    repartidor -> pasamos_3,
    repartidor -> llegamos
    );
    fclose(fptr);
    exit(1);
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
  int num_created = atoi(argv[8]);

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
    bodega,
    num_created
    );
    // recepcion de señal de termino, crear funicion y escribir archivo
  signal(SIGABRT, handle_kill);
  signal(SIGKILL, handle_kill_2);
  connect_sigaction(SIGUSR1, handler_change_light);
  int contador = 0;
  while (repartidor ->position < repartidor ->bodega)
  {
    printf("REPATIDOR %d POSITION: %d\n", getpid(), repartidor->position);
    contador ++;
    sleep(1);
    if (repartidor -> position + 1 == repartidor ->pos_s1 && repartidor -> color_s1 == 0)
    {
      repartidor -> position ++;
      repartidor -> pasamos_1 = contador;
    }
    else if (repartidor -> position + 1 == repartidor ->pos_s2 && repartidor -> color_s2 == 0)
    {
      repartidor -> position ++;
      repartidor -> pasamos_2 = contador;
    }
    else if (repartidor -> position + 1 == repartidor ->pos_s3 && repartidor -> color_s3 == 0)
    {
      repartidor -> position ++;
      repartidor -> pasamos_3 = contador;
    }
    else if (repartidor -> position + 1 != repartidor ->pos_s1 
    && repartidor -> position + 1 != repartidor ->pos_s2
    && repartidor -> position + 1 != repartidor ->pos_s3
    && repartidor -> position != repartidor ->bodega)
    {
      repartidor -> position ++;
    }

    // hay que revisar la pos y la luz
    // avanzar

    // atento para recibir señal
    
  }
   // escribir archivo
  repartidor -> llegamos = contador;
  printf("%d\n", repartidor -> llegamos);
  FILE *fptr;
  char *number = malloc(sizeof(int));
  sprintf(number, "%d", repartidor -> num_created);

  char *route = malloc(sizeof(char));
  route = "repartidor_" ;

  char path[strlen(route) + strlen(number) + 1];

  strcpy(path, route);
  strcat(path, number);

  char *termi = malloc(sizeof(char));
  termi = ".txt";

  char path2[strlen(path) + strlen(termi) + 1]; 

  strcpy(path2, path);
  strcat(path2, termi);
  printf("ROUTEEEEE: %s\n", path2);
  fptr = fopen(path2,"w");
  fprintf(fptr,"%d,%d,%d,%d",
  repartidor -> pasamos_1, 
  repartidor -> pasamos_2, 
  repartidor -> pasamos_3,
  repartidor -> llegamos
  );
  fclose(fptr);
  printf("HE TERMINADO %d\n", num_created);
  exit(1);  
}
