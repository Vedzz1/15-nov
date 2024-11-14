#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <bitset>

class InterruptHandler {
private:
    struct InterruptInfo {
        uint8_t vector;
        std::string message;
    };

    std::map<std::string, InterruptInfo> interruptTable = {
        {"ILLEGAL_OPCODE", {0x01, "Invalid opcode detected"}},
        {"DIVISION_BY_ZERO", {0x02, "Division by zero attempted"}},
        {"MEMORY_ERROR", {0x03, "Memory access violation"}}
    };

public:
    void handleInterrupt(const std::string& type, uint16_t address) {
        auto it = interruptTable.find(type);
        if (it != interruptTable.end()) {
            std::cout << "\nINTERRUPT RAISED!" << std::endl;
            std::cout << "Type: " << type << std::endl;
            std::cout << "Vector: 0x" << std::hex << std::setw(2) 
                     << std::setfill('0') << static_cast<int>(it->second.vector) << std::endl;
            std::cout << "At instruction address: 0x" << std::setw(4) 
                     << std::setfill('0') << address << std::endl;
            std::cout << "Message: " << it->second.message << std::endl;
            std::cout << std::dec; // Reset to decimal output
        }
    }
};

class CPU {
private:
    std::map<std::string, std::string> validOpcodes = {
        {"ADD",   "0000"},
        {"SUB",   "0001"},
        {"MUL",   "0010"},
        {"DIV",   "0011"},
        {"LOAD",  "0100"},
        {"STORE", "0101"},
        {"JMP",   "0110"},
        {"HALT",  "1111"}
    };

    uint16_t programCounter;
    bool interruptFlag;
    InterruptHandler interruptHandler;

    bool isValidOpcode(const std::string& opcode) {
        for (const auto& pair : validOpcodes) {
            if (pair.second == opcode) {
                return true;
            }
        }
        return false;
    }

public:
    CPU() : programCounter(0), interruptFlag(false) {}

    bool checkOpcode(const std::string& instruction) {
        try {
            // Extract opcode (first 4 bits)
            std::string opcode = instruction.substr(0, 4);
            
            if (!isValidOpcode(opcode)) {
                raiseInterrupt("ILLEGAL_OPCODE");
                return false;
            }
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing instruction: " << e.what() << std::endl;
            raiseInterrupt("ILLEGAL_OPCODE");
            return false;
        }
    }

    void raiseInterrupt(const std::string& type) {
        interruptFlag = true;
        uint16_t savedPC = programCounter;
        interruptHandler.handleInterrupt(type, savedPC);
        interruptFlag = false;
    }

    void executeJob(const std::vector<std::string>& instructions) {
        std::cout << "Starting job execution...\n" << std::endl;

        for (const auto& instruction : instructions) {
            std::cout << "Processing instruction: " << instruction << std::endl;

            // Verify instruction format
            if (instruction.length() != 16) {
                std::cout << "Invalid instruction length" << std::endl;
                raiseInterrupt("ILLEGAL_OPCODE");
                break;
            }

            // Check if instruction contains only binary digits
            if (instruction.find_first_not_of("01") != std::string::npos) {
                std::cout << "Invalid instruction format - non-binary characters" << std::endl;
                raiseInterrupt("ILLEGAL_OPCODE");
                break;
            }

            if (!checkOpcode(instruction)) {
                std::cout << "Job terminated due to illegal opcode" << std::endl;
                break;
            }

            programCounter++;
        }

        std::cout << "\nJob execution completed" << std::endl;
    }

    // Utility function to convert assembly to binary instruction
    std::string assembleToBinary(const std::string& opcode, uint16_t operand) {
        auto it = validOpcodes.find(opcode);
        if (it == validOpcodes.end()) {
            throw std::runtime_error("Invalid opcode");
        }

        std::string binaryOpcode = it->second;
        std::bitset<12> binaryOperand(operand); // 12 bits for operand
        return binaryOpcode + binaryOperand.to_string();
    }
};

int main() {
    CPU cpu;
    
    // Test with some instructions (some valid, some invalid)
    std::vector<std::string> testInstructions = {
        "0000101010101010",  // ADD (valid)
        "0001101010101010",  // SUB (valid)
        "1010101010101010",  // Invalid opcode
        "0010101010101010"   // MUL (valid but won't execute due to previous error)
    };

    cpu.executeJob(testInstructions);

    // Example of using the assembler utility
    try {
        std::cout << "\nAssembling instruction 'ADD 42':" << std::endl;
        std::string assembledInstruction = cpu.assembleToBinary("ADD", 42);
        std::cout << "Binary: " << assembledInstruction << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Assembly error: " << e.what() << std::endl;
    }

    return 0;
}
