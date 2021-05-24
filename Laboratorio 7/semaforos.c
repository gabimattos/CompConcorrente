#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0, y=0;
sem_t condt2, condt3;

/* Thread 4 */
void *T4 (void *t) {
  int boba1, boba2;

  printf("T4: Comecei\n");
  
  /* faz alguma coisa para gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("-- Seja bem-vindo! --\n");
  x++;
  printf("T4: x = %d, vai sinalizar a condicao \n", x);
  sem_post(&condt2);

  pthread_exit(NULL);
}

/* Thread 2 */
void *T2 (void *t) {
  printf("T2: Comecei\n");

  sem_wait(&condt2);

  printf("-- Fique a vontade. --\n");
  x++;
  sem_post(&condt2);
  if(x==3){
    sem_post(&condt3);
  }
  pthread_exit(NULL);
}

/* Thread 3 */
void *T3 (void *t) {
  printf("T3: Comecei\n");

  sem_wait(&condt2);
  
  printf("-- Sente-se por favor. --\n");
  x++;
  sem_post(&condt2);
  if(x==3){
    sem_post(&condt3);
  }
  
  pthread_exit(NULL);
}

/* Thread 1 */
void *T1 (void *t) {
  printf("T1: Comecei\n");

  sem_wait(&condt3);

  printf("-- Volte sempre! --\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  //inicia os semaforos
  sem_init(&condt2, 0, 0);
  sem_init(&condt3, 0, 0);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, T1, NULL);
  pthread_create(&threads[1], NULL, T2, NULL);
  pthread_create(&threads[2], NULL, T3, NULL);
  pthread_create(&threads[3], NULL, T4, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");
  
   /* Desaloca variaveis e termina */
   sem_destroy(&condt2);
   sem_destroy(&condt3);

  return 0;
}
