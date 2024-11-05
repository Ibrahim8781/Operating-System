/*You need to design a data-processing pipeline involving three steps, each done by a separate process using fork() and exec().

Process 1: Generate a list of random integers and send it to the next process.
Process 2: Sort the integers and send the sorted result to the next process.
Process 3: Count the number of even and odd integers in the sorted list and display the result.
Each process should use the exec() system call to run a different program to perform its respective task. The programs should communicate via pipes, and all data transfer should use system calls (read(), write()).

Program 1: Generates an array of random integers.
Program 2: Receives the array, sorts it, and passes it on.
Program 3: Receives the sorted array, counts even/odd numbers, and prints the result. */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<math.h>

int main(){

    int pipe1[2],pipe2[2],pipe3[2];

    if(pipe(pipe1)==-1 || pipe(pipe2)==-1 || pipe(pipe3)==-1){
        perror("pipe1 failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();

    if(pid1<0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if(pid1==0){
        int*data[11] = {};
        for (int i=0;i<11;i++){
            //insert random smaller than hundred 
            data[i] = rand()%100;
        }
    }

    return 0;
}