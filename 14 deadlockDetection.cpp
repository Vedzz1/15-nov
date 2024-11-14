#include <iostream>
#include <vector>

using namespace std;

// Function to perform deadlock detection
bool deadlockDetection(int numProcesses, int numResources, vector<int> available, vector<vector<int>> allocation, vector<vector<int>> request) {
    vector<int> work = available;
    vector<bool> finish(numProcesses, false);
    vector<int> deadlockedProcesses;

    // Attempt to find an order of processes that can complete
    bool found;
    do {
        found = false;
        for (int i = 0; i < numProcesses; ++i) {
            if (!finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < numResources; ++j) {
                    if (request[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int j = 0; j < numResources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }
    } while (found);

    // Check for any unfinished processes
    bool isDeadlock = false;
    for (int i = 0; i < numProcesses; ++i) {
        if (!finish[i]) {
            isDeadlock = true;
            deadlockedProcesses.push_back(i);
        }
    }

    // Output results
    if (isDeadlock) {
        cout << "Deadlock detected. Deadlocked processes: ";
        for (int i : deadlockedProcesses) {
            cout << "P" << i << " ";
        }
        cout << endl;
    } else {
        cout << "No deadlock detected. All processes can complete." << endl;
    }

    return isDeadlock;
}

int main() {
    // Example of input data
    int numProcesses = 5;
    int numResources = 3;

    vector<int> available = {3, 3, 2}; // Available resources

    vector<vector<int>> allocation = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    vector<vector<int>> request = {
        {0, 0, 0},
        {2, 0, 2},
        {0, 0, 0},
        {1, 0, 0},
        {0, 0, 2}
    };

    deadlockDetection(numProcesses, numResources, available, allocation, request);

    return 0;
}
