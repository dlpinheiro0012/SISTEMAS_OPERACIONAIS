#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_INCOGNITAS 10
#define NUM_THREADS 4

int vet_X[NUM_INCOGNITAS] = {1};
int vet_coef[NUM_INCOGNITAS][NUM_INCOGNITAS]; 
int vet_result[NUM_INCOGNITAS] = {0};


double somatorio_jacobi(int ** vet_coeficientes, int * vet_x_past ) {
    double soma = 0;
    for ( int i = 0; i < NUM_INCOGNITAS; i++)  {
        for (int k = 0; k < NUM_INCOGNITAS; k ++) {
            if (i != k) {
                soma += vet_coeficientes[i][k] * vet_x_past[k]; // soma com pesos para os coeficietes e as incógnitas com valores da interação anterior
            }
        }
    }

    return soma;
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
    
    return 0;
}