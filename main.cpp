#include "ipv4.hpp"

#include <iostream>
#include <string>

// Keeps reading lines until END and prints the address found in each line.
int main() {
    std::string line;
    while (true) {
        // Read the whole line, including spaces.
        std::cout << "Enter a string (or 'END' to quit): " << std::flush;
        if (!std::getline(std::cin, line)) {
            break;
        }
        // The quit command must be exactly END.
        if (line == "END") {
            std::cout << "Program terminated.\n";
            break;
        }

        // Try to find an address in this line.
        unsigned long address = 0;
        int port = -1;
        if (!extractIPv4(line, address, port)) {
            std::cout << "Invalid input: no valid IPv4 address found\n";
            continue;
        }

        // Split the 32-bit number back into four octets for display.
        std::cout << "Extracted IPv4 address: "
                  << ((address >> 24) & 255UL) << '.'
                  << ((address >> 16) & 255UL) << '.'
                  << ((address >> 8) & 255UL) << '.'
                  << (address & 255UL)
                  << " (decimal value: " << address << ", port: ";
        // A port of -1 means the address did not include one.
        if (port == -1) {
            std::cout << "none";
        } else {
            std::cout << port;
        }
        std::cout << ")\n";
    }
    return 0;
}
