#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//Variaveis globais
int *vetor;
int nthreads;
int bloqueadas = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;


//Funcao da barreira
void barreira(int iter) {

	//inicio secao critica
	pthread_mutex_lock(&mutex);
	
	//se for a última thread a chegar na barreira
	if (bloqueadas == nthreads - iter - 1) {
		bloqueadas = 0;
		pthread_cond_broadcast(&cond);
	}
	//se não for a última thread a chegar na barreira
	else {
		bloqueadas++;
		pthread_cond_wait(&cond, &mutex);
	}

	//fim secao critica
	pthread_mutex_unlock(&mutex);
}

//Funcao executada pelas threads
void * soma(void * arg) {
	int id = *(int *)arg;
	int var; //variavel auxiliar

	for(int i = 1; i <= id; i *= 2) {
		var = vetor[id - i];
		barreira(i); //espera que os elementos sejam lidos
		vetor[id] += var;
		barreira(i); //espera que os elementos sejam calculados
	}
	pthread_exit(NULL);
}

//Funcao principal
int main(int argc, char * argv[]) {

	pthread_t *tid;
	int *id;

	//Recebe e valida os parametros de entrada
	if (argc < 2) {
		fprintf(stderr, "Digite: %s <numero de threads(deve ser potencia de 2)>\n", argv[0]);
		return 1;
	}

	nthreads = atoi(argv[1]);

	

	//Aloca a memória
	id = (int *) malloc(sizeof(int) * nthreads);
	if(id == NULL) {
    	fprintf(stderr, "ERRO - malloc\n");
    	return 2;
	}

	vetor = (int *) malloc(sizeof(int) * nthreads);
	if(vetor == NULL) {
    	fprintf(stderr, "ERRO - malloc\n");
    	return 2;
	}
	
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL) {
    	fprintf(stderr, "ERRO - malloc\n");
    	return 2;
	}

	//Inicializa vetor com 1
	for(int i = 0; i < nthreads; i++) {
		vetor[i] = 1;
	}

	//Inicializa o lock e variavel de condicao
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	//Cria as threads
	for (int i = 0; i < nthreads; i++) {
		id[i] = i;
		pthread_create(&tid[i], NULL, soma, (void *)&id[i]);
	}

	//Espera as threads terminarem
	for (int i = 0; i < nthreads; i++) {
		pthread_join(tid[i], NULL);
	}

	//Imprime vetor de saida
	printf("Vetor de saida: ");
	for(int i = 0; i < nthreads; i++) {
		printf("%d ", vetor[i]);
	}
	printf("\n");

	//Desaloca variaveis
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	
	return 0;
}
