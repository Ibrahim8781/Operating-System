#include <stdio.h>
#include <pthread.h>

// Global variables for input and results
double num1 = 10.0, num2 = 5.0;
double add_result, sub_result, mul_result, div_result;

// Thread functions for each operation
void* add_numbers(void* arg) {
    add_result = num1 + num2;
    printf("Addition: %.2f + %.2f = %.2f\n", num1, num2, add_result);
    return NULL;
}

void* subtract_numbers(void* arg) {
    sub_result = num1 - num2;
    printf("Subtraction: %.2f - %.2f = %.2f\n", num1, num2, sub_result);
    return NULL;
}

void* multiply_numbers(void* arg) {
    mul_result = num1 * num2;
    printf("Multiplication: %.2f * %.2f = %.2f\n", num1, num2, mul_result);
    return NULL;
}

void* divide_numbers(void* arg) {
    if (num2 != 0) {
        div_result = num1 / num2;
        printf("Division: %.2f / %.2f = %.2f\n", num1, num2, div_result);
    } else {
        printf("Error: Division by zero!\n");
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3, thread4;
    
    // Create threads for each operation
    pthread_create(&thread1, NULL, add_numbers, NULL);
    pthread_create(&thread2, NULL, subtract_numbers, NULL);
    pthread_create(&thread3, NULL, multiply_numbers, NULL);
    pthread_create(&thread4, NULL, divide_numbers, NULL);
    
    // Wait for all threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    
    return 0;
}
