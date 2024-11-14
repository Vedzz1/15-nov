#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <iomanip>
using namespace std;

class DiskScheduler {
    public:
    // Function to find the request with minimum seek time
    static int findClosest(vector<int>& requests, vector<bool>& visited, int curr_track) {
        int min_diff = INT_MAX;
        int min_index = -1;
        
        for(int i = 0; i < requests.size(); i++) {
            if(!visited[i]) {
                int diff = abs(curr_track - requests[i]);
                if(diff < min_diff) {
                    min_diff = diff;
                    min_index = i;
                }
            }
        }
        return min_index;
    }
    
    static pair<vector<int>, int> sstf(vector<int>& requests, int head) {
        int total_seek = 0;
        int curr_track = head;
        vector<int> seek_sequence;
        vector<bool> visited(requests.size(), false);
        
        // Process all requests
        for(int i = 0; i < requests.size(); i++) {
            // Find closest unserviced request
            int next = findClosest(requests, visited, curr_track);
            if(next == -1) break;  // No more unvisited requests
            
            // Add to seek sequence
            seek_sequence.push_back(requests[next]);
            
            // Calculate seek time
            total_seek += abs(curr_track - requests[next]);
            
            // Update current track and mark as visited
            curr_track = requests[next];
            visited[next] = true;
        }
        
        return make_pair(seek_sequence, total_seek);
    }
    
    static void printTable(vector<int>& requests, int head, vector<int>& sequence, int total) {
        cout << "\n==================== SSTF Disk Scheduling ====================\n";
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
    // Sample input
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;
    
    // Get result
    pair<vector<int>, int> result = DiskScheduler::sstf(requests, head);
    vector<int> sequence = result.first;
    int total = result.second;
    
    // Print table
    DiskScheduler::printTable(requests, head, sequence, total);
    
    return 0;
}