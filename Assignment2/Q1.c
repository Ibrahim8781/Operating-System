#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_QUEUES 4

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    bool completed;
} Process;

typedef struct {
    Process processes[MAX_PROCESSES];
    int count;
    int quantum;
    char name[50];
} Queue;

Queue queues[MAX_QUEUES];

void read_processes(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }

    // Read and copy processes to each queue
    int pid, arrival, burst, priority;
    int count = 0;
    while (fscanf(file, "%d %d %d %d", &pid, &arrival, &burst, &priority) == 4) {
        for (int i = 0; i < MAX_QUEUES; i++) {
            queues[i].processes[count].pid = pid;
            queues[i].processes[count].arrival_time = arrival;
            queues[i].processes[count].burst_time = burst;
            queues[i].processes[count].priority = priority;
            queues[i].processes[count].remaining_time = burst;
            queues[i].processes[count].waiting_time = 0;
            queues[i].processes[count].turnaround_time = 0;
            queues[i].processes[count].completed = false;
            queues[i].count++;
        }
        count++;
    }
    fclose(file);
}

// Initialize queues
void setup_queues(int num_queues) {
    char* names[] = {"Priority Scheduling", "Round Robin (Q=8)", 
                     "Round Robin (Q=10)", "FCFS"};
    int quantums[] = {0, 8, 10, 0};

    for (int i = 0; i < num_queues; i++) {
        queues[i].count = 0;
        queues[i].quantum = quantums[i];
        strcpy(queues[i].name, names[i]);
    }
}

// Priority Scheduling
void run_priority(Queue* q) {
    int current_time = 0;
    int completed = 0;
    
    while (completed < q->count) {
        // Find highest priority process
        int selected = -1;
        int highest_priority = -1;
        
        for (int i = 0; i < q->count; i++) {
            if (!q->processes[i].completed && 
                q->processes[i].arrival_time <= current_time && 
                q->processes[i].priority > highest_priority) {
                selected = i;
                highest_priority = q->processes[i].priority;
            }
        }

        if (selected == -1) {
            current_time++;
            continue;
        }

        // Process the selected process
        q->processes[selected].remaining_time--;
        
        // Update waiting time for other processes
        for (int i = 0; i < q->count; i++) {
            if (i != selected && !q->processes[i].completed && 
                q->processes[i].arrival_time <= current_time) {
                q->processes[i].waiting_time++;
            }
        }

        current_time++;

        // Check if process is completed
        if (q->processes[selected].remaining_time == 0) {
            q->processes[selected].completed = true;
            q->processes[selected].turnaround_time = 
                current_time - q->processes[selected].arrival_time;
            completed++;
        }
    }
}

// Round Robin
void run_round_robin(Queue* q) {
    int current_time = 0;
    int completed = 0;
    
    while (completed < q->count) {
        for (int i = 0; i < q->count; i++) {
            if (!q->processes[i].completed && 
                q->processes[i].arrival_time <= current_time) {
                
                int run_time = q->quantum;
                if (q->processes[i].remaining_time < run_time) {
                    run_time = q->processes[i].remaining_time;
                }

                // Execute process
                q->processes[i].remaining_time -= run_time;
                current_time += run_time;

                // Update waiting time for other processes
                for (int j = 0; j < q->count; j++) {
                    if (j != i && !q->processes[j].completed && 
                        q->processes[j].arrival_time <= current_time) {
                        q->processes[j].waiting_time += run_time;
                    }
                }

                // Check if process completed
                if (q->processes[i].remaining_time == 0) {
                    q->processes[i].completed = true;
                    q->processes[i].turnaround_time = 
                        current_time - q->processes[i].arrival_time;
                    completed++;
                }
            }
        }
        if (completed == 0) current_time++;  // If no process was ready
    }
}

// FCFS
void run_fcfs(Queue* q) {
    int current_time = 0;
    
    for (int i = 0; i < q->count; i++) {
        if (current_time < q->processes[i].arrival_time) {
            current_time = q->processes[i].arrival_time;
        }
        
        q->processes[i].waiting_time = current_time - q->processes[i].arrival_time;
        current_time += q->processes[i].burst_time;
        q->processes[i].turnaround_time = current_time - q->processes[i].arrival_time;
        q->processes[i].completed = true;
    }
}

// Print results
void show_results(Queue* q) {
    printf("\n%s Statistics:\n", q->name);
    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");
    
    float total_waiting = 0, total_turnaround = 0;
    
    for (int i = 0; i < q->count; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n",
               q->processes[i].pid,
               q->processes[i].arrival_time,
               q->processes[i].burst_time,
               q->processes[i].waiting_time,
               q->processes[i].turnaround_time);
        
        total_waiting += q->processes[i].waiting_time;
        total_turnaround += q->processes[i].turnaround_time;
    }
    
    printf("Average Waiting Time: %.2f\n", total_waiting / q->count);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / q->count);
}

int main() {
    int num_queues;
    char filename[100];
    
    printf("Enter the number of scheduling algorithms to implement (max 4): ");
    scanf("%d", &num_queues);
    
    if (num_queues < 1 || num_queues > MAX_QUEUES) {
        printf("Invalid number of queues!\n");
        return 1;
    }
    
    printf("Enter the input file name: ");
    scanf("%s", filename);
    
    setup_queues(num_queues);
    read_processes(filename);
    
    // Run each scheduling algorithm
    for (int i = 0; i < num_queues; i++) {
        switch(i) {
            case 0: run_priority(&queues[i]); break;
            case 1:
            case 2: run_round_robin(&queues[i]); break;
            case 3: run_fcfs(&queues[i]); break;
        }
        show_results(&queues[i]);
    }
    
    return 0;
}