#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FileCopy {
    private:
        string sourcePath, destPath;
        
    public:
        FileCopy(string src, string dst) : sourcePath(src), destPath(dst) {}
        
        // Method to copy file character by character
        bool copyCharByChar() {
            ifstream sourceFile(sourcePath, ios::in);
            ofstream destFile(destPath, ios::out);
            
            if (!sourceFile || !destFile) {
                cout << "Error opening files!" << endl;
                return false;
            }
            
            char ch;
            int charCount = 0;
            
            cout << "\nCopying character by character...\n";
            cout << "Content being copied:\n";
            cout << "----------------------------------------\n";
            
            // Read and write character by character
            while (sourceFile.get(ch)) {
                destFile.put(ch);
                cout << ch;
                charCount++;
            }
            
            cout << "\n----------------------------------------\n";
            cout << "Total characters copied: " << charCount << endl;
            
            sourceFile.close();
            destFile.close();
            return true;
        }
        
        // Method to copy file line by line
        bool copyLineByLine() {
            ifstream sourceFile(sourcePath);
            ofstream destFile(destPath);
            
            if (!sourceFile || !destFile) {
                cout << "Error opening files!" << endl;
                return false;
            }
            
            string line;
            int lineCount = 0;
            
            cout << "\nCopying line by line...\n";
            cout << "Content being copied:\n";
            cout << "----------------------------------------\n";
            
            // Read and write line by line
            while (getline(sourceFile, line)) {
                destFile << line << endl;
                cout << "Line " << ++lineCount << ": " << line << endl;
            }
            
            cout << "----------------------------------------\n";
            cout << "Total lines copied: " << lineCount << endl;
            
            sourceFile.close();
            destFile.close();
            return true;
        }
        
        // Method to display file contents
        static void displayFile(string path) {
            ifstream file(path);
            
            if (!file) {
                cout << "Error opening file: " << path << endl;
                return;
            }
            
            string line;
            cout << "\nFile contents:\n";
            cout << "----------------------------------------\n";
            
            while (getline(file, line)) {
                cout << line << endl;
            }
            
            cout << "----------------------------------------\n";
            file.close();
        }
};

int main() {
    string source, dest;
    int choice;
    
    while (true) {
        cout << "\n=== File Copy Program ===\n";
        cout << "1. Copy character by character\n";
        cout << "2. Copy line by line\n";
        cout << "3. Display file contents\n";
        cout << "4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        if (choice == 4) {
            cout << "Exiting program...\n";
            break;
        }
        
        if (choice >= 1 && choice <= 3) {
            cout << "Enter source file path: ";
            getline(cin, source);
            
            if (choice <= 2) {
                cout << "Enter destination file path: ";
                getline(cin, dest);
                
                FileCopy copier(source, dest);
                
                if (choice == 1) {
                    copier.copyCharByChar();
                } else {
                    copier.copyLineByLine();
                }
                
                cout << "\nFile copied successfully!\n";
            } else {
                FileCopy::displayFile(source);
            }
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }
    
    return 0;
}
