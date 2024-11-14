#include <stdio.h>
#include <stdlib.h>

// Structure to store process details
struct Process {
    int pid;          // Process ID
    int arrivalTime;  // Arrival Time
    int burstTime;    // Burst Time
    int priority;     // Priority
    int finishTime;   // Finish Time
    int waitTime;     // Waiting Time
    int tat;          // Turnaround Time
    int isCompleted;  // Flag to check if the process is completed
};

// Function to find the next process to execute
int selectNextProcess(struct Process proc[], int n, int currentTime) {
    int highestPriority = -1;
    int selectedIndex = -1;

    // Select the process with the highest priority that has arrived and is not completed
    for (int i = 0; i < n; i++) {
        if (proc[i].arrivalTime <= currentTime && !proc[i].isCompleted) {
            if (proc[i].priority > highestPriority || (proc[i].priority == highestPriority && proc[i].arrivalTime < proc[selectedIndex].arrivalTime)) {
                highestPriority = proc[i].priority;
                selectedIndex = i;
            }
        }
    }

    return selectedIndex;
}

// Priority Scheduling (Non-Preemptive) function
void priorityScheduling(struct Process proc[], int n) {
    int totalWaitTime = 0, totalTat = 0;
    int currentTime = 0;
    int completed = 0;

    printf("\nOrder of process execution:\n");

    while (completed < n) {
        // Find the process with the highest priority and earliest arrival time
        int selectedIndex = selectNextProcess(proc, n, currentTime);

        if (selectedIndex == -1) {
            currentTime++;  // If no process is available, increment time
            continue;
        }

        // If the selected process hasn't arrived yet, jump to its arrival time
        if (currentTime < proc[selectedIndex].arrivalTime) {
            currentTime = proc[selectedIndex].arrivalTime;
        }

        // Calculate Finish Time
        proc[selectedIndex].finishTime = currentTime + proc[selectedIndex].burstTime;

        // Calculate Turnaround Time (TAT = Finish Time - Arrival Time)
        proc[selectedIndex].tat = proc[selectedIndex].finishTime - proc[selectedIndex].arrivalTime;

        // Calculate Wait Time (Wait Time = Turnaround Time - Burst Time)
        proc[selectedIndex].waitTime = proc[selectedIndex].tat - proc[selectedIndex].burstTime;

        totalWaitTime += proc[selectedIndex].waitTime;
        totalTat += proc[selectedIndex].tat;

        printf("P%d executed from %d to %d\n", proc[selectedIndex].pid, currentTime, proc[selectedIndex].finishTime);

        // Move the current time forward to the finish time of the current process
        currentTime = proc[selectedIndex].finishTime;
        proc[selectedIndex].isCompleted = 1;  // Mark process as completed
        completed++;  // Increment the number of completed processes
    }

    // Print process details
    printf("\nProcess ID  Arrival Time  Burst Time  Priority  Finish Time  Wait Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d          %d             %d           %d        %d           %d          %d\n",
               proc[i].pid, proc[i].arrivalTime, proc[i].burstTime, proc[i].priority,
               proc[i].finishTime, proc[i].waitTime, proc[i].tat);
    }

    // Calculate and print average times
    float avgWaitTime = (float)totalWaitTime / n;
    float avgTat = (float)totalTat / n;

    printf("\nAverage Waiting Time: %.2f\n", avgWaitTime);
    printf("Average Turnaround Time: %.2f\n", avgTat);
}

int main() {
    int n;
    
    // Dynamic input: Ask for the number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Dynamically allocate memory for n processes
    struct Process *proc = (struct Process *)malloc(n * sizeof(struct Process));

    // Take input for each process
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1; // Assign Process ID (starting from 1)
        proc[i].isCompleted = 0;  // Initialize all processes as not completed
        printf("\nEnter Arrival Time, Burst Time, and Priority for Process %d: ", i + 1);
        scanf("%d %d %d", &proc[i].arrivalTime, &proc[i].burstTime, &proc[i].priority);
    }

    // Execute Priority Scheduling algorithm
    priorityScheduling(proc, n);

    // Free the dynamically allocated memory
    free(proc);

    return 0;
}
