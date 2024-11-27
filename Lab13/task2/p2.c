#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t account1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t account2_mutex = PTHREAD_MUTEX_INITIALIZER;

int account1_balance = 1000;
int account2_balance = 1000;

void* transfer(void* arg) {
    int direction = *(int*)arg;
    pthread_mutex_t *first_mutex, *second_mutex;
    int *from_balance, *to_balance;
    
    if (direction == 1) {
        first_mutex = &account1_mutex;
        second_mutex = &account2_mutex;
        from_balance = &account1_balance;
        to_balance = &account2_balance;
        printf("Transferring from account 1 to 2\n");
    } else {
        first_mutex = &account2_mutex;
        second_mutex = &account1_mutex;
        from_balance = &account2_balance;
        to_balance = &account1_balance;
        printf("Transferring from account 2 to 1\n");
    }

    pthread_mutex_lock(first_mutex);
    printf("Thread %d: Locked first account\n", direction);
    sleep(1);  // Simulate some work

    pthread_mutex_lock(second_mutex);
    printf("Thread %d: Locked second account\n", direction);

    int amount = rand() % 100 + 1;  // Transfer a random amount between 1 and 100
    if (*from_balance >= amount) {
        *from_balance -= amount;
        *to_balance += amount;
        printf("Thread %d: Transferred %d\n", direction, amount);
    } else {
        printf("Thread %d: Insufficient funds\n", direction);
    }

    pthread_mutex_unlock(second_mutex);
    pthread_mutex_unlock(first_mutex);
    printf("Thread %d: Transfer completed\n", direction);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int dir1 = 1, dir2 = 2;
    srand(time(NULL));  // Initialize random seed

    printf("Initial balances: Account 1 = %d, Account 2 = %d\n", account1_balance, account2_balance);

    pthread_create(&t1, NULL, transfer, &dir1);
    pthread_create(&t2, NULL, transfer, &dir2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final balances: Account 1 = %d, Account 2 = %d\n", account1_balance, account2_balance);
    printf("All transfers completed\n");
    return 0;
}