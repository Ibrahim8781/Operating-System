#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_BUF 1024

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    int fd;
    char buf[MAX_BUF];

    // Open the named pipe for reading
    fd = open("vowel_pipe", O_RDONLY);

    while (1) {
        int bytes_read = read(fd, buf, MAX_BUF);
        if (bytes_read <= 0) break;

        buf[bytes_read] = '\0';
        int vowels = count_vowels(buf);
        printf("Vowel count: %d\n", vowels);
    }

    close(fd);
    return 0;
}