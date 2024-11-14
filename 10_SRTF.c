#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;
    int bt;   // Burst Time
    int art;  // Arrival Time
};

void findWaitingTime(struct Process proc[], int n, int wt[], int ft[]) {
    int rt[n];  // Remaining Time
    for (int i = 0; i < n; i++)
        rt[i] = proc[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if (proc[j].art <= t && rt[j] < minm && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        rt[shortest]--;

        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = 0;

            finish_time = t + 1;
            ft[shortest] = finish_time;  // Store finish time for each process

            wt[shortest] = finish_time - proc[shortest].bt - proc[shortest].art;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        t++;
    }
}

void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = proc[i].bt + wt[i];
}

void findavgTime(struct Process proc[], int n) {
    int wt[n], tat[n], ft[n];  // Add finish time array
    int total_wt = 0, total_tat = 0;

    findWaitingTime(proc, n, wt, ft);  // Pass finish time array
    findTurnAroundTime(proc, n, wt, tat);

    printf("Processes\tBurst time\tArrival time\tWaiting time\tTurn around time\tFinish time\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", proc[i].pid, proc[i].bt, proc[i].art, wt[i], tat[i], ft[i]);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turn around time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d (PID, Burst Time, Arrival Time): ", i + 1);
        scanf("%d %d %d", &proc[i].pid, &proc[i].bt, &proc[i].art);
    }

    findavgTime(proc, n);
    return 0;
}
