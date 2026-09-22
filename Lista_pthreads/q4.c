#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_INCOGNITAS 10
#define NUM_THREADS 4

double vet_X[NUM_INCOGNITAS] = {1.0};
int vet_coef[NUM_INCOGNITAS][NUM_INCOGNITAS]; 
double vet_result[NUM_INCOGNITAS] = {1.0};

pthread_barrier_t barreira;
pthread_t threads[NUM_THREADS];

typedef struct {
    int id;
    int* subarray_x;
} thread;


double * somatorio_jacobi(int vet_coeficientes[NUM_INCOGNITAS][NUM_INCOGNITAS], double * vet_x_past , double* vet_x_new) {

    int qtd_variaveis = sizeof(vet_x_new)/sizeof(double);
            
        for ( int i = 0; i < qtd_variaveis; i++)  {
            double soma = 0;
            for (int k = 0; k < NUM_INCOGNITAS; k ++) {
                if (i != k) {
                    soma += vet_coeficientes[i][k] * vet_x_past[k]; // soma com pesos para os coeficietes e as incógnitas com valores da interação anterior
                }
            }
            vet_x_new[i] = (vet_result[i] - soma)/vet_coeficientes[i][i];
        }

    return vet_x_new;
}

void escalonador( thread * thread_[NUM_THREADS]) {
    int particao = NUM_INCOGNITAS/ NUM_THREADS; // particao de quantos termos teremos dentro de cada subarray
    int resto = NUM_INCOGNITAS % NUM_THREADS; // quando tivermos uma divisão nao inteira, direcionamos os "resto" valores para as threads até acabar
    int k=0;
    int i = 0;

    for ( ; i<NUM_THREADS; i ++) {
        int j =0;
        thread_[i]->subarray_x = NULL;
        thread_[i]->subarray_x = (int*)malloc(particao * sizeof(int));

        if (thread_[i]->subarray_x == NULL) {
            printf("Erro ao alocar memória para o escalonador");
            return(-1);
        }
        for ( ; k<particao; k++){
            thread_[i]->subarray_x[j] = 0 ; //setei como 0 mas tá errado, so nao tava conseguindo achar o que colocar, é o sono
        }
    }

    if (resto) {
        i = 0;
        while (!resto) {
            if (i >=NUM_THREADS) i = 0;

            resto--;
        }

    }
}

void * executar_thread(void* funcao) {

}

void init_vet_coef() {

    //cada coef como i+j
    for( int i=0 ; i, NUM_INCOGNITAS; i++) {
        for ( int k = 0; k < NUM_INCOGNITAS; k++) {
            if(i == 0 && k == 0) {
                vet_coef[i][k] = 1;
            }
            vet_coef[i][k] = i+k;
        }
    }
}

int main() {
    init_vet_coef();

    pthread_barrier_init(&barreira, NULL, NUM_THREADS);

    pthread_barrier_destroy(&barreira);

    return 0;
}