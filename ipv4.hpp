#ifndef IPV4_HPP
#define IPV4_HPP

#include <string>

// Returns true if a valid address was found. On failure, resets both outputs.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);

#endif
