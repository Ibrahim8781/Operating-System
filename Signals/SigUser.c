#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handle_sigusr1(int sig) {
    printf("Received SIGUSR1: Custom user signal 1\n");
}

void handle_sigusr2(int sig) {
    printf("Received SIGUSR2: Custom user signal 2\n");
}

int main() {
    // Register handlers for SIGUSR1 and SIGUSR2
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    printf("Process running with PID: %d\n", getpid());
    printf("Send SIGUSR1 with 'kill -USR1 %d' or SIGUSR2 with 'kill -USR2 %d'\n", getpid(), getpid());

    // Run an infinite loop to keep the process alive
    while (1) {
        sleep(1);
    }

    return 0;
}
