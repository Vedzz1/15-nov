#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

class Loader {
private:
    // Structure for symbol table entry
    struct SymbolEntry {
        string symbol;
        int address;
        bool isExternal;
    };
    
    // Structure for program section
    struct Section {
        string name;
        int start_address;
        int length;
        vector<string> code;
    };
    
    int starting_address;
    map<string, SymbolEntry> symbol_table;
    vector<Section> sections;
    vector<string> memory;
    
public:
    Loader() {
        starting_address = 0x1000;  // Default starting address
        memory.resize(1000, "XX");  // Initialize memory with XX
    }
    
    void loadProgram(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Error: Cannot open input file!" << endl;
            return;
        }
        
        cout << "\nLoading program from file: " << filename << endl;
        cout << "----------------------------------------\n";
        
        string line;
        Section current_section;
        bool in_section = false;
        int current_address = starting_address;
        
        while (getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == ';') continue;
            
            // Parse the line
            if (line[0] == '.') {  // Section declaration
                if (in_section) {
                    current_section.length = current_address - current_section.start_address;
                    sections.push_back(current_section);
                }
                
                current_section = Section();
                current_section.name = line.substr(1);
                current_section.start_address = current_address;
                in_section = true;
                
                cout << "Found section: " << current_section.name << 
                     " at address: " << hex << current_address << endl;
            }
            else if (line[0] == '@') {  // Symbol definition
                string symbol = line.substr(1, line.find(' ')-1);
                SymbolEntry entry = {symbol, current_address, false};
                symbol_table[symbol] = entry;
                
                cout << "Found symbol: " << symbol << 
                     " at address: " << hex << current_address << endl;
            }
            else {  // Code line
                if (in_section) {
                    current_section.code.push_back(line);
                    memory[current_address - starting_address] = line;
                    current_address++;
                }
            }
        }
        
        // Add last section
        if (in_section) {
            current_section.length = current_address - current_section.start_address;
            sections.push_back(current_section);
        }
        
        file.close();
        cout << "Program loaded successfully!\n";
    }
    
    void displayMemory() {
        cout << "\nMemory Contents:\n";
        cout << "----------------------------------------\n";
        cout << "Address  | Contents | Decoded\n";
        cout << "----------------------------------------\n";
        
        for (int i = 0; i < memory.size(); i++) {
            if (memory[i] != "XX") {
                cout << hex << uppercase << setw(8) << (starting_address + i) 
                     << " | " << setw(8) << memory[i] 
                     << " | " << decodeInstruction(memory[i]) << endl;
            }
        }
    }
    
    void displaySymbolTable() {
        cout << "\nSymbol Table:\n";
        cout << "----------------------------------------\n";
        cout << "Symbol  | Address | External\n";
        cout << "----------------------------------------\n";
        
        for (const auto& entry : symbol_table) {
            cout << left << setw(8) << entry.first << " | " 
                 << hex << uppercase << setw(7) << entry.second.address 
                 << " | " << (entry.second.isExternal ? "Yes" : "No") << endl;
        }
    }
    
    void displaySections() {
        cout << "\nProgram Sections:\n";
        cout << "----------------------------------------\n";
        cout << "Name    | Start   | Length\n";
        cout << "----------------------------------------\n";
        
        for (const auto& section : sections) {
            cout << left << setw(8) << section.name << " | " 
                 << hex << uppercase << setw(7) << section.start_address 
                 << " | " << dec << section.length << endl;
        }
    }
    
private:
    string decodeInstruction(const string& instruction) {
        // Simple instruction decoder - can be expanded
        if (instruction[0] == 'M') return "MOV";
        if (instruction[0] == 'A') return "ADD";
        if (instruction[0] == 'S') return "SUB";
        if (instruction[0] == 'J') return "JMP";
        return instruction;
    }
};

int main() {
    Loader loader;
    string filename;
    int choice;
    
    while (true) {
        cout << "\n=== ALP Program Loader ===\n";
        cout << "1. Load Program\n";
        cout << "2. Display Memory Contents\n";
        cout << "3. Display Symbol Table\n";
        cout << "4. Display Sections\n";
        cout << "5. Exit\n";
        cout << "Enter choice (1-5): ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Enter input file name: ";
                cin >> filename;
                loader.loadProgram(filename);
                break;
            
            case 2:
                loader.displayMemory();
                break;
            
            case 3:
                loader.displaySymbolTable();
                break;
            
            case 4:
                loader.displaySections();
                break;
            
            case 5:
                cout << "Exiting program...\n";
                return 0;
            
            default:
                cout << "Invalid choice!\n";
        }
    }
    
    return 0;
}
