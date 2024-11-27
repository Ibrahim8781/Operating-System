// process_b.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

#define PIPE_A_TO_B "/tmp/pipe_a_to_b"
#define PIPE_B_TO_A "/tmp/pipe_b_to_a"
#define MAX_MSG_SIZE 256

// Global variables for pipe file descriptors
int fd_read, fd_write;
pid_t process_a_pid = 0;

// Function to process the message (convert to uppercase)
void process_message(char* message) {
    for (int i = 0; message[i]; i++) {
        message[i] = toupper(message[i]);
    }
}

int main() {
    char buffer[MAX_MSG_SIZE];
    
    // Get Process A's PID from command line
    printf("Process B: Started (PID: %d)\n", getpid());
    printf("Enter Process A's PID: ");
    scanf("%d", &process_a_pid);
    
    printf("Process B: Opening pipes...\n");
    // Open pipes
    fd_read = open(PIPE_A_TO_B, O_RDONLY);
    if (fd_read == -1) {
        perror("Process B: Error opening read pipe");
        exit(1);
    }
    
    fd_write = open(PIPE_B_TO_A, O_WRONLY);
    if (fd_write == -1) {
        perror("Process B: Error opening write pipe");
        close(fd_read);
        exit(1);
    }

    printf("Process B: Ready to receive messages...\n");

    while (1) {
        // Read message from Process A
        ssize_t bytes_read = read(fd_read, buffer, MAX_MSG_SIZE);
        if (bytes_read > 0) {
            if (strcmp(buffer, "END") == 0) {
                // Send back END message
                write(fd_write, "END", 4);
                break;
            }

            printf("Process B: Received message: %s\n", buffer);

            // Process the message
            process_message(buffer);
            printf("Process B: Processed message: %s\n", buffer);

            // Write processed message back
            write(fd_write, buffer, strlen(buffer) + 1);

            // Send signal to Process A
            kill(process_a_pid, SIGUSR1);
            printf("Process B: Sent confirmation signal to Process A (PID: %d)\n", process_a_pid);
        }
    }

    // Clean up
    close(fd_read);
    close(fd_write);
    printf("Process B: Terminating...\n");
    return 0;
}