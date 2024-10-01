#include <stdio.h>
#include <unistd.h>  // for fork(), getpid()
#include <sys/wait.h> // for wait()

int main() {
    pid_t pidB, pidC, pidD, pidE, pidF, pidG, pidH, pidI, pidJ, pidK, pidL;

    // Root Process (A)
    printf("Process A: PID = %d\n", getpid());
    
    // Fork process B (Left child of A)
    printf("Process A is about to create Process B...\n");
    pidB = fork();

    if (pidB < 0) {
        perror("Fork failed");
        return 1;
    }
    
    if (pidB == 0) {
        // Inside child process B
        printf("Process B (Child of A): PID = %d, Parent PID = %d\n", getpid(), getppid());

        // Fork process D (Child of B)
        printf("Process B is about to create Process D...\n");
        pidD = fork();
        if (pidD == 0) {
            printf("Process D (Child of B): PID = %d, Parent PID = %d\n", getpid(), getppid());
        } else {
            // Fork process E (Child of B)
            printf("Process B is about to create Process E...\n");
            pidE = fork();
            if (pidE == 0) {
                printf("Process E (Child of B): PID = %d, Parent PID = %d\n", getpid(), getppid());
            } else {
                // Parent B waits for D and E to finish
                wait(NULL);  // Wait for D
                wait(NULL);  // Wait for E
                printf("Process B completed: PID = %d\n", getpid());
            }
        }

    } else {
        // Fork process C (Right child of A)
        printf("Process A is about to create Process C...\n");
        pidC = fork();

        if (pidC == 0) {
            // Inside child process C
            printf("Process C (Child of A): PID = %d, Parent PID = %d\n", getpid(), getppid());

            // Fork process F (Child of C)
            printf("Process C is about to create Process F...\n");
            pidF = fork();
            if (pidF == 0) {
                printf("Process F (Child of C): PID = %d, Parent PID = %d\n", getpid(), getppid());
            } else {
                // Fork process G (Child of C)
                printf("Process C is about to create Process G...\n");
                pidG = fork();
                if (pidG == 0) {
                    printf("Process G (Child of C): PID = %d, Parent PID = %d\n", getpid(), getppid());
                } else {
                    // Fork process H (Child of F)
                    printf("Process F is about to create Process H...\n");
                    pidH = fork();
                    if (pidH == 0) {
                        printf("Process H (Child of F): PID = %d, Parent PID = %d\n", getpid(), getppid());
                    } else {
                        // Fork process I (Child of G)
                        printf("Process G is about to create Process I...\n");
                        pidI = fork();
                        if (pidI == 0) {
                            printf("Process I (Child of G): PID = %d, Parent PID = %d\n", getpid(), getppid());
                        } else {
                            // Wait for H and I
                            wait(NULL);  // Wait for H to finish
                            wait(NULL);  // Wait for I to finish
                            printf("Process C completed: PID = %d\n", getpid());
                        }
                    }
                }
            }

        } else {
            // Parent A waits for B and C to finish
            wait(NULL);  // Wait for B to finish
            wait(NULL);  // Wait for C to finish
            printf("Process A completed: PID = %d\n", getpid());
        }
    }

    return 0;
}
