#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_BUF 1024

int main() {
    int fd_vowels, fd_reverse, fd_capitals;
    char input[MAX_BUF];

    // Create named pipes
    mkfifo("vowel_pipe", 0666);
    mkfifo("reverse_pipe", 0666);
    mkfifo("capital_pipe", 0666);

    // Open pipes for writing
    fd_vowels = open("vowel_pipe", O_WRONLY);
    fd_reverse = open("reverse_pipe", O_WRONLY);
    fd_capitals = open("capital_pipe", O_WRONLY);

    while (1) {
        printf("Enter a string (or 'quit' to exit): ");
        fgets(input, MAX_BUF, stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline

        if (strcmp(input, "quit") == 0) {
            break;
        }

        // Write input to all pipes
        write(fd_vowels, input, strlen(input) + 1);
        write(fd_reverse, input, strlen(input) + 1);
        write(fd_capitals, input, strlen(input) + 1);
    }

    // Close pipes
    close(fd_vowels);
    close(fd_reverse);
    close(fd_capitals);

    // Remove named pipes
    unlink("vowel_pipe");
    unlink("reverse_pipe");
    unlink("capital_pipe");

    return 0;
}