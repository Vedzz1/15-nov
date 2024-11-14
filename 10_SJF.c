#include <stdio.h>

int main() {
    int at[10], bt[10], pr[10];
    int n, i, j, temp, time = 0, count, over = 0, sum_wait = 0, sum_turnaround = 0, start;
    float avgwait, avgturn;

    // Input the number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input arrival and burst times for each process
    for (i = 0; i < n; i++) {
        printf("Enter the arrival time and execution time for process %d: ", i + 1);
        scanf("%d%d", &at[i], &bt[i]);
        pr[i] = i + 1;
    }

    // Sort processes by arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (at[i] > at[j]) {
                // Swap arrival time, burst time, and process ID
                temp = at[i]; at[i] = at[j]; at[j] = temp;
                temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
                temp = pr[i]; pr[i] = pr[j]; pr[j] = temp;
            }
        }
    }

    // Output header for the process table
    printf("\n\nProcess\t|Arrival time\t|Execution time\t|Start time\t|Finish time\t|Waiting time\t|Turnaround time\n\n");

    // Process the tasks
    while (over < n) {
        count = 0;
        for (i = over; i < n; i++) {
            if (at[i] <= time) count++;
            else break;
        }

        // If multiple processes have arrived, sort by burst time (for SJF logic)
        if (count > 1) {
            for (i = over; i < over + count - 1; i++) {
                for (j = i + 1; j < over + count; j++) {
                    if (bt[i] > bt[j]) {
                        // Swap based on burst time
                        temp = at[i]; at[i] = at[j]; at[j] = temp;
                        temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
                        temp = pr[i]; pr[i] = pr[j]; pr[j] = temp;
                    }
                }
            }
        }

        // Start the next process
        start = time;

        // If no process has arrived yet, advance time to the next arrival
        if (at[over] > time) {
            time = at[over];
        }

        // Calculate finish time (end time for the current process)
        time += bt[over];
        int finishTime = time;

        // Calculate waiting time and turnaround time
        int waitingTime = start - at[over];
        if (waitingTime < 0) waitingTime = 0; // Fix negative waiting time if the process starts as soon as it arrives
        int turnaroundTime = finishTime - at[over];

        // Output process details, including finish time
        printf("P[%d]\t|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", pr[over], at[over], bt[over], start, finishTime, waitingTime, turnaroundTime);

        // Sum the waiting time and turnaround time for averages
        sum_wait += waitingTime;
        sum_turnaround += turnaroundTime;

        // Move to the next process
        over++;
    }

    // Calculate and print average waiting time and turnaround time
    avgwait = (float)sum_wait / n;
    avgturn = (float)sum_turnaround / n;

    printf("Average waiting time is %.2f\n", avgwait);
    printf("Average turnaround time is %.2f\n", avgturn);

    return 0;
}
