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

pthread_mutex_t mutex;
int array_mexidon[20] = {0};

int numero_loko()
{
    return (rand() % 20) + 1;
}

void* leitura(void* arg)
{
    array_mexidon[numero_loko()] = numero_loko()+ 50;
    printf("Lendo\n");

}

void* escrita(void* arg)
{
    pthread_mutex_lock(&mutex);

    array_mexidon[numero_loko()] = numero_loko()+ 50;
    printf("Escrevendo\n");

    pthread_mutex_unlock(&mutex);
}



int main() {
    srand(time(NULL));

    pthread_t array_threads[100], array_threads2[100];
    pthread_mutex_init(&mutex, NULL);

    int i;
    while(1)
    {
        for(i=0; i<N; i++)
        {
            if(pthread_create(array_threads + i, NULL, &leitura, NULL)!=0)
            {
                return 1;
            }
        }
        for(i=0; i<M; i++)
        {
            if(pthread_create(array_threads2 + i, NULL, &escrita, NULL)!=0)
            {
                return 1;
            }
        }
        for(i=0; i<N; i++)
        {
            if(pthread_join(array_threads[i], NULL)!=0)
            {
                return 1;
            }
        }
        for(i=0; i<M; i++)
        {
            if(pthread_join(array_threads2[i], NULL)!=0)
            {
                return 1;
            }
        }
        pthread_mutex_destroy(&mutex);
    }   
    return 0;
}