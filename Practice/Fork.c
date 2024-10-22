#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(){


    pid_t pid = fork();

    if(pid == -1){
        printf("Error\n");
    }

    if(pid == 0){
        printf("Child process with PID %d\n", getpid());

        pid_t pid2 = fork();

        if(pid2 == 0){
            printf("Child 2 process with PID %d and PPID %d\n ", getpid(), getppid());
        }
        else{
            
            printf("Child 1 process with PID %d and PPID %d\n ", getpid(), getppid());
        }

    }

    else{
        printf("Parent process with PID %d and PPID %d\n ", getpid(), getppid());
    }

    /*
    int vaiable = 10;

    pid_t pid = fork();

    if(pid == 0){
        printf("Child process\n");
        vaiable += 5;
        printf("Child process vaiable: %d\n and PID %d\n", vaiable, getpid());
    }
    else if(pid == -1){
        printf("Error\n");
    }
    else{
        wait(NULL);
        printf("Parent process\n");
        vaiable -= 5;
        printf("Parent process vaiable: %d\n and PID %d\n", vaiable, getpid());
    }

    -------------------------------------
    pip_t pid = fork();

    if(pid == 0){
        printf("Child process\n");
    }
    else if(pid == -1){
        printf("Error\n");
    }
    else{
        printf("Parent process\n");
    }
    */
    return 0;
}