#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../file_manager/manager.h"
// global int *lights = calloc(3,sizeof(int));
int *lights;
int *id_semaforos;

void handle_sigint(int sig, siginfo_t *siginfo, void *context)
{
  int number_received = siginfo->si_value.sival_int;
  printf("THE VALUE %d\n", number_received);
  // aca se chequea que el id que se recibio sea igual a algun valor de la lista
  // entonces se cambia el color en el caso que coincida
  // y luego hay que notificar a los repartidores
  printf("LOS SEMAFOROS 1: %d, EL SEGUNDO: %d, EL TERCERO: %d\n", id_semaforos[0], id_semaforos[1], id_semaforos[2]);
  if (id_semaforos[0] == number_received){
    printf("SE INGRESA EL PRIMER SEMORO POR SEGUNDA VEZ: %d\n", number_received);
    // Aca se revisara el color actual del semaforo y se cambiara
    if (lights[0] == 0)
    {
      lights[0] = 1;
      printf("ESTABA EN VERDE 1\n");
      // ahora hay que mandar la señal a los repartidores
    }
    else
    {
      lights[0] = 0;
      printf("ESTABA EN ROJO 1\n");
      // ahora hay que mandar la señal a los repartidores
    }
  }
  else if(id_semaforos[1] == number_received){
    printf("SE INGRESA EL segundo SEMORO POR SEGUNDA VEZ: %d\n", number_received);
    // Aca se revisara el color actual del semaforo y se cambiara
    if (lights[1] == 0)
    {
      lights[1] = 1;
      printf("ESTABA EN VERDE 2\n");
      // ahora hay que mandar la señal a los repartidores
    }
    else
    {
      lights[1] = 0;
      printf("ESTABA EN ROJO 2\n");
      // ahora hay que mandar la señal a los repartidores
    }
  }
  else if(id_semaforos[2] == number_received){
    printf("SE INGRESA EL tercer SEMORO POR SEGUNDA VEZ: %d\n", number_received);
    // Aca se revisara el color actual del semaforo y se cambiara
    if (lights[2] == 0)
    {
      lights[2] = 1;
      printf("ESTABA EN VERDE 3\n");
      // ahora hay que mandar la señal a los repartidores
    }
    else
    {
      lights[2] = 0;
      printf("ESTABA EN ROJO 3\n");
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
      printf("SE INGRESA EL PRIMER SEMORO POR PRIMERA VEZ: %d\n", number_received);
    }
    else if(id_semaforos[1] == 0)
    {
      id_semaforos[1] = number_received;
      printf("SE INGRESA EL segundo SEMORO POR PRIMERA VEZ: %d\n", number_received);
    }
    else if(id_semaforos[2] == 0)
    {
      id_semaforos[2] = number_received;
      printf("SE INGRESA EL tercer SEMORO POR PRIMERA VEZ: %d\n", number_received);
    }
  }

}

int main(int argc, char const *argv[])
{
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  char *filename = "input.txt";
  InputFile *data_in = read_file(filename);

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");
  int *distances = calloc(4,sizeof(int));
  int *datos = calloc (5, sizeof(int));
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
  printf("\n");
  // crear array
  pid_t fabrica = fork();
  if(fabrica == 0)
  {
      // This code will be executed only by the child
      printf(" IN DIR\n");

      while (true)
      {
        // signal(SIGUSR1, handle_sigint);
        connect_sigaction(SIGUSR1, handle_sigint);
      }
      
      // sleep(5);
      // printf(" fbsffsnfsnR\n");
      // for ()
      // pid_t repartidor = fork();
      // if (repartidor == 0) 
      // {
      //   execlp("repartidor","semaforo", "main.c",NULL);
      // }
      // sleep(5)
  }
  else
  {
      pid_t semaforo1 = fork();
      if(semaforo1 == 0)
      {
          // We are the child
          // Aca se crean los argumentos que se entregaran a continuacion a EXECV
          char * pid_char = malloc(sizeof(int)); 
          sprintf(pid_char, "%d", fabrica);
          char * distance = malloc(sizeof(int)); 
          sprintf(distance, "%d", distances[0]);
          char * dato = malloc(sizeof(int)); 
          sprintf(dato, "%d", datos[2]);
          char *args[] = {"./semaforo", distance, dato, pid_char, NULL};
          execv(args[0], args);
          //Esto se repite igual en todos los casos
      }
      else 
      {
        printf(" fbsffsnfsnR\n");
        send_signal_with_int(fabrica, semaforo1);
        pid_t semaforo2 = fork();
        if(semaforo2 == 0)
        {
            // We are the child
          char * pid_char = malloc(sizeof(int)); 
          sprintf(pid_char, "%d", fabrica);

          char * distance = malloc(sizeof(int)); 
          sprintf(distance, "%d", distances[1]);

          char * dato = malloc(sizeof(int)); 
          sprintf(dato, "%d", datos[3]);
          char *args[] = {"./semaforo", distance, dato, pid_char,NULL};
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
            char * pid_char = malloc(sizeof(int)); 
            sprintf(pid_char, "%d", fabrica);

            char * distance = malloc(sizeof(int)); 
            sprintf(distance, "%d", distances[2]);
            char * dato = malloc(sizeof(int)); 
            sprintf(dato, "%d",  datos[4]);
            char *args[] = {"./semaforo", distance, dato, pid_char,NULL};
            execv(args[0], args);
          }
          else
          {
            // id_semaforos[2] = semaforo3;
            send_signal_with_int(fabrica, semaforo3);
          }
        }
      }
      
  }
  // while (true)
  // {
  //   // signal(SIGUSR1, handle_sigint);
  //   connect_sigaction(SIGUSR1, handle_sigint);
  // }

  printf("Liberando memoria...\n");
  input_file_destroy(data_in);
}