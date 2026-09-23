#include "../ipv4.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Case {
    std::string input;
    bool found;
    unsigned long address;
    int port;
};

int main() {
    const std::vector<Case> cases = {
        {"", false, 0, -1},
        {"no number here", false, 0, -1},
        {"0.0.0.0", true, 0UL, -1},
        {"255.255.255.255:65535", true, 4294967295UL, 65535},
        {"connecting to 192.168.1.1 now", true, 3232235777UL, -1},
        {"server=10.0.0.255:8080end", true, 167772415UL, 8080},
        {"192a168.1.1.1", true, 2818638081UL, -1},
        {"1.2.3.4:0", true, 16909060UL, 0},
        {"1.2.3.4:65535", true, 16909060UL, 65535},
        {"x1.2.3.4x", true, 16909060UL, -1},
        {"ENDx 1.2.3.4", true, 16909060UL, -1},
        {"1.2.3.4 5.6.7.8", true, 16909060UL, -1},
        {"1.2.3.4. then 5.6.7.8", true, 84281096UL, -1},
        {"192.168.01.1", false, 0, -1},
        {"01.2.3.4", false, 0, -1},
        {"1.2.3.04", false, 0, -1},
        {"1.2.3.4:080", false, 0, -1},
        {"256.2.3.4", false, 0, -1},
        {"1.2.3.256", false, 0, -1},
        {"1.2.3.4:65536", false, 0, -1},
        {"1.2.3.4:99999", false, 0, -1},
        {"1.2.3.4:", false, 0, -1},
        {"1.2.3.4::80", false, 0, -1},
        {"1.2.3.4:80:90", false, 0, -1},
        {":1.2.3.4", false, 0, -1},
        {"1.2.3.4.", false, 0, -1},
        {"1.2.3.4.5", false, 0, -1},
        {"1234.2.3.4", false, 0, -1},
        {"1.2.3.400000000", false, 0, -1},
        {"1.2.3.4:123456", false, 0, -1},
        {"1..2.3.4", false, 0, -1},
        {"1.2.3", false, 0, -1},
        {"1.2.3.4:8.0", false, 0, -1},
        {"1.2.3.4:00", false, 0, -1},
        {"1.2.3.4a5", true, 16909060UL, -1},
    };

    for (std::size_t i = 0; i < cases.size(); ++i) {
        unsigned long address = 12345UL;
        int port = 12345;
        const bool found = extractIPv4(cases[i].input, address, port);
        if (found != cases[i].found || address != cases[i].address ||
            port != cases[i].port) {
            std::cerr << "Case " << i << " failed: " << cases[i].input
                      << " (found=" << found << ", address=" << address
                      << ", port=" << port << ")\n";
            return 1;
        }
    }

    // Sweep the full representable port range and the next invalid values.
    for (int candidate = 0; candidate <= 99999; ++candidate) {
        unsigned long address = 999UL;
        int port = 999;
        const bool found = extractIPv4(
            "1.2.3.4:" + std::to_string(candidate), address, port);
        const bool expected = candidate <= 65535;
        if (found != expected || address != (expected ? 16909060UL : 0UL) ||
            port != (expected ? candidate : -1)) {
            std::cerr << "Port boundary check failed for " << candidate << '\n';
            return 1;
        }
    }

    // Each octet is checked independently at every value from 0 through 999.
    for (int field = 0; field < 4; ++field) {
        for (int candidate = 0; candidate <= 999; ++candidate) {
            const int octets[4] = {
                field == 0 ? candidate : 1,
                field == 1 ? candidate : 2,
                field == 2 ? candidate : 3,
                field == 3 ? candidate : 4,
            };
            const std::string input = std::to_string(octets[0]) + '.' +
                                      std::to_string(octets[1]) + '.' +
                                      std::to_string(octets[2]) + '.' +
                                      std::to_string(octets[3]);
            unsigned long address = 999UL;
            int port = 999;
            const bool found = extractIPv4(input, address, port);
            const bool expected = candidate <= 255;
            const unsigned long expectedAddress = expected
                ? static_cast<unsigned long>(octets[0]) * 16777216UL +
                  static_cast<unsigned long>(octets[1]) * 65536UL +
                  static_cast<unsigned long>(octets[2]) * 256UL +
                  static_cast<unsigned long>(octets[3])
                : 0UL;
            if (found != expected || address != expectedAddress || port != -1) {
                std::cerr << "Octet boundary check failed for " << input << '\n';
                return 1;
            }
        }
    }

    std::cout << "Passed " << cases.size() << " extraction cases, 100000 port "
              << "checks, and 4000 octet checks.\n";
    return 0;
}
