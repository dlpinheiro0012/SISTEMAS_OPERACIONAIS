#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_INCOGNITAS 10
#define NUM_THREADS 3

int vet_X[NUM_INCOGNITAS];
int vet_coef[NUM_INCOGNITAS][NUM_INCOGNITAS];


double somatorio_jacobi(int ** vet_coeficientes, int * vet_incognitas ) {
    double soma = 0;
    for ( int i = 0; i < NUM_INCOGNITAS; i++)  {
        for (int k = 0; k < NUM_INCOGNITAS; k ++) {
            if (i != k) {
                soma += vet_coeficientes[i][k] * vet_incognitas[k]; // soma com pesos para os coeficietes e as incógnitas com valores da interação anterior
            }
        }
    }

    return soma;
}

void init_vet_incognitas() { // começamos cada incónita X como 1
    for( int i = 0 ; i < NUM_INCOGNITAS; i++) {
        vet_X[i] = 1;
    }    
}


int main() {
    init_vet_incognitas();
    
    return 0;
}