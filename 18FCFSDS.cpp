#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class DiskScheduler {
    public:
    static pair<vector<int>, int> fcfs(vector<int>& requests, int head) {
        int total_seek = 0;
        int curr_track = head;
        vector<int> seek_sequence;
        
        // Simply process requests in order
        for(int track : requests) {
            seek_sequence.push_back(track);
            total_seek += abs(curr_track - track);
            curr_track = track;
        }
        
        return make_pair(seek_sequence, total_seek);
    }
    
    static void printTable(vector<int>& requests, int head, vector<int>& sequence, int total) {
        cout << "\n==================== FCFS Disk Scheduling ====================\n";
        cout << "\nInitial Head Position: " << head;
        cout << "\nTrack Requests: ";
        for(int req : requests) cout << req << " ";
        
        cout << "\n\nSeek Sequence:\n";
        cout << setw(10) << "Track" << setw(15) << "Seek Time" << setw(20) << "Cumulative\n";
        cout << "-------------------------------------------------------\n";
        
        int curr_head = head;
        int cumulative = 0;
        
        for(int track : sequence) {
            int seek_time = abs(curr_head - track);
            cumulative += seek_time;
            cout << setw(10) << track 
                 << setw(15) << seek_time 
                 << setw(20) << cumulative << endl;
            curr_head = track;
        }
        
        // Calculate average seek time
        double avg_seek_time = static_cast<double>(total) / requests.size();
        
        cout << "\nTotal Seek Time: " << total << " cylinders";
        cout << "\nAverage Seek Time: " << fixed << setprecision(2) << avg_seek_time << " cylinders";
        cout << "\n========================================================\n";
    }
};

int main() {
    // Sample input
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;
    
    // Process requests using FCFS
    pair<vector<int>, int> result = DiskScheduler::fcfs(requests, head);
    vector<int> sequence = result.first;
    int total = result.second;
    
    // Print results
    DiskScheduler::printTable(requests, head, sequence, total);
    
    // Allow user to input custom values
    char choice;
    cout << "\nWould you like to try with custom values? (y/n): ";
    cin >> choice;
    
    if(choice == 'y' || choice == 'Y') {
        int n, start_pos;
        cout << "\nEnter the number of requests: ";
        cin >> n;
        
        cout << "Enter the initial head position: ";
        cin >> start_pos;
        
        cout << "Enter the track requests:\n";
        vector<int> custom_requests(n);
        for(int i = 0; i < n; i++) {
            cout << "Request " << i+1 << ": ";
            cin >> custom_requests[i];
        }
        
        // Process custom requests
        result = DiskScheduler::fcfs(custom_requests, start_pos);
        sequence = result.first;
        total = result.second;
        
        // Print results for custom input
        DiskScheduler::printTable(custom_requests, start_pos, sequence, total);
    }
    
    return 0;
}