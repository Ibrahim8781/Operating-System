#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;
int total_sum = 0;

typedef struct {
    int start;
    int end;
    int* array;
} ThreadArgs;

void* thread_sum(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    int local_sum = 0;

    // Calculate local sum
    for (int i = thread_args->start; i <= thread_args->end; i++) {
        local_sum += thread_args->array[i];
    }

    // Safely update total sum
    sem_wait(&semaphore);
    total_sum += local_sum;
    sem_post(&semaphore);

    return NULL;
}

int main() {
    pthread_t threads[3];
    int array[15] = {3, 7, 1, 10, 5, 6, 2, 4, 8, 3, 9, 1, 7, 3, 5};
    ThreadArgs thread_args[3] = {
        {0, 4, array},
        {5, 9, array},
        {10, 14, array}
    };

    // Initialize semaphore
    sem_init(&semaphore, 0, 1);

    // Create threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_sum, &thread_args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print total sum
    printf("Sum of the array: %d\n", total_sum);

    // Cleanup
    sem_destroy(&semaphore);

    return 0;
}