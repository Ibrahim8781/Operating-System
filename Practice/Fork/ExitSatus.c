#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>


int main(){

    pid_t pid = fork();
    if(pid == -1){
        perror("fork failed");
        return 1;
    }
    else if(pid == 0){
        int age=0;
        age = scanf("Enter age %d", &age);
        return age;
    }
    else{
        int age;
        wait(&age);
        printf("In parent process\n");
        printf("Age is %d\n", WEXITSTATUS(age));
    }

    return 0;
}
// Pid t Pidl • fork').
// if Ipidl
// perror( •Fork failed") r
// return 1,


