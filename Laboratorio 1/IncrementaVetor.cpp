#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define TAM_VETOR 10000


int vector[TAM_VETOR];

void inicializaVetor(){
	int i;
	for(i = 0; i < TAM_VETOR; i++){
		vector[i]= 0;	
	}
}

// função a ser executada pelas threads
void *Contador (void *arg) {
	int i;
	int id = *(int *) arg;
	for(i = id; i < TAM_VETOR; i+=2){
		
 		vector[i] = vector[i] + 1;	 	
	}	
  	pthread_exit(NULL);
}

// função principal
int main(void) {
	
  	pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  	int thread; //variavel auxiliar
  	int id[NTHREADS]; // Identificador local
  	
  	inicializaVetor();

	// cria a thread e verifica se foi criada com sucesso
  	for(thread = 0; thread < NTHREADS; thread++) {
  		id[thread] = thread;
    	if (pthread_create(&tid_sistema[thread], NULL, Contador, (void *)&id[thread])) {
      		printf("--ERRO: pthread_create()\n"); 
    	}
  	}
  
  	// espera as threads terminarem
	for(thread = 0; thread < NTHREADS; thread++) {
	    if (pthread_join(tid_sistema[thread], NULL)) {
	         printf("--ERRO: pthread_join() \n"); 
	    } 
	}
	
	// verifica se a incrementação funcionou
	for(thread = 0; thread < TAM_VETOR; thread++){
		if(vector[thread] != 1){
			printf("A incrementacao falhou!\n");
			return 0;
		}
	}
	
    printf("A incrementacao funcionou!\n");
	return 0;


}
