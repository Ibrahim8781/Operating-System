#include <stdio.h>
#include <pthread.h>

int balance = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        balance++;
        printf("Incrementing - ");
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* decrement(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
         printf("Decrementing - ");
        balance--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, decrement, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Final balance: %d\n", balance);
    return 0;
}