#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("In child process\n");
        printf("Going to find .c files...\n");
        
        // Arguments for find command
        char *commands[] = {"find", ".", "-name", "*.c", NULL};

        // Correct path to find executable
        execv("/usr/bin/find", commands);
        
        // If execv fails
        perror("execv failed");
    } else {
        wait(NULL);
        printf("In parent process\n");
    }

    /*
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("In child process\n");

        // Take user input dynamically
        char command[100];
        printf("Enter the command: ");
        scanf("%s", command);  // Note: this only reads one word

        char* args[] = {command, NULL};
        execv("/bin/" , args); // Assuming the command is in /bin
        
        // If execv fails
        perror("execv failed");
    } else {
        wait(NULL);
        printf("In parent process\n");
    }
    */
    return 0;
}
