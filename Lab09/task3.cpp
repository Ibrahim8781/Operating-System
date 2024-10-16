#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
   int fd = open("test.txt",O_WRONLY | O_CREAT | O_TRUNC);
   int fd_error = dup(STDERR_FILENO);
   dup2(fd,STDERR_FILENO);
   fprintf(stderr, "Something went wrong!\n");
}
