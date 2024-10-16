#include <stdio.h>
#include <stdlib.h>

// Define the opcodes as an enum for readability
enum OpCode {
    ADD = 1,    // Add value from memory to register
    SUB = 2,    // Subtract value from memory from register
    LOAD = 3,   // Load value from memory into register
    STORE = 4,  // Store value from register into memory
    HALT = 5    // Stop the program
};

// Registers and memory for our simple CPU
int regA = 0; 
int pc = 0;   
int memory[10];

void fetch_decode_execute();
void load_sample_program1();
void load_sample_program2();

int main() {
    printf("Loading Sample Program 1:\n");
    load_sample_program1();

    // Run the fetch-decode-execute cycle
    while (1) {
        fetch_decode_execute();
    }

    return 0;
}

// Fetch-Decode-Execute function
void fetch_decode_execute() {
    int opcode = memory[pc];

    printf("PC: %d, regA: %d\n", pc, regA);
    
    switch (opcode) {
        case LOAD:
            pc++; 
            regA = memory[memory[pc]]; // Load value from memory into regA
            printf("LOAD: Loaded %d into register A from memory[%d]\n", regA, memory[pc]);
            break;

        case ADD:
            pc++; 
            regA += memory[memory[pc]]; // Add value from memory to regA
            printf("ADD: Added %d from memory[%d] to register A, now A=%d\n", memory[memory[pc]], memory[pc], regA);
            break;

        case SUB:
            pc++; 
            regA -= memory[memory[pc]]; // Subtract value from memory from regA
            printf("SUB: Subtracted %d from memory[%d] from register A, now A=%d\n", memory[memory[pc]], memory[pc], regA);
            break;

        case STORE:
            pc++; 
            memory[memory[pc]] = regA; // Store regA value into memory
            printf("STORE: Stored %d from register A into memory[%d]\n", regA, memory[pc]);
            break;

        case HALT:
            printf("HALT: Program halted\n");
            exit(0); 
            break;

        default:
            printf("Unknown opcode at PC=%d\n", pc);
            exit(1); 
            }

    printf("PC: %d, regA: %d\n", pc, regA);
}

