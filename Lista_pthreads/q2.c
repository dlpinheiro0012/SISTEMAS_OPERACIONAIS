/*
Para resolvermos a questão, iremos criar uma thread específica para cada arquivo que será lido.
Além disso, teremos funções específicas de leitura e escrita para fazer em cima de cada arquivo. 
precisamos ter mutex para as mesmas linhas não serem acessadas ao mesmo tempo ( NUM_CONSU ).
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_FILES 3 // quantidade de threads leitoras 
#define NUM_CONSU 6 // de 1 a 7 -> Permanece nessa quantidade, mas pode ser alterado

//declarando e inicializando vetores de forma global 
pthread_mutex_t mutex_consultorio[NUM_CONSU];
pthread_cond_t _vetor_cond[NUM_CONSU];
int termino_linha = 0;

void init_mutexes() {
    for ( int idx = 0 ; idx < NUM_CONSU ; idx++) {
        pthread_mutex_init(&mutex_consultorio[idx], NULL); 
        pthread_cond_init(&_vetor_cond[idx], NULL);
    } 
}

void destroy_mutex() {
    for ( int idx = 0; idx < NUM_CONSU ; idx++) {
        pthread_mutex_destroy(&mutex_consultorio[idx]);
        pthread_mutex_destroy(&_vetor_cond[idx]);
    }
}

void create_consultorios() { // NAO ESTA FUNCIONANDO
    for (int i=0; i <6;i++) {
        int cor = i+1;
        printf("\033[%d;1H\033[4%dm Paciente LIVRE  Consultório %i", cor, i);
    }
}

typedef struct thread_file {
    int id; // id da thread
    char *file; // declarando ponteiro pelo qual a thread irá ler o seu arquivo específico
} rotina;

void* executar_thread( void* thread ) {
    rotina *data = (rotina*) thread;

    FILE * f = fopen(data->file, "r");

    if(!f) {
        printf("Erro ao ler arquivo da thread");
        pthread_exit(NULL); //extinguindo a thread
    }

    char linha[100];
    char nome[50];
    int consultorio;

    while( fgets(linha, sizeof(linha), f) != NULL ) {

        int validade = sscanf( linha, "%49s %i", nome, &consultorio);

        if (validade != 2) {
            printf("Erro ao coletar nome / consultorio do paciente");
            pthread_exit(NULL);
        }

        int idx_linha = consultorio - 1;

        pthread_mutex_lock(&mutex_consultorio[idx_linha]); // Lock na linha do consultorio

        if (!termino_linha) { //VERIFICAR AQUI O SINAL PARA OS MUTEXES
            pthread_cond_wait(&_vetor_cond[idx_linha],&mutex_consultorio[idx_linha]); //sinal de aguardo para o mutex da linha

            printf("\033[%i;1H\033[4%im\033[2K+Paciente %s  Consultório %i", consultorio, idx_linha, nome, consultorio); // NAO ESTA FUNCIONANDO
            printf("\033[0m"); //Reset na cor de fundo

            fflush(stdout); // Função usada para que a biblioteca não guarde as modificações antes de mandar para a tela

            sleep(3); //Garantindo a espera por 3 segundos
            
            termino_linha = 1;
        }

        pthread_mutex_unlock(&mutex_consultorio[idx_linha]);
    
        idx_linha++;
    }

    fclose(f);
    pthread_exit(NULL);

}

int main() {
    create_consultorios();
    return 0;
}