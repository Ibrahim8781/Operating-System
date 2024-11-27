#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t account1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t account2_mutex = PTHREAD_MUTEX_INITIALIZER;

void* transfer1to2(void* arg) {
    pthread_mutex_lock(&account1_mutex);
    printf("Thread 1: Locked account 1\n");
    sleep(1);  // Simulate some work
    pthread_mutex_lock(&account2_mutex);
    printf("Thread 1: Locked account 2\n");
    pthread_mutex_unlock(&account2_mutex);
    pthread_mutex_unlock(&account1_mutex);
    return NULL;
}

void* transfer2to1(void* arg) {
    pthread_mutex_lock(&account2_mutex);
    printf("Thread 2: Locked account 2\n");
    sleep(1);  // Simulate some work
    pthread_mutex_lock(&account1_mutex);
    printf("Thread 2: Locked account 1\n");
    pthread_mutex_unlock(&account1_mutex);
    pthread_mutex_unlock(&account2_mutex);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, transfer1to2, NULL);
    pthread_create(&t2, NULL, transfer2to1, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Transfers completed\n");
    return 0;
}