#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t recurso1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recurso2 = PTHREAD_MUTEX_INITIALIZER;

void* proceso1(void* arg) {
    while(1) {
        pthread_mutex_lock(&recurso1);
        printf("Proceso 1 ha adquirido el recurso 1\n");
        sleep(1);
        pthread_mutex_lock(&recurso2);
        printf("Proceso 1 ha adquirido el recurso 2\n");
        pthread_mutex_unlock(&recurso2);
        pthread_mutex_unlock(&recurso1);
        sleep(1);
    }
    return NULL;
}

void* proceso2(void* arg) {
    while(1) {
        pthread_mutex_lock(&recurso2);
        printf("Proceso 2 ha adquirido el recurso 2\n");
        sleep(1);
        pthread_mutex_lock(&recurso1);
        printf("Proceso 2 ha adquirido el recurso 1\n");
        pthread_mutex_unlock(&recurso1);
        pthread_mutex_unlock(&recurso2);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, proceso1, NULL);
    pthread_create(&t2, NULL, proceso2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
