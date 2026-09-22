#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 10000
#define N 10


char *mensagens[]={"Mensagem 1","Mensagem 2","Mensagem 3"};




typedef struct{

    const char *funcao;
    int arg;

} Tarefas;

typedef struct{

    long contador;
    bool livre;
    Tarefas tarefa;
    int indice;
    

} Dados;


typedef struct{

    Tarefas tasks[MAX_SIZE];
    int front;
    int rear;

} Queue;



void start(Queue *q){

    q->front=-1;
    q->rear=0;
}


void push(Queue *q,Tarefas *tarefa){

    q->tasks[q->rear]=*tarefa;
    q->rear++;
}

void pop(Queue *q){

    q->front++;
}

Tarefas front(Queue *q){

    return q->tasks[q->front+1];

}

bool empty(Queue *q){

    if((q->rear-q->front)==1){
        return true;
    }

    return false;
}

Dados dados_threads[N];
Queue q;

pthread_t threads[N];
pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_vagas = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vaga_disponivel = PTHREAD_COND_INITIALIZER;


void *message(void *argument){


    Dados *data=argument;
    Tarefas *tsk=&data->tarefa;

    char *msg=mensagens[tsk->arg];
    int idx=data->indice;

    printf("Posicao %d imprimiu %s\n",idx,msg);

    return NULL;
}

void *increase(void *argument){

    Dados *data=argument;
    Tarefas *tsk = &data->tarefa;

    data->contador+=tsk->arg;

    return NULL;
}


void *f(void *argument){

    Dados *data=argument;
    Tarefas *tsk=&data->tarefa;

    if(!strcmp(tsk->funcao,"message")){

        message(data);
    }

    if(!strcmp(tsk->funcao,"increase")){

        increase(data);
    }

    pthread_mutex_lock(&mutex_vagas);

    data->livre=true;

    pthread_cond_signal(&vaga_disponivel);
    pthread_mutex_unlock(&mutex_vagas);

    return NULL;
}


void *funcao_escalonador(void *arg){


    while(!empty(&q)){

        pthread_mutex_lock(&mutex_fila);

        Tarefas tarefa_atual=front(&q);
        pop(&q);

        pthread_mutex_unlock(&mutex_fila);


        int index_thread=-1;

        pthread_mutex_lock(&mutex_vagas);

        while(index_thread==-1){

            for(int i=0;i<N;i++){

                if(dados_threads[i].livre){

                    index_thread=i;
                    dados_threads[i].livre=false;
                    break;

                }
            }

            if(index_thread==-1){
                pthread_cond_wait(&vaga_disponivel,&mutex_vagas);
            }
        }

        dados_threads[index_thread].tarefa=tarefa_atual;

        pthread_create(&threads[index_thread],NULL,f,&dados_threads[index_thread]);
        pthread_detach(threads[index_thread]); 

        pthread_mutex_unlock(&mutex_vagas);

    }

    return NULL;
}



int main(){


    srand((unsigned)time(NULL));

    start(&q);

    for(int i=0;i<N;i++){

        dados_threads[i].contador=0;
        dados_threads[i].livre=true;
        dados_threads[i].indice=i+1;

    }

    pthread_t escalonador;


    int it=20; //quantidade de iterações a serem executadas


    //gerador aleatório para ficar mais fácil de testar o programa em diferentes situações
    //a variável type define se a tarefa vai ser do tipo "message" ou "increase"

    for(int i=0;i<it;i++){

        pthread_mutex_lock(&mutex_fila);

        int type=(rand())%100;

        if(type%2==0){

            int idx=(rand())%3;

            Tarefas tarefa;
            tarefa.funcao="message";
            tarefa.arg=idx;

            push(&q,&tarefa);

        }
        else{

            int val=(rand())%5;

            Tarefas tarefa;
            tarefa.funcao="increase";
            tarefa.arg=val;

            push(&q,&tarefa);
        }

        pthread_mutex_unlock(&mutex_fila);

    }


    pthread_create(&escalonador, NULL, funcao_escalonador, NULL);
    pthread_join(escalonador, NULL);


    pthread_mutex_lock(&mutex_vagas);

    while(true){

        bool todas_livres=true;

        for(int i=0;i<N;i++){

            if(!dados_threads[i].livre){
                todas_livres=false;
            }
        }

        if(todas_livres){
            break;
        }

        pthread_cond_wait(&vaga_disponivel, &mutex_vagas);

    }

    pthread_mutex_unlock(&mutex_vagas);

    for(int i=0;i<N;i++){

        printf("Contador da posicao %d vale %ld\n",i+1,dados_threads[i].contador);
    }

}