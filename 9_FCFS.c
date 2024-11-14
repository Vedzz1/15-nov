#include <stdio.h>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
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

void calculateWaitingTime(struct Process processes[], int n, int waitingTime[]) {
    int total = 0;
    int currentTime = 0;

    waitingTime[0] = 0; // First process has no waiting time

    for (int i = 1; i < n; i++) {
        currentTime = processes[i - 1].arrivalTime + processes[i - 1].burstTime + waitingTime[i - 1];
        
        // Waiting time is current time minus arrival time of process
        waitingTime[i] = currentTime - processes[i].arrivalTime;

        // If the process arrives after the current time, it won't wait
        if (waitingTime[i] < 0)
            waitingTime[i] = 0;
    }
}

void calculateTurnaroundTime(struct Process processes[], int n, int waitingTime[], int turnaroundTime[]) {
    for (int i = 0; i < n; i++) {
        turnaroundTime[i] = processes[i].burstTime + waitingTime[i];
    }
}

void calculateFinishTime(struct Process processes[], int n, int turnaroundTime[], int finishTime[]) {
    for (int i = 0; i < n; i++) {
        finishTime[i] = processes[i].arrivalTime + turnaroundTime[i];
    }
}

void calculateAverageTime(struct Process processes[], int n) {
    int waitingTime[n], turnaroundTime[n], finishTime[n];

    calculateWaitingTime(processes, n, waitingTime);
    calculateTurnaroundTime(processes, n, waitingTime, turnaroundTime);
    calculateFinishTime(processes, n, turnaroundTime, finishTime);

    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    float avgTurnaroundTime = (float)totalTurnaroundTime / n;

    printf("\nProcess\t Arrival Time\t Burst Time\t Finish Time\t Waiting Time\t Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrivalTime, processes[i].burstTime, finishTime[i], waitingTime[i], turnaroundTime[i]);
    }

    printf("Average Waiting Time: %.2f\n", avgWaitingTime);
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
}

void FCFS(struct Process processes[], int n) {
    sortByArrivalTime(processes, n);
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

    FCFS(p, n);

    return 0;
}
