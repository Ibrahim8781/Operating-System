// #include <unistd.h>
// #include<stdio.h>
// #include<sys/wait.h>

// int variable = 0;


// int main(){
//     int pid;
//     pid = fork();

//     if (pid == 0 ){
//         variable += 5;
//         printf("Child 1 with varaible value %d \n",variable);
//     }
//     else
//     {
//         int pid2 = fork();
//         if (pid2 == 0)
//         {
//             variable += 10;
//             printf("Child 2 with varaible value %d \n",variable);
//         }
//         else{
//             wait(NULL);
//             wait(NULL);
//             printf("Parent with varaible value %d \n",variable);
//         }
//     }

// return 0;
// }

// #include<unistd.h>
// #include<sys/wait.h>
// #include<stdio.h>

// int main(){

//     int pid , pid2 ;
//     pid = fork();
//     int var1 , var2 , var3;
//     if (pid==0){


//         pid2 = fork();

//         if (pid2 == 0)
//         {
//             var2 = getpid();
//             printf("Child 2 with its PID %d and Parent PID %d \n", getpid(), getppid());
//         } 
//         else{
//             var1 = getpid();
//             printf("Child 1 with its PID %d and Parent PID %d \n", getpid(), getppid());
//         }
//     }
//     else{
//         wait(NULL);
//         printf("Parent with its PID %d and Parent PID %d \n", getpid(), getppid());
//         printf("Child 1 %d \n", pid);
//         printf("Child 2 %d \n", pid2);
//     } 
// }

#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){

    int pid;

    int sum = 0;
    for (int i =0 ; i < 3 ; i++){
        pid = fork();

        if (pid == 0)
        {
            printf("Child %d with PID %d and Parent PID %d \n", i+1 , getpid() , getppid() );
            sum +=5;
            printf("Child %d completed task with variable %d \n", i+1 , sum );
            _exit(0);
        }
        else{
            wait(NULL);
            printf( "Parent : Child %d with PID %d has completed \n", i+1 , getpid());
        }

    }
    return 0;
}