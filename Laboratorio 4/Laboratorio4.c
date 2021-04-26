#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0, y=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 4 */
void *T4 (void *t) {
  int boba1, boba2;

  printf("T4: Comecei\n");
  
  /* faz alguma coisa para gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("-- Seja bem-vindo! --\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  printf("T4: x = %d, vai sinalizar a condicao \n", x);
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread 2 */
void *T2 (void *t) {
  printf("T2: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  while (x == 0) { 
     printf("T2: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("T2: sinal recebido\n");
     pthread_cond_signal(&x_cond);
  }
  
  printf("-- Fique a vontade. --\n");
  x++;
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  
  pthread_exit(NULL);
}

/* Thread 3 */
void *T3 (void *t) {
  printf("T3: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  while (x == 0 ) { 
     printf("T3: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("T3: sinal recebido\n");
  }
  
  printf("-- Sente-se por favor. --\n");
  x++;
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  
  pthread_exit(NULL);
}

/* Thread 1 */
void *T1 (void *t) {
  printf("T1: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  while (x != 3) { 
     printf("T1: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("T1: sinal recebido\n");
     
     if(x != 3){
	pthread_cond_signal(&x_cond);
     }
  }
  pthread_mutex_unlock(&x_mutex); 
  printf("-- Volte sempre! --\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, T4, NULL);
  pthread_create(&threads[1], NULL, T2, NULL);
  pthread_create(&threads[2], NULL, T3, NULL);
  pthread_create(&threads[3], NULL, T1, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
