#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_CHILDREN 5
#define BUFFER_SIZE 1024

void create_chain_processes() {
    int pipes[NUM_CHILDREN + 1][2];
    pid_t pids[NUM_CHILDREN + 1];
    char buffer[BUFFER_SIZE];

    // Create pipes for communication between processes
    for (int i = 0; i <= NUM_CHILDREN; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    // Create child processes
    for (int i = 0; i <= NUM_CHILDREN; ++i) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pids[i] == 0) {  // Child process
            // Set up pipe redirections
            if (i > 0) {
                close(pipes[i - 1][1]);  // Close write end of previous pipe
            }
            if (i < NUM_CHILDREN) {
                close(pipes[i][0]);      // Close read end of current pipe
            }

            while (1) {
                if (i > 0) {
                    // Read from the previous process
                    read(pipes[i - 1][0], buffer, BUFFER_SIZE);
                    if (strcmp(buffer, "Quit") == 0) {
                        exit(0);  // Exit if "Quit" is received
                    }
                } else {
                    // First process prompts for input
                    printf("Enter message (or 'Quit' to exit): ");
                    fgets(buffer, BUFFER_SIZE, stdin);
                    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline

                    if (strcmp(buffer, "Quit") == 0) {
                        for (int j = 0; j <= NUM_CHILDREN; ++j) {
                            write(pipes[j][1], "Quit", strlen("Quit") + 1);
                            close(pipes[j][1]);
                        }
                        exit(0);
                    }
                }

                // Append PID to the message, reserving space
                char updated_message[BUFFER_SIZE];
                snprintf(updated_message, BUFFER_SIZE, "%.*s:%d", BUFFER_SIZE - 20, buffer, getpid());
                printf("%s\n", updated_message);

                // Send the message to the next process
                if (i < NUM_CHILDREN) {
                    write(pipes[i][1], updated_message, strlen(updated_message) + 1);
                } else {
                    // If it's the last process, print final message and break
                    printf("%s\n", updated_message);
                }

                // First process breaks after sending the message
                if (i == 0) break;
            }
            exit(0);
        }
    }

    // Close pipes in parent and wait for child processes to finish
    for (int i = 0; i <= NUM_CHILDREN; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i <= NUM_CHILDREN; ++i) {
        waitpid(pids[i], NULL, 0);
    }
}

int main() {
    create_chain_processes();
    return 0;
}
