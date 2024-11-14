#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int startTime;
    int completionTime;
};

// Function to find process with highest priority among the arrived processes at current time
int findHighestPriorityProcess(struct Process processes[], int n, int currentTime) {
    int highestPriority = INT_MAX;
    int highestPriorityIndex = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
            if (processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                highestPriorityIndex = i;
            }
        }
    }

    return highestPriorityIndex;
}

// Function to calculate waiting time, turnaround time, and average time
void calculateTimes(struct Process processes[], int n, int waitingTime[], int turnaroundTime[], float *averageWaitingTime, float *averageTurnaroundTime) {
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {
        turnaroundTime[i] = processes[i].completionTime - processes[i].arrivalTime;
        waitingTime[i] = turnaroundTime[i] - processes[i].burstTime;

        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
    }

    *averageWaitingTime = (float)totalWaitingTime / n;
    *averageTurnaroundTime = (float)totalTurnaroundTime / n;
}

// Priority Preemptive Scheduling Algorithm
void priorityPreemptive(struct Process processes[], int n) {
    int currentTime = 0;
    int remainingProcesses = n;
    int waitingTime[n], turnaroundTime[n];
    float averageWaitingTime, averageTurnaroundTime;

    while (remainingProcesses > 0) {
        int highestPriorityIndex = findHighestPriorityProcess(processes, n, currentTime);

        if (highestPriorityIndex == -1) {
            currentTime++;
            continue;
        }

        processes[highestPriorityIndex].remainingTime--;

        if (processes[highestPriorityIndex].startTime == -1)
            processes[highestPriorityIndex].startTime = currentTime;

        if (processes[highestPriorityIndex].remainingTime == 0) {
            processes[highestPriorityIndex].completionTime = currentTime + 1;
            remainingProcesses--;
        }

        currentTime++;
    }

    calculateTimes(processes, n, waitingTime, turnaroundTime, &averageWaitingTime, &averageTurnaroundTime);

    printf("Process ID\tArrival Time\tBurst Time\tPriority\tStart Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].processID, processes[i].arrivalTime, processes[i].burstTime, processes[i].priority,
               processes[i].startTime, processes[i].completionTime, waitingTime[i], turnaroundTime[i]);
    }

    printf("Average Waiting Time: %.2f\n", averageWaitingTime);
    printf("Average Turnaround Time: %.2f\n", averageTurnaroundTime);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    for (int i = 0; i < n; i++) {
        processes[i].processID = i + 1;  // Assign process ID based on input sequence
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        printf("Enter priority for process %d: ", i + 1);
        scanf("%d", &processes[i].priority);
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].startTime = -1;
        processes[i].completionTime = -1;
    }

    priorityPreemptive(processes, n);

    return 0;
}
