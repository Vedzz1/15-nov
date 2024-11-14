#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

class DiskScheduler {
public:
    static pair<vector<int>, int> cscan(vector<int>& requests, int head, int disk_size) {
        vector<int> left, right;
        vector<int> seek_sequence;
        int total_seek = 0;

        // Separate tracks on left and right of head
        for(int req : requests) {
            if(req < head) left.push_back(req);
            else right.push_back(req);
        }

        // Sort both vectors
        sort(left.begin(), left.end());
        sort(right.begin(), right.end());

        // Process tracks on right
        for(int track : right) {
            total_seek += abs(track - head);
            head = track;
            seek_sequence.push_back(track);
        }

        // Move to the end of the disk
        total_seek += disk_size - 1 - head;
        head = disk_size - 1;
        seek_sequence.push_back(head);

        // Process tracks on left
        for(int track : left) {
            total_seek += abs(track - head);
            head = track;
            seek_sequence.push_back(track);
        }

        // Move to the beginning of the disk
        total_seek += head;
        head = 0;
        seek_sequence.push_back(head);

        return make_pair(seek_sequence, total_seek);
    }

    static void printTable(vector<int>& requests, int head, vector<int>& sequence, int total) {
        cout << "\n==================== C-SCAN Disk Scheduling ====================\n";
        cout << "\nInitial Head Position: " << head;
        cout << "\nTrack Requests: ";
        for(int req : requests) cout << req << " ";

        cout << "\n\nSeek Sequence:\n";
        cout << setw(10) << "Track" << setw(15) << "Seek Time\n";
        cout << "------------------------------------------------\n";

        int curr_head = head;
        for(int track : sequence) {
            cout << setw(10) << track << setw(15) << abs(curr_head - track) << endl;
            curr_head = track;
        }

        cout << "\nTotal Seek Time: " << total << " cylinders\n";
        cout << "================================================================\n";
    }
};

int main() {
    // Sample input
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;
    int disk_size = 200;

    // Get result
    pair<vector<int>, int> result = DiskScheduler::cscan(requests, head, disk_size);
    vector<int> sequence = result.first;
    int total = result.second;

    // Print table
    DiskScheduler::printTable(requests, head, sequence, total);

    return 0;
}