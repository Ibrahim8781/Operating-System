#include <stdio.h>
#include <pthread.h>
#include <math.h>

// Structure to pass data to threads
struct calc_data {
    double input;
    double* result;
};

void* calculate_square(void* arg) {
    struct calc_data* data = (struct calc_data*)arg;
    *(data->result) = data->input * data->input;
    printf("Square of %.2f = %.2f\n", data->input, *(data->result));
    return NULL;
}

void* calculate_sqrt(void* arg) {
    struct calc_data* data = (struct calc_data*)arg;
    *(data->result) = sqrt(data->input);
    printf("Square root of %.2f = %.2f\n", data->input, *(data->result));
    return NULL;
}

int main() {
    pthread_t square_thread, sqrt_thread;
    double input = 16.0;
    double square_result, sqrt_result;
    
    struct calc_data square_data = {input, &square_result};
    struct calc_data sqrt_data = {input, &sqrt_result};
    
    pthread_create(&square_thread, NULL, calculate_square, &square_data);
    pthread_create(&sqrt_thread, NULL, calculate_sqrt, &sqrt_data);
    
    pthread_join(square_thread, NULL);
    pthread_join(sqrt_thread, NULL);
    
    return 0;
}