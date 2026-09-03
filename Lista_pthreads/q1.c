#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//Valores que podem ser alterados
#define VET_SIZE 102//Tamanho fixo do vetor numérico
#define NUM_THREADS 11 //Quantidade de Threads que serão usadas


//Devemos separar o vetor com um escalonador que irá dividi-lo em tamanhos iguais e passará os subarrays para as threads
typedef struct {
    int id_thread;
    int posicao_inicial;
    int qtd_particao;
    int* vetor_numerico;
    int soma_parcial;
} _subarray;

//funcao que cada thread irá efetuar
void* sum(void* subarray) {
    _subarray *array = (_subarray*) subarray;
    int posicao_inicial = array->posicao_inicial;
    int qtd_particao = array->qtd_particao;

    for (int i = posicao_inicial; i < (qtd_particao + posicao_inicial); i++) {
        array->soma_parcial += array->vetor_numerico[i];
    }
    printf("Soma Parcial : %d\n",array->soma_parcial);

    pthread_exit((void*) array);
}

//Função que irá dividir o vetor numérico em subarrays e separá-los para que cada uma das threads possa efetuar a soma sob os valores selecionados
//basicamente irá retornar um vetor de escalonador que indica quantos termos terão na thread [i]
int* escalonador(int vetor_size, int n_threads) {
    int particao = vetor_size / n_threads; // particao de quantos termos teremos dentro de cada subarray
    int resto = vetor_size % n_threads; // quando tivermos uma divisão nao inteira, direcionamos os "resto" valores para as threads até acabar
    int i=0;

    int * escalonador = NULL; //inciando o ponteiro como nulo 
    escalonador = (int*)malloc(n_threads * sizeof(int)); //alocamos memória com a quantidade de threads

    if (!escalonador) {
        printf("Erro ao alocar memória para o escalonador");
        return NULL;
    }

    while ( i < n_threads ) {
        escalonador[i] = particao;

        if (resto > 0) {
            escalonador[i]++;
            resto--;
        }
        i++;
    }

    return escalonador;

}

//Funçao apenas para alimentar o vetor numérico com a quantidade que quiser
int* alimentarArray(int* vetor_numerico) {
    for (int k=0; k < VET_SIZE; k++) {
        vetor_numerico[k] = 1; //setando todos os termos como 1; o resultado final será o próprio tamanho do vetor ( VET_SIZE )
    }

    return vetor_numerico;
}

int main() {
    _subarray subarray[NUM_THREADS];
    pthread_t lista_threads[NUM_THREADS];

    int rc;
    int posicao_inicial_atual=0;
    int soma_total = 0;
    int* separador = NULL;
    int* vetor_numerico = NULL;

    separador = escalonador(VET_SIZE,NUM_THREADS);
    vetor_numerico = (int*)malloc(VET_SIZE * sizeof(int));

    if (!vetor_numerico || !separador) {
        printf("Erro ao inicializar ponteiros na main");
        return(-1);
    }


    vetor_numerico = alimentarArray(vetor_numerico);

    //agora, iremos alimentar cada um dos subarrays e criar as threads que executarão na função sum
    for (int i=0;i < NUM_THREADS; i++) {
        subarray[i].id_thread = i+1;
        subarray[i].posicao_inicial = posicao_inicial_atual;
        subarray[i].qtd_particao = separador[i];
        subarray[i].vetor_numerico = vetor_numerico;
        subarray[i].soma_parcial = 0;

        rc = pthread_create((&lista_threads[i]), NULL, sum, (void*) &subarray[i]);

        if (rc != 0) {
            printf("Erro de criacao de thread");
            exit(-1);
        }

        posicao_inicial_atual += separador[i];
    }

    //agora, efetuando a soma das partições parciais para a soma total final
    for (int i=0; i< NUM_THREADS; i++) {

        rc = pthread_join(lista_threads[i], NULL);
        if(rc != 0) {
            printf("Erro no join das threads");
            return(-1);
        }

        soma_total += subarray[i].soma_parcial;
    }

    free(vetor_numerico);
    free(separador);

    printf("Soma Total: %d\n", soma_total);

    return 0;
}