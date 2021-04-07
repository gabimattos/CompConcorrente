#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float *matriz1; //primeira matriz quadrada a ser multiplicada
float *matriz2; //segunda matriz quadrada a ser multiplicada
float *saida; //matriz de saida

int nthreads; //numero de threads
int dim; //dimensao da matriz quadrada

//Funcao que as threads vao executar
void * multMatriz(void *arg) {
	int id = *(int*)arg;
	for(int i = id; i < dim; i+=nthreads) {
		for(int j = 0; j < dim; j++) {
			for(int k = 0; k < dim; k++) {
				saida[i*dim + j] += matriz1[i*dim + k]*matriz2[k*dim + j];
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	pthread_t *tid; //identificadores das threads no sistema
	double inicio, fim, delta; //valores para registrar o tempo gasto no codigo

	//Leitura e avaliacao dos parametros de entrada
	GET_TIME(inicio);
	if(argc < 3) {
		printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return -1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]) > dim ? dim : atoi(argv[2]); //nthreads precisa ser menor que a dimensao
	int id[nthreads]; //identificador local da thread

	//Aloca memoria para as estruturas de dados
	matriz1 = (float *) malloc(sizeof(float)*dim*dim);
	if(matriz1 == NULL) {
		printf("ERRO - malloc\n");
		return -1;
	}
	matriz2 = (float *) malloc(sizeof(float)*dim*dim);
	if(matriz2 == NULL) {
		printf("ERRO - malloc\n");
		return -1;
	}
	saida = (float *) malloc(sizeof(float)*dim*dim);
	if(saida == NULL) {
		printf("ERRO - malloc\n");
		return -1;
	}

	//Inicializa as estruturas de dados de entrada e saida
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			matriz1[i*dim + j] = 1;
			matriz2[i*dim + j] = 1;
			saida[i*dim + j] = 0;
		}
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo da inicializacao: %lfs\n", delta);

	//Aloca as estruturas
	GET_TIME(inicio);
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if(tid == NULL) {
		puts("ERRO - malloc");
		return -1;
	}	

	//Cria as threads
	for(int i = 0; i < nthreads; i++) {
		id[i] = i;
		if(pthread_create(&tid[i], NULL, multMatriz, (int*) &id[i])) {
			puts("ERRO - pthread_create");
			return -1;
		}
	}

	//Espera as threads terminarem
	for(int i = 0; i < nthreads; i++) {
		pthread_join(*(tid + i), NULL);
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo da multiplicacao: %lfs\n", delta);

	//Verifica os resultados
	int check = 1;
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			if(saida[i*dim + j] != dim) {
				check = 0;
				break;
			}
		}
	}

	//Libera a memoria
	GET_TIME(inicio);
	free(matriz1);
	free(matriz2);
	free(saida);
	free(tid);
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo da finalizacao: %lfs\n", delta);

	check == 1 ?
		printf("A multiplicacao foi calculada corretamente!\n") :
		printf("Ocorreu um erro na multiplicacao!\n");

	return 0;
}
