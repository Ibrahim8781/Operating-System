#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("In child process\n");

        // Define custom environment variables
        char *env[] = { "MESSAGE=Hello from execle!", NULL };

        // Use execle to run the script with custom environment variables
        execle("user/bin/script.sh", "script.sh", NULL, env);
        //execle("/script.sh", "script.sh", NULL, env);
        
        // If execle fails
        perror("execle failed");
    } else {
        wait(NULL);  // Wait for child process
        printf("In parent process\n");
    }

    /*
    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("In child process\n");

        // Define custom environment variables
        char *env[] = { "MY_VAR=HelloWorld", NULL };

        // Use execle to execute /usr/bin/env and pass custom environment
        execle("/usr/bin/env", "env", NULL, env);
        
        // If execle fails
        perror("execle failed");
    } else {
        wait(NULL);  // Wait for the child to complete
        printf("In parent process\n");
    }
    */


    return 0;
}
