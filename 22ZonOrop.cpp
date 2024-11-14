#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

void createZombieProcess() {
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process: Sleep to keep parent alive while child becomes a zombie.
        sleep(5);
        cout << "Parent process (Zombie demonstration)." << endl;
    } else if (pid == 0) {
        // Child process: Exit immediately to become a zombie.
        cout << "Child process exiting to become a zombie." << endl;
        exit(0);
    } else {
        cerr << "Fork failed" << endl;
    }
}

void createOrphanProcess() {
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process: Exiting immediately, leaving the child orphaned.
        cout << "Parent process exiting (Orphan demonstration)." << endl;
        exit(0);
    } else if (pid == 0) {
        // Child process: Sleep to keep it running after parent exit.
        sleep(3);
        cout << "Orphan child process with PID " << getpid() << " and PPID " << getppid() << endl;
    } else {
        cerr << "Fork failed" << endl;
    }
}

void sumEvenOddInParentChild() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = sizeof(arr) / sizeof(arr[0]);
    int even_sum = 0, odd_sum = 0;
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process: Sum even numbers
        for (int i = 0; i < n; i++) {
            if (arr[i] % 2 == 0) {
                even_sum += arr[i];
            }
        }
        wait(NULL);  // Wait for child to complete
        cout << "Sum of even numbers in parent process: " << even_sum << endl;
    } else if (pid == 0) {
        // Child process: Sum odd numbers
        for (int i = 0; i < n; i++) {
            if (arr[i] % 2 != 0) {
                odd_sum += arr[i];
            }
        }
        cout << "Sum of odd numbers in child process: " << odd_sum << endl;
        exit(0);
    } else {
        cerr << "Fork failed" << endl;
    }
}

int main() {
    cout << "Demonstrating Zombie Process:" << endl;
    createZombieProcess();
    sleep(8); // Ensure the zombie process demonstration completes

    cout << "\nDemonstrating Orphan Process:" << endl;
    createOrphanProcess();
    sleep(4); // Ensure the orphan process demonstration completes

    cout << "\nSumming Even and Odd Numbers with Parent and Child Processes:" << endl;
    sumEvenOddInParentChild();

    return 0;
}
