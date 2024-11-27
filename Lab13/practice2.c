#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Declare global variables
pthread_t tid[2];          // Array to hold thread IDs for 2 threads
int counter;               // Shared counter variable
pthread_mutex_t lock;      // Mutex to protect shared resource

// Function to be executed by threads
void* trythis(void* arg)
{
    pthread_mutex_lock(&lock);  // Lock the mutex to protect shared resource (counter)
    
    unsigned long i = 0;
    counter += 1;  // Increment the shared counter
    printf("\n Job %d has started\n", counter);  // Print which job started

    // Simulate work by busy-waiting
    for (i = 0; i < (0xFFFFFFFF); i++)
        ;

    printf("\n Job %d has finished\n", counter);  // Print which job finished
    pthread_mutex_unlock(&lock);  // Unlock the mutex to allow the other thread to access the counter

    return NULL;
}

int main(void)
{
    int i = 0;
    int error;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    // Create two threads
    while (i < 2) {
        error = pthread_create(&(tid[i]), NULL, &trythis, NULL);  // Create each thread
        if (error != 0)
            printf("\nThread can't be created :[%s]", strerror(error));  // Print error if thread creation fails
        i++;
    }

    // Wait for both threads to finish
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    // Destroy the mutex after usage
    pthread_mutex_destroy(&lock);

    return 0;
}
