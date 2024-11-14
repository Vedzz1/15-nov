#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

class DiskScheduler {
    public:
    static pair<vector<int>, int> scan(vector<int>& requests, int head, int disk_size) {
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
        
        // Process tracks on right and move to end
        for(int track : right) {
            total_seek += abs(track - head);
            head = track;
            seek_sequence.push_back(track);
        }
        
        // Process tracks on left
        for(int i = left.size()-1; i >= 0; i--) {
            total_seek += abs(left[i] - head);
            head = left[i];
            seek_sequence.push_back(left[i]);
        }
        
        return make_pair(seek_sequence, total_seek);
    }
    
    static void printTable(vector<int>& requests, int head, vector<int>& sequence, int total) {
        cout << "\n==================== SCAN Disk Scheduling ====================\n";
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
        cout << "========================================================\n";
    }
};

int main() {
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;
    int disk_size = 200;
    
    // Get the result without structured binding
    pair<vector<int>, int> result = DiskScheduler::scan(requests, head, disk_size);
    vector<int> sequence = result.first;
    int total = result.second;
    
    DiskScheduler::printTable(requests, head, sequence, total);
    
    return 0;
}