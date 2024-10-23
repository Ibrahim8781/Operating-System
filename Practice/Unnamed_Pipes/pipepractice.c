#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2]; // Array to hold the read and write ends of the pipe
    pid_t pid;
    char buf[20];
    char msg[] = "Hello from parent!";

    int a =pipe(pipefd);
    // Create the unnamed pipe
    pid = fork(); // Create a new process

    if (pid == 0) { // Child process
    
        close(pipefd[1]); // Close the write end of the pipe
        ssize_t bytesRead = read(pipefd[0], buf, sizeof(buf));
        if (bytesRead == -1) {
            perror("Error reading from pipe");
            exit(3);
        }
        buf[bytesRead] = '\0'; // Null-terminate the buffer
        printf("Child read: %s\n", buf);
        close(pipefd[0]); // Close the read end of the pipe
        exit(0);
    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe
        ssize_t bytesWritten = write(pipefd[1], msg, strlen(msg) + 1); // Include null terminator
        if (bytesWritten == -1) {
            perror("Error writing to pipe");
            exit(4);
        }
        close(pipefd[1]); // Close the write end of the pipe
        wait(NULL); // Wait for the child process to finish
        exit(0);
    }
}
