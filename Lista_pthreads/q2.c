/*
Para resolvermos a questão, iremos criar uma thread específica para cada arquivo que será lido.
Além disso, teremos funções específicas de leitura e escrita para fazer em cima de cada arquivo. 
precisamos ter mutex para as mesmas linhas não serem acessadas ao mesmo tempo ( NUM_CONSU ).
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_FILES 3 // quantidade de threads leitoras 
#define NUM_CONSU 6 // de 1 a 7 -> Permanece nessa quantidade, mas pode ser alterado

//declarando e inicializando o vetor de mutexes de forma global para que possamos manipular diretamete em cada struct de thread
pthread_mutex_t mutex_consultorio[NUM_CONSU];

void init_mutexes() {
    for ( int idx=0 ; idx < NUM_CONSU ; idx++)  pthread_mutex_init(&mutex_consultorio[idx], NULL); 
}

void init_consultorio() {

    FILE * f0 = fopen("pacientes_q2/pacientes_iniciais.txt", "w");

    if ( f0 == NULL) {
        printf("Erro ao criar Arquivo de pacientes\n");
        exit(1);
    }

    for (int i=0 ; i < NUM_CONSU; i++) {
        fprintf(f0,"LIVRE consultório %i\n",(i+1));
    }

    fclose(f0);
}

typedef struct thread_file {
    int id; // id da thread
    char *file; // declarando ponteiro pelo qual a thread irá ler o seu N arquivo
} thread_n;

int main() {
    init_mutexes();
    init_consultorio();

    return 0;
}