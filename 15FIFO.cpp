#include <iostream>
using namespace std;

void fifo(int refStr[], int n, int frames[], int totalRef) {
    int faults = 0, current = 0;
    cout << "\nFIFO Page Replacement\n";
    for (int i = 0; i < totalRef; i++) {
        bool found = false;
        for (int j = 0; j < n; j++) {
            if (frames[j] == refStr[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            frames[current] = refStr[i];
            current = (current + 1) % n;
            faults++;
        }
        cout << "Page frame after accessing " << refStr[i] << ": ";
        for (int k = 0; k < n; k++) {
            cout << frames[k] << " ";
        }
        cout << endl;
    }
    cout << "Total Page Faults in FIFO: " << faults << endl;
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
    fifo(refStr, n, frames, totalRef);

    return 0;
}
