#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>
int main(){
   int p[2];
   pid_t child;
   pipe(p);
   child = fork();

   if(child == 0)
   {
       close(p[0]);
       dup2(p[1],STDOUT_FILENO);
       printf("Message from child");
       close(p[1]);
       exit(0);
   }
   else{
    
       char msg[100];
       close(p[1]);
       ssize_t n = read(p[0],msg,sizeof(msg)-1);
       msg[n] = '\0';
       printf("%s",msg);
       wait(NULL);
   }
}
