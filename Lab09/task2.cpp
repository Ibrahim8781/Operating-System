
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
   int fd = open("test.txt",O_RDONLY);
   dup2(fd,STDIN_FILENO);
   char str[100];
   scanf("%s",str);
   printf("%s",str);
}
