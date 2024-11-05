#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_sigsegv(int sig) {
    printf("Caught SIGSEGV: Segmentation fault occurred!\n");
    // Perform any logging or cleanup here
    exit(1);
}

int main() {
    // Register the SIGSEGV handler
    signal(SIGSEGV, handle_sigsegv);

    printf("Process running with PID: %d\n", getpid());
    printf("Triggering a segmentation fault...\n");

    // Intentionally cause a segmentation fault
    int *ptr = NULL;
    *ptr = 42;  // Dereference NULL pointer to trigger SIGSEGV

    return 0;
}
