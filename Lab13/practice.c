/*
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
int counter = 0; // shared global variable

void *mythread(void *arg) {
    char *letter = arg;
    int i; // stack (private per thread)
    printf("%s: begin [addr of i: %p]\n", letter, &i);
    for (i = 0; i < max; i++) {
        counter = counter + 1; // shared: only one
    }
    printf("%s: done\n", letter);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: main-first <loopcount>\n");
        exit(1);
    }
    max = atoi(argv[1]);
    pthread_t p1, p2;
    printf("main: begin [counter = %d] [addr of counter: %p]\n", counter, (void*)&counter);
    pthread_create(&p1, NULL, mythread, "A");
    pthread_create(&p2, NULL, mythread, "B");
    // join waits for the threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    // pthread join (to wait thread finish but main do not and resources)
    printf("main: done\n [counter: %d]\n [should: %d]\n", counter, max * 2);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
pthread_mutex_t lock;
int counter = 0; // shared global variable

void *mythread(void *arg) {
    char *letter = arg;
    int i;
    printf("%s: starting\n", letter);  // Indicate the thread is starting
    for (i = 0; i < max; i++) {
        // Lock the mutex
        printf("%s: trying to lock the critical section\n", letter);
        pthread_mutex_lock(&lock);   // Lock the mutex before entering the critical section
        printf("%s: locked the critical section\n", letter);

        // Critical section: increment the shared counter
        counter = counter + 1;

        // Unlock the mutex
        printf("%s: releasing the lock\n", letter);
        pthread_mutex_unlock(&lock); // Unlock the mutex after leaving the critical section
    }
    printf("%s: done\n", letter);  // Indicate the thread is done
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: main-first <loopcount>\n");
        exit(1);
    }
    max = atoi(argv[1]);
    pthread_t p1, p2;
    printf("main: begin [counter = %d] [%p]\n", counter, (void*) &counter);
    
    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    pthread_create(&p1, NULL, mythread, "Thread A");
    pthread_create(&p2, NULL, mythread, "Thread B");

    // Join waits for the threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    // Print the final result
    printf("main: done\n [counter: %d]\n [should: %d]\n", counter, max * 2);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
*/

