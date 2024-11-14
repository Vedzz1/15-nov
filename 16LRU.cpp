#include <iostream>
using namespace std;

int findLRU(int time[], int n) {
    int minimum = time[0], pos = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] < minimum) {
            minimum = time[i];
            pos = i;
        }
    }
    return pos;
}

void lru(int refStr[], int n, int frames[], int totalRef) {
    int faults = 0, time[10], counter = 0;
    cout << "\nLRU Page Replacement\n";
    for (int i = 0; i < totalRef; i++) {
        bool found = false;
        for (int j = 0; j < n; j++) {
            if (frames[j] == refStr[i]) {
                counter++;
                time[j] = counter;
                found = true;
                break;
            }
        }
        if (!found) {
            int pos = findLRU(time, n);
            frames[pos] = refStr[i];
            counter++;
            time[pos] = counter;
            faults++;
        }
        cout << "Page frame after accessing " << refStr[i] << ": ";
        for (int k = 0; k < n; k++) {
            cout << frames[k] << " ";
        }
        cout << endl;
    }
    cout << "Total Page Faults in LRU: " << faults << endl;
}

int main() {
    int n, totalRef;

    cout << "Enter the number of frames: ";
    cin >> n;

    cout << "Enter the length of the reference string: ";
    cin >> totalRef;

    int refStr[totalRef];
    int frames[n];

    cout << "Enter the reference string: ";
    for (int i = 0; i < totalRef; i++) {
        cin >> refStr[i];
    }

    fill(frames, frames + n, -1);
    lru(refStr, n, frames, totalRef);

    return 0;
}
