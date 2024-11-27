// p1.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>

#define PIPE_A_TO_B "/tmp/pipe_a_to_b"
#define PIPE_B_TO_A "/tmp/pipe_b_to_a"
#define MAX_MSG_SIZE 256
#define NUM_MESSAGES 5

// Global variables for pipe file descriptors
int fd_write, fd_read;
volatile sig_atomic_t signal_received = 0;

// Signal handler function
void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        signal_received = 1;
        printf("Process A: Received confirmation signal from Process B\n");
    }
}

// Thread function to generate and send messages
void* message_generator(void* arg) {
    char message[MAX_MSG_SIZE];
    for (int i = 1; i <= NUM_MESSAGES; i++) {
        snprintf(message, MAX_MSG_SIZE, "Message %d from Process A", i);
        write(fd_write, message, strlen(message) + 1);
        printf("Process A: Sent message: %s\n", message);
        sleep(1);  // Add delay between messages
    }
    // Send termination message
    strcpy(message, "END");
    write(fd_write, message, strlen(message) + 1);
    return NULL;
}

// Thread function to handle received messages
void* message_handler(void* arg) {
    char buffer[MAX_MSG_SIZE];
    while (1) {
        ssize_t bytes_read = read(fd_read, buffer, MAX_MSG_SIZE);
        if (bytes_read > 0) {
            if (strcmp(buffer, "END") == 0) {
                break;
            }
            printf("Process A: Received processed message: %s\n", buffer);
        }
    }
    return NULL;
}

// Thread function to wait for signals
void* signal_waiter(void* arg) {
    while (1) {
        while (!signal_received) {
            pause();  // Wait for signal
        }
        printf("Process A: Signal waiter acknowledged signal\n");
        signal_received = 0;  // Reset signal flag
    }
    return NULL;
}

int main() {
    // Remove existing pipes if they exist
    unlink(PIPE_A_TO_B);
    unlink(PIPE_B_TO_A);

    // Create named pipes
    if (mkfifo(PIPE_A_TO_B, 0666) == -1 && errno != EEXIST) {
        perror("Process A: Error creating pipe A to B");
        exit(1);
    }
    if (mkfifo(PIPE_B_TO_A, 0666) == -1 && errno != EEXIST) {
        perror("Process A: Error creating pipe B to A");
        unlink(PIPE_A_TO_B);
        exit(1);
    }

    // Set up signal handling
    struct sigaction sa = {0};  // Initialize to zero
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Process A: Error setting up signal handler");
        exit(1);
    }

    printf("Process A: Started (PID: %d)\n", getpid());
    printf("Process A: Opening pipes...\n");

    // Open pipes (blocks until both ends are opened)
    printf("Process A: Waiting for Process B to connect...\n");
    fd_write = open(PIPE_A_TO_B, O_WRONLY);
    if (fd_write == -1) {
        perror("Process A: Error opening write pipe");
        exit(1);
    }
    
    fd_read = open(PIPE_B_TO_A, O_RDONLY);
    if (fd_read == -1) {
        perror("Process A: Error opening read pipe");
        close(fd_write);
        exit(1);
    }

    printf("Process A: Connected to pipes\n");

    // Create threads
    pthread_t thread_gen, thread_handler, thread_signal;
    pthread_create(&thread_gen, NULL, message_generator, NULL);
    pthread_create(&thread_handler, NULL, message_handler, NULL);
    pthread_create(&thread_signal, NULL, signal_waiter, NULL);

    // Wait for message generator to complete
    pthread_join(thread_gen, NULL);
    sleep(2);  // Allow time for final messages

    // Clean up
    close(fd_write);
    close(fd_read);
    unlink(PIPE_A_TO_B);
    unlink(PIPE_B_TO_A);

    printf("Process A: Terminating...\n");
    return 0;
}