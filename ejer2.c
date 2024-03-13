#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define VECES_QUE_COMEN 10

sem_t tenedores[NUM_FILOSOFOS];
int contador_comidas[NUM_FILOSOFOS];

void* filosofo(void* num) {
    int* i = (int*)num;
    while (contador_comidas[*i] < VECES_QUE_COMEN) {
        printf("Filósofo %d está pensando.\n", *i);
        sleep(1);

        sem_wait(&tenedores[*i]);
        sem_wait(&tenedores[(*i + 1) % NUM_FILOSOFOS]);

        contador_comidas[*i]++;
        printf("Filósofo %d está comiendo por %dª vez.\n", *i, contador_comidas[*i]);
        sleep(1);

        sem_post(&tenedores[*i]);
        sem_post(&tenedores[(*i + 1) % NUM_FILOSOFOS]);

        sleep(1); // Añadido para dar tiempo de pensar
    }
    printf("Filósofo %d ha terminado de comer.\n", *i);
    return NULL;
}

int main() {
    int i;
    pthread_t thread[NUM_FILOSOFOS];
    int filosofos[NUM_FILOSOFOS];

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&tenedores[i], 0, 1);
        contador_comidas[i] = 0;
    }

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        filosofos[i] = i;
        pthread_create(&thread[i], NULL, filosofo, (void*)&filosofos[i]);
    }

    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("Todos los filósofos han terminado de comer.\n");

    return 0;
}
