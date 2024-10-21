#include<studio.h>
#include<unistd.h>

int main(){

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
        printf("Parent process\n");
        vaiable -= 5;
        printf("Parent process vaiable: %d\n and PID %d\n", vaiable, getpid());
    }


    /*
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