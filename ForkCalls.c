#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("m: PID = %d (Root)\n", getpid());

    pid_t pidM1 = fork();  

    if (pidM1 < 0) {
        printf("Fork failed\n");
        return 1;
    } else if (pidM1 == 0) {
        // m1 lefot of m
        printf("m1: PID = %d, m PID = %d\n", getpid(), getppid());

        pid_t pidM2 = fork();  

        if (pidM2 == 0) {
            // Inside m2 lefot of m1
            printf("m2: PID = %d, m1 PID = %d\n", getpid(), getppid());

            // C5 C6 children of m2
            pid_t pidC5 = fork();
            if (pidC5 == 0) {
                printf("C5: PID = %d, m2 PID = %d\n", getpid(), getppid());
                
                // C9 C10 children of C5
                pid_t pidC9 = fork();
                if (pidC9 == 0) {
                    printf("C9: PID = %d, C5 PID = %d\n", getpid(), getppid());
                    return 0;
                }
                wait(NULL);  

                pid_t pidC10 = fork();
                if (pidC10 == 0) {
                    printf("C10: PID = %d, C5 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  
                return 0;  
            }
            wait(NULL);  

            pid_t pidC6 = fork();
            if (pidC6 == 0) {
                printf("C6: PID = %d, m2 PID = %d\n", getpid(), getppid());
                
                // C11  C12 child C6
                pid_t pidC11 = fork();
                if (pidC11 == 0) {
                    printf("C11: PID = %d, C6 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  

                pid_t pidC12 = fork();
                if (pidC12 == 0) {
                    printf("C12: PID = %d, C6 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  
                return 0;  
            }
            wait(NULL);  
            return 0;  
        }
        wait(NULL);  

        // right child of m1
        pid_t pidC2 = fork();
        if (pidC2 == 0) {
            printf("C2: PID = %d, m1 PID = %d\n", getpid(), getppid());
            
            // C7, C8 child of C2
            pid_t pidC7 = fork();
            if (pidC7 == 0) {
                printf("C7: PID = %d, C2 PID = %d\n", getpid(), getppid());
                
                // C13 C14 children of C7
                pid_t pidC13 = fork();
                if (pidC13 == 0) {
                    printf("C13: PID = %d, C7 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  

                pid_t pidC14 = fork();
                if (pidC14 == 0) {
                    printf("C14: PID = %d, C7 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  
                return 0;  
            }
            wait(NULL);  

            pid_t pidC8 = fork();
            if (pidC8 == 0) {
                printf("C8: PID = %d, C2 PID = %d\n", getpid(), getppid());
                
                // C15 C16 child of C8
                pid_t pidC15 = fork();
                if (pidC15 == 0) {
                    printf("C15: PID = %d, C8 PID = %d\n", getpid(), getppid());
                    return 0;
                }
                wait(NULL);  

                pid_t pidC16 = fork();
                if (pidC16 == 0) {
                    printf("C16: PID = %d, C8 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  
                return 0;  
            }
            wait(NULL);  
            return 0;  
        }
        wait(NULL);  
        return 0;  
    } else {
        // root m 
        pid_t pidC1 = fork();
        if (pidC1 == 0) {
            printf("C1: PID = %d, m PID = %d\n", getpid(), getppid());
            
            //right child of m
            pid_t pidC3 = fork();
            if (pidC3 == 0) {
                printf("C3: PID = %d, C1 PID = %d\n", getpid(), getppid());
                
                // Create C17 and C18 as children of C3
                pid_t pidC17 = fork();
                if (pidC17 == 0) {
                    printf("C17: PID = %d, C3 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  

                pid_t pidC18 = fork();
                if (pidC18 == 0) {
                    printf("C18: PID = %d, C3 PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  
                return 0;  
            }
            wait(NULL);  

            pid_t pidC4 = fork();
            if (pidC4 == 0) {
                printf("C4: PID = %d, C1 PID = %d\n", getpid(), getppid());
                
                // C19 child C4
                pid_t pidC19 = fork();
                if (pidC19 == 0) {
                    printf("C19: PID = %d,  PID = %d\n", getpid(), getppid());
                    return 0;  
                }
                wait(NULL);  
                return 0;  
            }
            wait(NULL);  
            return 0;  
        }
        wait(NULL);  
    }

    wait(NULL);  
    return 0;
}