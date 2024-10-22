#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    pid_t pid = fork();

    if(pid==0)
    {
        printf("In child process\n");
        execl("/bin/ps", "ps", NULL);
    }
    else
    {
        wait(NULL);
        printf("In parent process\n");
    }

    return 0;
}
 