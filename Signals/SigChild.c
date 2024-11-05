#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sigchld(int sig) {
    int status;
    pid_t pid = wait(&status);  // Wait for the terminated child process
    printf("Received SIGCHLD: Child with PID %d has terminated.\n", pid);
}

int main() {
    // Register the SIGCHLD handler
    signal(SIGCHLD, handle_sigchld);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("Child process running with PID %d\n", getpid());
        sleep(2);  // Simulate some work in the child process
        printf("Child process terminating.\n");
        exit(0);
    } else if (pid > 0) {
        // Parent process
        printf("Parent process waiting for child to terminate.\n");
        while (1) {
            sleep(1);  // Keep parent running indefinitely
        }
    } else {
        perror("fork failed");
    }

    return 0;
}
