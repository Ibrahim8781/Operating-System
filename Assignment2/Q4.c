#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

void employee_process(int id, int read_pipe, int write_pipe) {
    const int MAX_TIME_LEN = 20;
    char suggested_time[MAX_TIME_LEN];
    char response[4];

    // Read suggested time from owner
    read(read_pipe, suggested_time, sizeof(suggested_time));

    // Simulate employee decision (80% chance of agreeing)
    srand(time(NULL) ^ (id << 16));
    if (rand() % 100 < 80) {
        strcpy(response, "Yes");
    } else {
        strcpy(response, "No");
    }

    printf("Employee %d response: %s\n", id, response);

    // Send response to owner
    write(write_pipe, response, sizeof(response));

    exit(0);
}

int main() {
    const int NUM_EMPLOYEES = 10;


    int pipes[NUM_EMPLOYEES][2];
    pid_t pids[NUM_EMPLOYEES];
    char suggested_time[MAX_TIME_LEN];
    char response[4];
    int all_agreed;

    // Create pipes for each employee
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Get suggested time from user
    printf("Enter suggested meeting time: ");
    fgets(suggested_time, sizeof(suggested_time), stdin);
    suggested_time[strcspn(suggested_time, "\n")] = 0; // Remove newline

    // Create employee processes
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            exit(1);
        } else if (pids[i] == 0) {
            // Child process (employee)
            close(pipes[i][WRITE_END]);
            employee_process(i + 1, pipes[i][READ_END], pipes[i][WRITE_END]);
        } else {
            // Parent process (owner)
            close(pipes[i][READ_END]);
            write(pipes[i][WRITE_END], suggested_time, sizeof(suggested_time));
        }
    }

    // Parent process (owner) waits for responses
    all_agreed = 1;
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        read(pipes[i][READ_END], response, sizeof(response));
        if (strcmp(response, "No") == 0) {
            all_agreed = 0;
        }
        close(pipes[i][WRITE_END]);
        waitpid(pids[i], NULL, 0);
    }

    // Print final result
    if (all_agreed) {
        printf("Meeting scheduled for %s. All employees agreed.\n", suggested_time);
    } else {
        printf("Meeting could not be scheduled. Not all employees agreed.\n");
    }

    return 0;
}