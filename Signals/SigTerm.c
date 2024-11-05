#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_sigterm(int sig) {
    printf("Received SIGTERM. Exiting gracefully...\n");
    // Perform cleanup tasks here
    exit(0);
}

int main() {
    // Register the SIGTERM handler
    signal(SIGTERM, handle_sigterm);

    printf("Process running with PID: %d\n", getpid());
    printf("Send SIGTERM with 'kill -TERM %d' to terminate\n", getpid());

    // Run an infinite loop to keep the process alive
    while (1) {
        printf("Running...\n");
        sleep(2);
    }

    return 0;
}
