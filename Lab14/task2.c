#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t semaphore;
int product = 0;

void* producer(void* args) {
    sem_wait(&semaphore);
    
    product = rand() % 100;  // Generate random product
    printf("Producer generated: %d\n", product);
    
    sem_post(&semaphore);
    return NULL;
}

void* consumer(void* args) {
    sem_wait(&semaphore);
    
    printf("Consumer received: %d\n", product);
    
    sem_post(&semaphore);
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphore with value 1
    sem_init(&semaphore, 0, 1);

    // Create threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Cleanup
    sem_destroy(&semaphore);

    return 0;
}