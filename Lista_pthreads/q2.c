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

void init_mutexes() {
    for ( int idx=1 ; idx <= NUM_CONSU ; idx++)  pthread_mutex_init(&mutex_consultorio[idx], NULL); 
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

int coletar_nome_consultorio(char * linha, char * nome, int * consultorio) {

    int validade = sscanf( linha, "%49s %i", nome, consultorio);

    return validade;
}

typedef struct thread_file {
    int id; // id da thread
    char *file; // declarando ponteiro pelo qual a thread irá ler o seu N arquivo
} thread_n;

void* executar_thread( void* thread ) {
    thread_n *data = (thread_n*) thread;

    FILE * f = fopen(data->file, "r");

    if(!f) {
        printf("Erro ao ler arquivo da thread");
        pthread_exit(NULL); //extinguindo a thread
    }

    char linha[100];
    char nome[50];
    int consultorio;

    while( fgets(linha, sizeof(linha), f) != NULL ) {

        int validade = coletar_nome_consultorio(linha,nome, &consultorio);

        if (validade != 2) {
            printf("Erro ao coletar nome / consultorio do paciente");
            pthread_exit(NULL);
        }

        int idx_linha = consultorio - 1;

        pthread_mutex_lock(&mutex_consultorio[idx_linha]); // Lock na linha do consultorio

        printf("\033[%i;1H\033[4%im\033[2K+Paciente %s  Consultório %i", consultorio, idx_linha, nome, consultorio);
        printf("\033[0m"); //Reset na cor de fundo

        fflush(stdout); // Função usada para que a biblioteca não guardar as modificações antes de mandar para a tela

        sleep(3); //Garantindo a espera por 3 segundos

        pthread_mutex_unlock(&mutex_consultorio[idx_linha]);

        idx_linha++;
    }

    fclose(f);
    pthread_exit(NULL);

}

int main() {
    init_mutexes();
    init_consultorio();

    return 0;
}