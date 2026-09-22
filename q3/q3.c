#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* 
    -Vai ter uma escritora e uma leitora
    -vai ter que usar um mutex para que a leitora e a escritora não acessem o mesmo arquivo ao mesmo tempo
    -a região crítica deve ser um array
    -laço infinito(while true)
    -um leitora não atrapalha a outra leitora, mas uma leitora atrapalha a escritora e vice-versa
    -porém, a thread escritora tem prioridade sobre uma escritora ou leitora

*/

#define N 5
#define M 5

int ne=0;
int nl=0;

pthread_mutex_t mutex;
pthread_cond_t escritores;
pthread_cond_t leitores;

int array_generico[20] = {0};

int gerador_num_aleatorio()
{
    return (rand() % 20);
}

void* leitura(void* arg)
{
    pthread_mutex_lock(&mutex);
    while(ne > 0) {
        pthread_cond_wait(&leitores, &mutex);
    }
    nl++;
    pthread_mutex_unlock(&mutex);

    int* lido_1 = malloc(sizeof(int));
    
    *lido_1 = array_generico[gerador_num_aleatorio()];

    pthread_mutex_lock(&mutex);

    nl--;

    if(nl == 0) {
        pthread_cond_signal(&escritores);
    }

    pthread_mutex_unlock(&mutex);

    return (void*) lido_1;

}

void* escrita(void* arg)
{
    pthread_mutex_lock(&mutex);

    while(nl > 0 || ne > 0) {
        pthread_cond_wait(&escritores, &mutex);
    } 
    ne++;

    pthread_mutex_unlock(&mutex);

    array_generico[gerador_num_aleatorio()] = gerador_num_aleatorio()+ 50;

    pthread_mutex_lock(&mutex);

    ne--;

    pthread_cond_signal(&escritores);
    pthread_cond_broadcast(&leitores);

    pthread_mutex_unlock(&mutex);

}




int main() {
    srand(time(NULL));

    int* lido;

    pthread_t array_threads[100], array_threads2[100];
    pthread_mutex_init(&mutex, NULL);

    int i;
    while(1)
    {
        for(i=0; i<N; i++)//ler
        {
            if(pthread_create(array_threads + i, NULL, &leitura, NULL)!=0)
            {
                return 1;
            }
        }
        for(i=0; i<M; i++)//escrever
        {
            if(pthread_create(array_threads2 + i, NULL, &escrita, NULL)!=0)
            {
                return 1;
            }
        }
        for(i=0; i<N; i++)//fim ler
        {
            if(pthread_join(array_threads[i], (void**) &lido)!=0)
            {
                return 1;
            }

            printf("\nLido: %d", *lido);
            free(lido);
        }
        for(i=0; i<M; i++)//fim escrever
        {
            if(pthread_join(array_threads2[i], NULL)!=0)
            {
                return 1;
            }
        }

        printf("\nArray Atualmente: ");
        for(i=0; i<20; i++) 
        {
            printf("%d ", array_generico[i]);
        }
    }   
    
    pthread_mutex_destroy(&mutex);
    return 0;
}