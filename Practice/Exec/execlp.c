#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("In child process\n");

        // Use execlp to run ls (search in PATH)
        execlp("ls", "ls", "-l", NULL);
        
        // If execlp fails
        perror("execlp failed");
    } else {
        wait(NULL);  // Wait for the child to complete
        printf("In parent process\n");
    }

    return 0;
}
