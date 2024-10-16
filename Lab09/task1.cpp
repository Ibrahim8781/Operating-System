#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
   int fd = open("test.txt",O_WRONLY | O_CREAT | O_TRUNC);
   int fd_copy = dup(fd);
   dup2(fd_copy,1);
   printf("Assalamualikum Sir Daniyal");
}
