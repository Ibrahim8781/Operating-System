#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#define NUM_PROCESSES 6
#define READ_END 0
#define WRITE_END 1

int main() {
    int pipes[NUM_PROCESSES][2];
    pid_t pids[NUM_PROCESSES];
    
    // Create all pipes first
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (pipe(pipes[i]) == -1) {
            std::cerr << "Pipe creation failed\n";
            exit(1);
        }
    }
    
    // Create child processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            std::cerr << "Fork failed\n";
            exit(1);
        }
        
        if (pids[i] == 0) { // Child process
            // Close unused pipes
            for (int j = 0; j < NUM_PROCESSES; j++) {
                if (j == i) { // Current process reads from this pipe
                    close(pipes[j][WRITE_END]);
                } else if (j == (i + 1) % NUM_PROCESSES) { // Write to next process
                    close(pipes[j][READ_END]);
                } else { // Close unused pipes
                    close(pipes[j][READ_END]);
                    close(pipes[j][WRITE_END]);
                }
            }
            
            char buffer[1024];
            std::string message;
            
            while (true) {
                if (i == 0) { // P1 process
                    std::cout << "Enter message (type 'Quit' to exit): " << std::flush;
                    std::getline(std::cin, message);
                    
                    if (message == "Quit") {
                        // Send quit message through the pipe
                        write(pipes[1][WRITE_END], "Quit", 5);
                        exit(0);
                    }
                    
                    message += ":" + std::to_string(getpid());
                    std::cout << message << std::endl;
                    write(pipes[1][WRITE_END], message.c_str(), message.length() + 1);
                    
                    // Wait for message to come back
                    ssize_t n = read(pipes[0][READ_END], buffer, sizeof(buffer));
                    if (n > 0) {
                        std::cout << buffer << std::endl;
                    }
                } else {
                    // Other processes (C1 to C5)
                    ssize_t n = read(pipes[i][READ_END], buffer, sizeof(buffer));
                    if (n <= 0) continue;
                    
                    if (strcmp(buffer, "Quit") == 0) {
                        write(pipes[(i + 1) % NUM_PROCESSES][WRITE_END], "Quit", 5);
                        exit(0);
                    }
                    
                    message = std::string(buffer) + ":" + std::to_string(getpid());
                    std::cout << message << std::endl;
                    
                    write(pipes[(i + 1) % NUM_PROCESSES][WRITE_END], 
                          message.c_str(), message.length() + 1);
                }
            }
        }
    }
    
    // Parent process
    // Close all pipes in parent
    for (int i = 0; i < NUM_PROCESSES; i++) {
        close(pipes[i][READ_END]);
        close(pipes[i][WRITE_END]);
    }
    
    // Wait for P1 to finish
    waitpid(pids[0], nullptr, 0);
    
    // Kill remaining processes
    for (int i = 1; i < NUM_PROCESSES; i++) {
        kill(pids[i], SIGTERM);
        waitpid(pids[i], nullptr, 0);
    }
    
    return 0;
}