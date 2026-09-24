#include "ipv4.hpp"

#include <iostream>
#include <string>

// Keeps reading lines until END and prints the address found in each line.
int main() {
    std::string line;
    while (true) {
        std::cout << "Enter a string (or 'END' to quit): " << std::flush;
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line == "END") {
            std::cout << "Program terminated.\n";
            break;
        }

        unsigned long address = 0;
        int port = -1;
        if (!extractIPv4(line, address, port)) {
            std::cout << "Invalid input: no valid IPv4 address found\n";
            continue;
        }

        std::cout << "Extracted IPv4 address: "
                  << ((address >> 24) & 255UL) << '.'
                  << ((address >> 16) & 255UL) << '.'
                  << ((address >> 8) & 255UL) << '.'
                  << (address & 255UL)
                  << " (decimal value: " << address << ", port: ";
        if (port == -1) {
            std::cout << "none";
        } else {
            std::cout << port;
        }
        std::cout << ")\n";
    }
    return 0;
}
