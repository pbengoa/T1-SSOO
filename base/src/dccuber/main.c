#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../file_manager/manager.h"

void handle_sigint(int sig)
{
  printf("FUNCIONOOOOOOOOOOOO: %d\n", sig);

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
  if(fabrica < 0)
  {
      printf("NOT SUCH FILE IN DIR\n");
  }
  else if(fabrica == 0)
  {
      // This code will be executed only by the child
      printf(" IN DIR\n");
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
          printf("semaforo1\n");
          execv(args[0], args);
          //Esto se repite igual en todos los casos
      }
      else 
      {
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
          printf("semaforo2\n");
          execv(args[0], args);
        }
        else
        {
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
            printf("semaforo3\n");
            execv(args[0], args);
          }
        }
      }
      
  }
  while (true)
  {
    signal(SIGINT, handle_sigint);
    sleep(4);
    return 0;
  }



  printf("Liberando memoria...\n");
  input_file_destroy(data_in);
}