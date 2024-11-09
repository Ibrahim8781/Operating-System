#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main() {
    int fd;
    char buf[MAX_BUF];

    // Open the named pipe for reading
    fd = open("reverse_pipe", O_RDONLY);

    while (1) {
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) break;

        buf[bytes_read] = '\0';
        reverse_string(buf);
        printf("Reversed string: %s\n", buf);
    }

    close(fd);
    return 0;
}