#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long int n; 
int nthreads;

// fluxo das threads
void * somaValores(void * arg) {
	long long int id = (long long int) arg; // id da thread
	double *somaLocal; // variavel local da soma
	somaLocal = (double*) malloc(sizeof(double));
	if(somaLocal == NULL) {		
		exit(1);
	}
	long long int tamBloco = n/nthreads; // tamanho do bloco de cada thread
	long long int ini = id * tamBloco; // elemento inicial do bloco da thread
	long long int fim; //elemento final(nao processado) do bloco da thread
		if(id == nthreads - 1) fim = n;
		else fim = ini + tamBloco; //trata o resto se houver
   	//soma os elementos do bloco da thread
		 
	for(long int i = ini; i < fim; i++)
		if(i % 2 == 0) *somaLocal += 1/((double)i*2 + 1);
		else *somaLocal += -1/((double)i*2 + 1);

	//retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}

//fluxo principal
int main(int argc, char *argv[]) {
	double somaSeq = 0; // soma sequencial
	double somaConc = 0; // soma concorrente
	double ini, fim; // tomada de tempo
	pthread_t *tid; // identificadores das threads no sistema	
	double *retorno; // valor de retorno das threads

	//recebe e valida os parametos de entrada 

	if(argc < 3) {
		fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
		return 1;
	}
	n = atoll(argv[1]);
	nthreads = atoi(argv[2]);

	//soma sequencial dos elementos
	GET_TIME(ini);
	for(long long int i=0; i<n; i++)
		if(i % 2 == 0) somaSeq += 1/((double)i*2 + 1);
		else somaSeq += -1/((double)i*2 + 1);
		
	GET_TIME(fim);
	printf("Tempo sequencial:  %lf\n", fim-ini);

	//soma concorrente dos elementos
	GET_TIME(ini);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid==NULL) {
		printf("ERRO--malloc\n");
		return 2;
	}
	//criar as threads
	for(long long int i=0; i<nthreads; i++) {
		if(pthread_create(tid+i, NULL, somaValores, (void*) i)){
			printf("ERRO--pthread_create\n");
			return 3;
		}
	}
	//aguardar o termino das threads
	for(long long int i=0; i<nthreads; i++) {
		if(pthread_join(*(tid+i), (void**) &retorno)){
			printf("ERRO--pthread_join\n");
			return 4;
		}
		//soma global
		somaConc += *retorno;
	}
	GET_TIME(fim);
	printf("Tempo concorrente:  %lf\n", fim-ini);

	//exibir os resultados
	printf("Soma sequencial:  %.15lf\n", 4 * somaSeq);
	printf("Soma concorrente: %.15lf\n", 4 * somaConc);
	printf("Valor de PI: %.15lf\n", M_PI);

	//libera as areas de memoria alocadas
	free(tid);

	return 0;
}
