#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_BUF 1024

void capitalize_string(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int main() {
    int fd;
    char buf[MAX_BUF];

    // Open the named pipe for reading
    fd = open("capital_pipe", O_RDONLY);

    while (1) {
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) break;

        buf[bytes_read] = '\0';
        capitalize_string(buf);
        printf("Capitalized string: %s\n", buf);
    }

    close(fd);
    return 0;
}