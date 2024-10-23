#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid == 0) {  // Child process
        char commands[100]; // Buffer for user input
        printf("Enter the command: ");
        scanf("%99s", commands); // Read command (up to 99 characters)

        // Prepare arguments; for simplicity, we only take one argument here
        char *args[]={commands, NULL}; 

        execvp(commands, args); // Execute command

    } else {  // Parent process
        wait(NULL);  // Wait for the child process to finish
        printf("Child process completed\n");
    }
    return 0;
}
