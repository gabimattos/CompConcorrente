#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float *matriz; 
float *vetorSaida;

int nthreads; //numero de threads
int dimMatriz; //dimensao da matriz quadrada


//Funcao que as threads vao executar
void * identificaMultiplos(void *arg) {
	
	int id = *(int*)arg;
	
	for(int i = id; i < dimMatriz; i+=nthreads) {
		for(int j = 0; j < dimMatriz; j++) {
		
			int resto = matriz[i*dimMatriz + j];
		 	
				if(resto % 2 == 0){					
					vetorSaida[i*5] ++;	
				}
				
				if(resto % 3 == 0){
					vetorSaida[i*5 + 1] ++;
					
				}
				
				if(resto % 5 == 0){
					vetorSaida[i*5 + 2] ++;
					
				}
				
				if(resto % 7 == 0){
					vetorSaida[i*5 + 3] ++;

				}
				
				if(resto % 11 == 0){
					vetorSaida[i*5 + 4] ++;

				}
															
		}	  
	}
		
		pthread_exit(NULL);
		

} 

//Função principal
int main(int argc, char* argv[]) {

	pthread_t *tid;            //identificadores das threads no sistema
	double inicio, fim, delta; //valores para registrar o tempo gasto no codigo
	int vetor[5];	    //dimensão do vetor
	int x = 0;                 // variavel auxiliar para preenchimento da matriz
	
	
	//Leitura e avaliacao dos parametros de entrada
	GET_TIME(inicio);
	if(argc < 3) {
		printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return -1;
	}
	
	dimMatriz = atoi(argv[1]);
	nthreads = atoi(argv[2]) > dimMatriz ? dimMatriz : atoi(argv[2]);  //nthreads precisa ser menor que a dimensao
	
	int id[nthreads];   //identificador local da thread

	//Aloca memoria para as estruturas de dados
	matriz = (float *) malloc(sizeof(float)*dimMatriz*dimMatriz);
	if(matriz == NULL) {
		printf("ERRO - malloc\n");
		return -1;
	}
	
	vetorSaida = (float *) malloc(sizeof(float)*dimMatriz*5);
	if(vetorSaida == NULL) {
		printf("ERRO - malloc\n");
		return -1;
	}

	//Inicializa a estrutura de dados de entrada
	for(int i = 0; i < dimMatriz; i++) {
		for(int j = 0; j < dimMatriz; j++) {
			matriz[i*dimMatriz + j] = x + 1;
			x++;	
		}
	}
	
	for(int i = 0; i < dimMatriz*5; i++) {
		vetorSaida[i] = 0;
	}
	
	for (int i=0; i<5; i++){
		vetor[i]=0;
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
		if(pthread_create(&tid[i], NULL, identificaMultiplos, (int*) &id[i])) {
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
	printf("Tempo para encontrar os multiplos e contar: %lfs\n", delta);
	
				
	// armazena no vetor a quantidade de cada um dos multiplos
	
	for (int i=0; i<dimMatriz; i++){
		
		vetor[0] += vetorSaida[i*5];
		vetor[1] += vetorSaida[i*5 + 1];
		vetor[2] += vetorSaida[i*5 + 2];
		vetor[3] += vetorSaida[i*5 + 3];
		vetor[4] += vetorSaida[i*5 + 4];	
			
	}
	

	//Libera a memoria
	GET_TIME(inicio);
	free(matriz);
	free(vetorSaida);
	free(tid);
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo da finalizacao: %lfs\n", delta);
	
	
	//Exibe o vetor resultante
	printf("\n");
	printf("A quantidade de multiplos de 2, 3, 5, 7 e 11, respectivamente, eh:\n");
	for(int i=0; i<5; i++){
	
		printf("%d ", vetor[i]);
		
	}
	printf("\n");
	
	
	pthread_exit(NULL);
	
	return 0;
}


