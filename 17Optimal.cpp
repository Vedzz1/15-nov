#include <iostream>
using namespace std;

int findOptimal(int refStr[], int frames[], int index, int totalRef, int n) {
    int pos = -1, farthest = index;
    for (int i = 0; i < n; i++) {
        int j;
        for (j = index; j < totalRef; j++) {
            if (frames[i] == refStr[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == totalRef) {
            return i;
        }
    }
    return pos == -1 ? 0 : pos;
}

void optimal(int refStr[], int n, int frames[], int totalRef) {
    int faults = 0;
    cout << "\nOptimal Page Replacement\n";
    for (int i = 0; i < totalRef; i++) {
        bool found = false;
        for (int j = 0; j < n; j++) {
            if (frames[j] == refStr[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            int pos = findOptimal(refStr, frames, i + 1, totalRef, n);
            frames[pos] = refStr[i];
            faults++;
        }
        cout << "Page frame after accessing " << refStr[i] << ": ";
        for (int k = 0; k < n; k++) {
            cout << frames[k] << " ";
        }
        cout << endl;
    }
    cout << "Total Page Faults in Optimal: " << faults << endl;
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
    optimal(refStr, n, frames, totalRef);

    return 0;
}
