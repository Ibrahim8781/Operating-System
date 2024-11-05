#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Process running with PID: %d\n", getpid());
    printf("Send SIGKILL with 'kill -KILL %d' to force termination\n", getpid());

    // Run an infinite loop to keep the process alive
    while (1) {
        printf("Running...\n");
        sleep(2);
    }

    return 0;
}
