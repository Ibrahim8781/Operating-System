#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define TOTAL_PARKING_SPOTS 5
#define TOTAL_CARS 8

sem_t parking_spots;

void* car_parking(void* arg) {
    int car_id = *(int*)arg;

    sem_wait(&parking_spots);
    printf("Car %d is parking...\n", car_id);
    sleep(3);  // Simulate parking time
    printf("Car %d is leaving the parking lot.\n", car_id);
    sem_post(&parking_spots);

    return NULL;
}

int main() {
    pthread_t cars[TOTAL_CARS];
    int car_ids[TOTAL_CARS];

    sem_init(&parking_spots, 0, TOTAL_PARKING_SPOTS);

    for (int i = 0; i < TOTAL_CARS; i++) {
        car_ids[i] = i + 1;
        pthread_create(&cars[i], NULL, car_parking, &car_ids[i]);
    }

    for (int i = 0; i < TOTAL_CARS; i++) {
        pthread_join(cars[i], NULL);
    }

    sem_destroy(&parking_spots);
    return 0;
}