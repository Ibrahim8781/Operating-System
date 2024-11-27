#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>  // Add this line to include the sleep function

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

int try_lock_with_timeout(pthread_mutex_t* mutex, int timeout_ms) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout_ms / 1000;
    ts.tv_nsec += (timeout_ms % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }
    return pthread_mutex_timedlock(mutex, &ts);
}

void* thread1_func(void* arg) {
    printf("Thread 1: Attempting to lock mutex1\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: Locked mutex1\n");
    sleep(1);
    printf("Thread 1: Attempting to lock mutex2\n");
    if (try_lock_with_timeout(&mutex2, 2000) == 0) {
        printf("Thread 1: Locked mutex2\n");
        pthread_mutex_unlock(&mutex2);
    } else {
        printf("Thread 1: Failed to lock mutex2, releasing mutex1\n");
    }
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

void* thread2_func(void* arg) {
    printf("Thread 2: Attempting to lock mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: Locked mutex2\n");
    sleep(1);
    printf("Thread 2: Attempting to lock mutex1\n");
    if (try_lock_with_timeout(&mutex1, 2000) == 0) {
        printf("Thread 2: Locked mutex1\n");
        pthread_mutex_unlock(&mutex1);
    } else {
        printf("Thread 2: Failed to lock mutex1, releasing mutex2\n");
    }
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Program completed\n");
    return 0;
}