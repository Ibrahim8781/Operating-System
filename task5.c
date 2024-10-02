#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_left_triangle(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

void print_inverted_full_triangle(int n) {
    for (int i = n; i >= 1; i--) {
        for (int j = 1; j <= n - i; j++) {
            printf("  ");
        }
        for (int j = 1; j <= 2 * i - 1; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

void print_right_triangle(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n - i; j++) {
            printf("  ");
        }
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Invalid Arugrmaents\n");
        return 1;
    }

    char *pattern_option = argv[1];
    int number = atoi(argv[2]);

    if (number <= 0) {
        fprintf(stderr, "Enter Positive Number\n");
        return 1;
    }

    if (strcmp(pattern_option, "left") == 0) {
        print_left_triangle(number);
    } else if (strcmp(pattern_option, "inverted_full") == 0) {
        print_inverted_full_triangle(number);
    } else if (strcmp(pattern_option, "right") == 0) {
        print_right_triangle(number);
    } else {
        fprintf(stderr, "Wrong Pattern Entered , only enter left | right | inverted_full .\n");
        return 1;
    }

    return 0;
}