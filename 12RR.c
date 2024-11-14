#include <stdio.h>
#include <stdbool.h>

struct Process {
    int pid;          
    int arrivalTime;  
    int burstTime;    
    int remainingTime; 
    int finishTime;   
    int waitTime;     
    int tat;          
    bool isCompleted; 
};

void roundRobin(struct Process proc[], int n, int timeQuantum) {
    int currentTime = 0;
    int completedProcesses = 0;
    int totalWaitTime = 0, totalTat = 0;

    while (completedProcesses < n) {
        bool processExecuted = false;

        for (int i = 0; i < n; i++) {
            if (proc[i].arrivalTime <= currentTime && !proc[i].isCompleted) {
                // Process the current process if it has arrived and not yet completed
                if (proc[i].remainingTime > 0) {
                    if (proc[i].remainingTime > timeQuantum) {
                        
                        printf("P%d executed from %d to %d\n", proc[i].pid, currentTime, currentTime + timeQuantum);
                        currentTime += timeQuantum;
                        proc[i].remainingTime -= timeQuantum;
                    } else {
                        printf("P%d executed from %d to %d\n", proc[i].pid, currentTime, currentTime + proc[i].remainingTime);
                        currentTime += proc[i].remainingTime;
                        proc[i].finishTime = currentTime;
                        proc[i].remainingTime = 0;
                        proc[i].isCompleted = true;
                        completedProcesses++;

                   
                        proc[i].tat = proc[i].finishTime - proc[i].arrivalTime;
                        proc[i].waitTime = proc[i].tat - proc[i].burstTime;

                        totalWaitTime += proc[i].waitTime;
                        totalTat += proc[i].tat;
                    }
                    processExecuted = true;
                }
            }
        }

        
        if (!processExecuted) {
            currentTime++;
        }
    }

    printf("\nProcess ID  Arrival Time  Burst Time  Finish Time  Wait Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d          %d             %d           %d           %d          %d\n",
               proc[i].pid, proc[i].arrivalTime, proc[i].burstTime, proc[i].finishTime,
               proc[i].waitTime, proc[i].tat);
    }

    float avgWaitTime = (float)totalWaitTime / n;
    float avgTat = (float)totalTat / n;

    printf("\nAverage Waiting Time: %.2f\n", avgWaitTime);
    printf("Average Turnaround Time: %.2f\n", avgTat);
}

int main() {
    int n, timeQuantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    struct Process proc[n];

   
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1; // Assign Process ID (starting from 1)
        printf("\nEnter Arrival Time and Burst Time for Process %d: ", i + 1);
        scanf("%d %d", &proc[i].arrivalTime, &proc[i].burstTime);
        proc[i].remainingTime = proc[i].burstTime;
        proc[i].isCompleted = false;
    }

    roundRobin(proc, n, timeQuantum);

    return 0;
}

