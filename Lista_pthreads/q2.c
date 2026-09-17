/*
A questão consiste em: separarmos uma thread para cada um dos arquivos de texto
Gerarmos um vetor de mutexes global para cada um dos consultórios

Assumimos que cada arquivo terá a mesma quantidade de linhas
tanto a quantidade de linhas quanto o numero de arquivos estão fixos ( podem ser trocados em NUM_FILES e NUM_CONSU, respectivamente)
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_FILES 3 // quantidade de threads leitoras 
#define NUM_CONSU 6 // de 1 a 7 -> 

//declarando e inicializando vetores de forma global 
pthread_mutex_t mutex_consultorio[NUM_CONSU];

void init_mutexes() {
    for ( int idx = 0 ; idx < NUM_CONSU ; idx++) {
        pthread_mutex_init(&mutex_consultorio[idx], NULL); 
    } 
}

void destroy_mutex() {
    for ( int idx = 0; idx < NUM_CONSU ; idx++) {
        pthread_mutex_destroy(&mutex_consultorio[idx]);
    }
}

void create_consultorios() { 
    int cor, consultorio;
    printf("\033[2J\033[?25l"); // Limpa a tela completamente antes de escrever e retira o cursor
    for (int i=0; i <= NUM_CONSU ;i++) {
        cor = i+1;
        consultorio = cor; // sao indexados da mesma forma
        printf("\033[%d;1H\033[4%dm LIVRE Consultorio %i", cor, i, consultorio);
    }
    printf("\033[0m"); //Reset na cor de fundo
}

typedef struct thread_file {
    int id; // id da thread
    char file[10]; // nome do arquivo único para a thread -> o arquivo txt estará no mesmo diretório do arquivo .c
} rotina;

void* executar_thread( void* thread ) {
    rotina *data = (rotina*) thread;

    FILE * f = fopen(data->file, "r");

    if(f == NULL) {
        printf("Erro ao ler arquivo da thread\n");
        pthread_exit(NULL); //extinguindo a thread
    }

    char linha[100];
    char nome[50];
    int consultorio;

    while( fgets(linha, sizeof(linha), f) != NULL ) {

        int validade = sscanf( linha, "%49s %i", nome, &consultorio);

        if (validade != 2) {
            printf("Erro ao coletar nome / consultorio do paciente\n");
            pthread_exit(NULL);
        }

        int idx_linha = consultorio - 1;

        pthread_mutex_lock(&mutex_consultorio[idx_linha]); // Lock na linha do consultorio

        printf("\033[%i;1H\033[4%im\033[2K Paciente %s Consultorio %i", consultorio, idx_linha, nome, consultorio); // 
        printf("\033[0m"); //Reset na cor de fundo

        sleep(3); //Garantindo a espera por 3 segundos

        pthread_mutex_unlock(&mutex_consultorio[idx_linha]); // dá unlock na linha

    }

    fclose(f);
    pthread_exit(NULL);

}

int main() {
    create_consultorios();
    init_mutexes();
    pthread_t vetor_threads[NUM_FILES];
    rotina funcao_thread[NUM_FILES];

    for (int i =0 ; i < NUM_FILES; i++) { //laço de criação de threds

        sprintf(funcao_thread[i].file, "p%d.txt", (i+1));
        funcao_thread[i].id = i+1;
        int rc = pthread_create(&vetor_threads[i], NULL, executar_thread, &funcao_thread[i]);

        if(rc) {
            printf("Erro ao criar a thread na Main\n");
            pthread_exit(NULL);
        }
    }

    for (int k =0 ; k < NUM_FILES; k++) {

        int rc = pthread_join(vetor_threads[k], NULL);
        if(rc) {
            printf("Erro no join das threads\n");
            pthread_exit(NULL);
        }
    }
    printf("\033[?25h\033[49m\033[%d;1H", NUM_CONSU+2); // Retorna o cursor, restaura a cor do terminal e coloca o cursor na linha após o print
    destroy_mutex();
    return 0;
}