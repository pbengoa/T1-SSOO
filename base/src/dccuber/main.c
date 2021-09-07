#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "../file_manager/manager.h"
int *lights;
int *id_semaforos;
int *distances;
int *repartidores;
int creados = 0;
int *datos;
pid_t fabrica;
int liberados = 0;
int rep_vivos = 0;

void handle_change_repartidor(int sig, siginfo_t *siginfo, void *context)
{
  int number_received = siginfo->si_value.sival_int;
  for (int i = 0; i < creados; i++)
  {
    if (number_received == repartidores[i])
    {
      repartidores[i] = 0;
      rep_vivos -= 1;
    }
  }
}
void handle_nothing(int sig){
  pid_t wpid;
  int status = 0;
  while ((wpid = wait(&status)) > 0);
  exit(1);
}

void handle_kill_repartidor(int sig)
{
  pid_t wpid;
  int status = 0;
  // printf("kill repartidor: %d\n", repartidores[0]);
  for (int i=0; i < creados; i++)
  {
    if (repartidores[i] != 0)
    {
      // printf("REPARTIDOR: %d\n", repartidores[i]);
      kill(repartidores[i], SIGABRT);
    }
  }
  if (rep_vivos > 0)
  {
    while ((wpid = wait(&status)) > 0);
    exit(1);
  }
  else
  {
    exit(1);
  }

}

void handle_repartidor(int sig)
{
  //int status;
  //pid_t pid = 0;
  if (creados < datos[1])
  {
    pid_t repartidor = fork();
    if (repartidor == 0)
    {
      char *d1 = malloc(sizeof(int));
      sprintf(d1, "%d", distances[0]);

      char *d2 = malloc(sizeof(int));
      sprintf(d2, "%d", distances[1]);

      char *d3 = malloc(sizeof(int));
      sprintf(d3, "%d", distances[2]);

      char *d4 = malloc(sizeof(int));
      sprintf(d4, "%d", distances[3]);

      char *l1 = malloc(sizeof(int));
      sprintf(l1, "%d", lights[0]);

      char *l2 = malloc(sizeof(int));
      sprintf(l2, "%d", lights[1]);

      char *l3 = malloc(sizeof(int));
      sprintf(l3, "%d", lights[2]);

      char *num_created = malloc(sizeof(int));
      sprintf(num_created, "%d", creados);

      char * args[] ={"./repartidor", l1, l2, l3, 
      d1, d2, d3, d4, num_created, NULL};
      execv(args[0], args);
    }
    else
    {
      //printf("--------------------------------------\n");
      //printf("REPARTIDOR NUMERO %d, ID: %d\n", creados, repartidor);
      repartidores[creados] = repartidor;
      creados ++;
      rep_vivos ++;
      alarm(datos[0]);
      // waitpid(repartidor, NULL, 0);
      //printf("CREADOS %d\n", creados);
  
    }
  }
  else
  {
    // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    while (repartidores[creados - 1] != 0);
    exit(1);
  }
}

void handle_sigint(int sig, siginfo_t *siginfo, void *context)
{
  int number_received = siginfo->si_value.sival_int;

  if (id_semaforos[0] == number_received){
    printf("SEMAFORO 1\n");
    if (lights[0] == 0)
    {
      lights[0] = 1;

      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 1);
      }
    }
    else
    {
      lights[0] = 0;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 1);
      }

    }
  }
  else if(id_semaforos[1] == number_received){
    printf("SEMAFORO 2\n");
    if (lights[1] == 0)
    {
      lights[1] = 1;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 2);
      }

    }
    else
    {
      lights[1] = 0;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 2);
      }

    }
  }
  else if(id_semaforos[2] == number_received){
    printf("SEMAFORO 3\n");
    if (lights[2] == 0)
    {
      lights[2] = 1;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 3);
      }

    }
    else
    {
      lights[2] = 0;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 3);
      }

    }
  }

  else
  {

    if (id_semaforos[0] == 0)
    {
      id_semaforos[0] = number_received;
    }
    else if(id_semaforos[1] == 0)
    {
      id_semaforos[1] = number_received;
    }
    else if(id_semaforos[2] == 0)
    {
      id_semaforos[2] = number_received;
    }
  }

}

void handle_killAll(int sig)
{
  // printf("hfsgjgfdsfghjkgfds\n");
  kill(fabrica, SIGABRT);
}

int main(int argc, char const *argv[])
{
  signal(SIGINT, handle_killAll);
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());
  char *filename = "input.txt";
  InputFile *data_in = read_file(filename);

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");
  distances = calloc(4,sizeof(int));
  datos = calloc (5, sizeof(int));
  lights = calloc(3,sizeof(int));
  id_semaforos = calloc(3,sizeof(int));
  
  printf("\t- ");
  for (int i = 0; i < 4; i++)
  {
    int x = atoi(data_in->lines[0][i]);
    distances[i] = x;
    printf("%s, ", data_in->lines[0][i]);
     
  }
  printf("\n");
  //printf("la distancia es de %i\n", distances[1]);
  printf("\t- ");
  for (int i = 0; i < 5; i++)
  {
    int y = atoi(data_in->lines[1][i]);
    datos[i] = y;
    printf("%s, ", data_in->lines[1][i]);
  }
  repartidores = calloc(datos[1], sizeof(int));
  printf("\n");
  // crear array
  fabrica = fork();
  if(fabrica == 0)
  {
    // This code will be executed only by the child
    // printf("hola\n");
    signal(SIGINT, handle_nothing);
    signal(SIGABRT, handle_kill_repartidor);
    signal(SIGALRM, handle_repartidor);
    alarm(datos[0]);
    connect_sigaction(SIGUSR1, handle_sigint);
    connect_sigaction(SIGUSR2, handle_change_repartidor);
    for (int i =0; i < 5000;i++){
      pause();
    }
  }
  else
  {
      pid_t semaforo1 = fork();
      if(semaforo1 == 0)
      {
          // We are the child
          // Aca se crean los argumentos que se entregaran a continuacion a EXECV
          char * pid_char = calloc(1, sizeof(int)); 
          sprintf(pid_char, "%d", fabrica);
          char * distance = calloc(1, sizeof(int)); 
          sprintf(distance, "%d", distances[0]);
          char * dato = calloc(1, sizeof(int)); 
          sprintf(dato, "%d", datos[2]);
          char *args[] = {"./semaforo", distance, dato, pid_char, "1",NULL};
          execv(args[0], args);
          //Esto se repite igual en todos los casos
      }
      else 
      {
        send_signal_with_int(fabrica, semaforo1);
        pid_t semaforo2 = fork();
        if(semaforo2 == 0)
        {
            // We are the child
          char * pid_char = calloc(1, sizeof(int)); 
          sprintf(pid_char, "%d", fabrica);

          char * distance = calloc(1, sizeof(int)); 
          sprintf(distance, "%d", distances[1]);

          char * dato = calloc(1, sizeof(int)); 
          sprintf(dato, "%d", datos[3]);
          char *args[] = {"./semaforo", distance, dato, pid_char, "2", NULL};
          execv(args[0], args);
        }
        else
        {
          send_signal_with_int(fabrica, semaforo2);
          // id_semaforos[1] = semaforo2;
          pid_t semaforo3 = fork();
          if(semaforo3 == 0)
          {
              // We are the child
            char * pid_char = calloc(1, sizeof(int)); 
            sprintf(pid_char, "%d", fabrica);

            char * distance = calloc(1, sizeof(int)); 
            sprintf(distance, "%d", distances[2]);
            char * dato = calloc(1, sizeof(int)); 
            sprintf(dato, "%d",  datos[4]);
            char *args[] = {"./semaforo", distance, dato, pid_char, "3", NULL};
            execv(args[0], args);
          }
          else
          {
            // id_semaforos[2] = semaforo3;
            send_signal_with_int(fabrica, semaforo3);
            waitpid(fabrica, NULL, 0);
            kill(semaforo1, SIGABRT);
            kill(semaforo2, SIGABRT);
            kill(semaforo3, SIGABRT);
            waitpid(semaforo1, NULL, 0);
            waitpid(semaforo2, NULL, 0);
            waitpid(semaforo3, NULL, 0);
          }
        }
      }
      
  }
  printf("Liberando memoria...\n");

  free(distances);
  free(datos);
  free(lights);
  free(id_semaforos);
  free(repartidores);

  input_file_destroy(data_in);
}