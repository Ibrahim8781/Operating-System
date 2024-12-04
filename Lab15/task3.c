#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTS 5
#define MAX_CONCURRENT_REQUESTS 3

pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t server_cv = PTHREAD_COND_INITIALIZER;
int active_requests = 0;

void* client_request(void* arg) {
    int client_id = *(int*)arg;

    pthread_mutex_lock(&server_mutex);
    printf("Client %d: Requesting service...\n", client_id);

    while (active_requests >= MAX_CONCURRENT_REQUESTS) {
        printf("Client %d: Server is busy, waiting...\n", client_id);
        pthread_cond_wait(&server_cv, &server_mutex);
    }

    active_requests++;
    printf("Client %d: Being served by the server...\n", client_id);
    pthread_mutex_unlock(&server_mutex);

    sleep(2);  // Simulate service time

    pthread_mutex_lock(&server_mutex);
    printf("Client %d: Service completed, leaving server.\n", client_id);
    active_requests--;
    pthread_cond_signal(&server_cv);
    pthread_mutex_unlock(&server_mutex);

    return NULL;
}

int main() {
    pthread_t clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_request, &client_ids[i]);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    pthread_mutex_destroy(&server_mutex);
    pthread_cond_destroy(&server_cv);

    return 0;
}