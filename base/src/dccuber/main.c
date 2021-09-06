#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "../file_manager/manager.h"
// global int *lights = calloc(3,sizeof(int));
int *lights;
int *id_semaforos;
int *distances;
int *repartidores;
int creados = 0;
int *datos;
pid_t fabrica;
int liberados = 0;

// void handle_finish_repartidor(int sig, siginfo_t *siginfo, void *context)
// {
//   printf("tttttttttttttttttttttttttttttttttttttttt\n");
//   liberados ++;
//   int number_received = siginfo->si_value.sival_int;
//   kill(number_received, SIGKILL);


// }

void handle_kill_repartidor(int sig)
{
  printf("kill repartidor\n");
  for (int i=0; i < creados; i++)
  {
    printf("REPARTIDOR: %d\n", repartidores[i]);
    kill(repartidores[i], SIGABRT);
  }
  if (creados == 0)
  {
    exit(1);
  }
  else{
    waitpid(repartidores[creados - 1], NULL, 0);
  }

}

void handle_repartidor(int sig)
{
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
      printf("--------------------------------------\n");
      printf("REPARTIDOR NUMERO %d, ID: %d\n", creados, repartidor);
      repartidores[creados] = repartidor;
      creados ++;
      alarm(datos[0]);
      waitpid(repartidor, NULL, 0);
      // liberados ++;
      // printf("SALIMOs DE %d\n", repartidor);

    }
  }
}

void handle_sigint(int sig, siginfo_t *siginfo, void *context)
{
  int number_received = siginfo->si_value.sival_int;
  //printf("THE VALUE %d\n", number_received);
  // aca se chequea que el id que se recibio sea igual a algun valor de la lista
  // entonces se cambia el color en el caso que coincida
  // y luego hay que notificar a los repartidores
  if (id_semaforos[0] == number_received){
    //printf("SE INGRESA EL PRIMER SEMORO POR SEGUNDA VEZ: %d\n", number_received);
    // Aca se revisara el color actual del semaforo y se cambiara
    if (lights[0] == 0)
    {
      lights[0] = 1;
      //printf("ESTABA EN VERDE 1\n");
      // ahora hay que mandar la señal a los repartidores
      // for repatidor: mando señal
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
      //printf("ESTABA EN ROJO 1\n");
      // ahora hay que mandar la señal a los repartidores
    }
  }
  else if(id_semaforos[1] == number_received){
    //printf("SE INGRESA EL segundo SEMORO POR SEGUNDA VEZ: %d\n", number_received);
    // Aca se revisara el color actual del semaforo y se cambiara
    if (lights[1] == 0)
    {
      lights[1] = 1;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 2);
      }
      //printf("ESTABA EN VERDE 2\n");
      // ahora hay que mandar la señal a los repartidores
    }
    else
    {
      lights[1] = 0;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 2);
      }
      //printf("ESTABA EN ROJO 2\n");
      // ahora hay que mandar la señal a los repartidores
    }
  }
  else if(id_semaforos[2] == number_received){
    //printf("SE INGRESA EL tercer SEMORO POR SEGUNDA VEZ: %d\n", number_received);
    // Aca se revisara el color actual del semaforo y se cambiara
    if (lights[2] == 0)
    {
      lights[2] = 1;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 3);
      }
      //printf("ESTABA EN VERDE 3\n");
      // ahora hay que mandar la señal a los repartidores
    }
    else
    {
      lights[2] = 0;
      for (int i = 0; i < creados; i++)
      {
        send_signal_with_int(repartidores[i], 3);
      }
      //printf("ESTABA EN ROJO 3\n");
      // ahora hay que mandar la señal a los repartidores
    }
  }

  else
  {
    // printf("Id semaforo: %d\n", number_received);
    // printf("Id lista semaforo 1: %d\n", id_semaforos[0]);
    // Aca reviso si la posicion 0, 1 y 2 de la lista id_semaforo estan con valor 0
    // Si estan con valor 0 entonces es porque todavia no ha cambiado el semaforo por primera vez
    // Entonces se agrega el id a la lista y y se cambia el color
    if (id_semaforos[0] == 0)
    {
      id_semaforos[0] = number_received;
      //printf("SE INGRESA EL PRIMER SEMORO POR PRIMERA VEZ: %d\n", number_received);
    }
    else if(id_semaforos[1] == 0)
    {
      id_semaforos[1] = number_received;
      //printf("SE INGRESA EL segundo SEMORO POR PRIMERA VEZ: %d\n", number_received);
    }
    else if(id_semaforos[2] == 0)
    {
      id_semaforos[2] = number_received;
      //printf("SE INGRESA EL tercer SEMORO POR PRIMERA VEZ: %d\n", number_received);
    }
  }

}

void handle_killAll(int sig)
{
  printf("hfsgjgfdsfghjkgfds\n");
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
    signal(SIGABRT, handle_kill_repartidor);
    signal(SIGALRM, handle_repartidor);
    alarm(datos[0]);
    connect_sigaction(SIGUSR1, handle_sigint);
    for (int i =0; i < 5000;i++){
      if (liberados == datos[1])
      {
        exit(1);
      }
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
  input_file_destroy(data_in);
}