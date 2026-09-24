#include "ipv4.hpp"

#include <cstddef>

namespace {

// Checks if this character is a digit from 0 to 9.
bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

// Checks if this character can be part of an IPv4 address or port.
bool isTokenCharacter(char ch) {
    return isDigit(ch) || ch == '.' || ch == ':';
}

// Reads one number digit by digit and checks its size and value.
bool readNumber(const std::string& text, std::size_t& position,
                std::size_t end, int maxDigits, int maxValue, int& value) {
    const std::size_t start = position;
    value = 0;

    while (position < end && isDigit(text[position])) {
        if (position - start >= static_cast<std::size_t>(maxDigits)) {
            return false;
        }
        value = value * 10 + (text[position] - '0');
        if (value > maxValue) {
            return false;
        }
        ++position;
    }

    return position != start &&
           (position - start == 1 || text[start] != '0');
}

// Checks one complete token for four octets and an optional port.
bool parseToken(const std::string& text, std::size_t start, std::size_t end,
                unsigned long& address, int& port) {
    std::size_t position = start;
    unsigned long parsedAddress = 0;

    for (int octetIndex = 0; octetIndex < 4; ++octetIndex) {
        int octet = 0;
        if (!readNumber(text, position, end, 3, 255, octet)) {
            return false;
        }
        parsedAddress = (parsedAddress << 8) | static_cast<unsigned long>(octet);

        if (octetIndex < 3) {
            if (position == end || text[position] != '.') {
                return false;
            }
            ++position;
        }
    }

    int parsedPort = -1;
    if (position < end && text[position] == ':') {
        ++position;
        if (!readNumber(text, position, end, 5, 65535, parsedPort)) {
            return false;
        }
    }

    if (position != end) {
        return false;
    }

    address = parsedAddress;
    port = parsedPort;
    return true;
}

}  // namespace

// Searches the line for a valid IPv4 token and sets the output values.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    std::size_t position = 0;
    while (position < str.size()) {
        if (!isTokenCharacter(str[position])) {
            ++position;
            continue;
        }

        const std::size_t start = position;
        while (position < str.size() && isTokenCharacter(str[position])) {
            ++position;
        }

        if (parseToken(str, start, position, outAddress, outPort)) {
            return true;
        }
    }

    return false;
}
