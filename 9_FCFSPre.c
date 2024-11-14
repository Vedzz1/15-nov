#include <stdio.h>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingBurstTime;  // Tracks remaining burst time for preemption
    int finishTime;
    int waitingTime;
    int turnaroundTime;
};

void sortByArrivalTime(struct Process processes[], int n) {
    struct Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime) {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void calculateAverageTime(struct Process processes[], int n) {
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    int currentTime = 0;
    int completed = 0;

    // Round-robin simulation
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            // Skip process if it is already completed
            if (processes[i].remainingBurstTime > 0) {
                // If the process arrives at or before the current time, execute it
                if (processes[i].arrivalTime <= currentTime) {
                    // Execute the process for up to 2 units of time (time quantum)
                    int timeSlice = (processes[i].remainingBurstTime > 2) ? 2 : processes[i].remainingBurstTime;
                    processes[i].remainingBurstTime -= timeSlice;
                    currentTime += timeSlice;

                    // If process is completed, calculate waiting and turnaround time
                    if (processes[i].remainingBurstTime == 0) {
                        processes[i].finishTime = currentTime;
                        processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
                        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
                        totalWaitingTime += processes[i].waitingTime;
                        totalTurnaroundTime += processes[i].turnaroundTime;
                        completed++;
                    }
                }
            }
        }
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    float avgTurnaroundTime = (float)totalTurnaroundTime / n;

    // Output
    printf("\nProcess\t Arrival Time\t Burst Time\t Finish Time\t Waiting Time\t Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime, processes[i].burstTime, processes[i].finishTime, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    printf("Average Waiting Time: %.2f\n", avgWaitingTime);
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
}

void preemptiveFCFS(struct Process processes[], int n) {
    // Initialize the remaining burst time for each process (equal to burst time initially)
    for (int i = 0; i < n; i++) {
        processes[i].remainingBurstTime = processes[i].burstTime;
    }

    // Sort processes by arrival time to ensure they are executed in order of arrival
    sortByArrivalTime(processes, n);

    // Calculate and display average times
    calculateAverageTime(processes, n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter Arrival Time and Burst Time of P%d: ", p[i].id);
        scanf("%d %d", &p[i].arrivalTime, &p[i].burstTime);
    }

    preemptiveFCFS(p, n);

    return 0;
}
