#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handle_sigint(int sig) {
    printf("Caught signal %d\n", sig);
    _exit(0); 
}

int main() {
    // Register signal handler
    signal(SIGINT, handle_sigint);

    // Infinite loop to keep the program running
    while (1) {
        printf("Running...\n");
        sleep(1);
    }

    return 0;
}
