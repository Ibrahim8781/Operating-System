#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>

sem_t semaphore;
int global_min = INT_MAX;

typedef struct {
    int start;
    int end;
    int* array;
} ThreadArgs;

void* find_min(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    int local_min = thread_args->array[thread_args->start];

    // Find local minimum
    for (int i = thread_args->start; i <= thread_args->end; i++) {
        if (thread_args->array[i] < local_min) {
            local_min = thread_args->array[i];
        }
    }

    // Safely update global minimum
    sem_wait(&semaphore);
    if (local_min < global_min) {
        global_min = local_min;
    }
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
        pthread_create(&threads[i], NULL, find_min, &thread_args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print minimum value
    printf("Minimum value in the array: %d\n", global_min);

    // Cleanup
    sem_destroy(&semaphore);

    return 0;
}