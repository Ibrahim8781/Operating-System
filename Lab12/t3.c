#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    pthread_t tid = pthread_self();
    printf("Thread ID: %lu\n", (unsigned long)tid);
    
    size_t stack_size = *(size_t*)arg;
    printf("Requested stack size: %zu bytes\n", stack_size);
    
    sleep(1); // Simulate some work
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_attr_t attr;
    
    // Initialize thread attributes
    pthread_attr_init(&attr);
    
    // Set custom stack size (2MB)
    size_t stack_size = 2 * 1024 * 1024;
    pthread_attr_setstacksize(&attr, stack_size);
    
    // Create thread with custom attributes
    pthread_create(&thread, &attr, thread_function, &stack_size);
    
    // Wait for thread to complete
    pthread_join(thread, NULL);
    
    // Cleanup
    pthread_attr_destroy(&attr);
    
    printf("Main thread completed\n");
    return 0;
}