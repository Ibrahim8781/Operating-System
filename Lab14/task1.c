#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

void* thread_func(void* args) {
    int thread_id = *(int*)args;
    
    sem_wait(&semaphore);
    printf("Hello from thread %d\n", thread_id);
    sem_post(&semaphore);
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int id1 = 1, id2 = 2;

    // Initialize semaphore with value 1 (binary semaphore)
    sem_init(&semaphore, 0, 1);

    // Create threads
    pthread_create(&thread1, NULL, thread_func, &id1);
    pthread_create(&thread2, NULL, thread_func, &id2);

    // Wait for threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Cleanup
    sem_destroy(&semaphore);

    return 0;
}