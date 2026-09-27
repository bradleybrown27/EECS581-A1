#include <iostream>
#include <string>

namespace {
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isTokenChar(char c) {
    return isDigit(c) || c == '.' || c == ':';
}

// Read one decimal field by hand, staying inside [pos, end).
bool readNumber(const std::string& text, std::size_t& pos, std::size_t end,
                unsigned int limit, std::size_t maxDigits,
                unsigned int& value) {
    const std::size_t start = pos;
    value = 0;
    while (pos < end && isDigit(text[pos])) {
        const unsigned int digit = static_cast<unsigned int>(text[pos] - '0');
        // Check BEFORE multiplying to prevent overflow, even for huge inputs.
        if (pos - start >= maxDigits || value > limit / 10 ||
            (value == limit / 10 && digit > limit % 10)) {
            return false;
        }
        value = value * 10 + digit;
        ++pos;
    }
    if (pos == start) {
        return false; // Empty fields are invalid.
    }
    // Zero must be a single '0'; reject 00, 01, 00080, etc.
    return pos - start == 1 || text[start] != '0';
}

bool parseCandidate(const std::string& text, std::size_t begin, std::size_t end,
                    unsigned long& address, int& port) {
    std::size_t pos = begin;
    unsigned long packed = 0;
    int parsedPort = -1;
    for (int i = 0; i < 4; ++i) {
        unsigned int octet = 0;
        if (!readNumber(text, pos, end, 255, 3, octet)) {
            return false;
        }
        // Append one base-256 digit: A*256^3 + B*256^2 + C*256 + D.
        // unsigned long holds at least 32 bits, enough for all four octets.
        packed = packed * 256UL + octet;
        if (i < 3) {
            if (pos == end || text[pos] != '.') {
                return false;
            }
            ++pos;
        }
    }
    if (pos < end && text[pos] == ':') {
        ++pos;
        unsigned int number = 0;
        if (!readNumber(text, pos, end, 65535, 5, number)) {
            return false; // An invalid port invalidates the whole address.
        }
        parsedPort = static_cast<int>(number); // Port zero is valid.
    }
    if (pos != end) {
        return false; // Reject leftover dots, colons, digits, and extra fields.
    }
    // Publish results only after the complete candidate passes validation.
    address = packed;
    port = parsedPort;
    return true;
}
} // namespace

// Required interface. On failure, outputs are always 0 and -1.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;
    std::size_t pos = 0;
    while (pos < str.size()) {
        // Only digits, periods, and colons belong to a candidate.
        while (pos < str.size() && !isTokenChar(str[pos])) {
            ++pos;
        }
        if (pos == str.size()) {
            break;
        }
        const std::size_t begin = pos;
        // Include leading punctuation and validate the ENTIRE maximal run.
        // Never retry inside :1.2.3.4, 9991.2.3.4, or 1.2.3.4.5.
        while (pos < str.size() && isTokenChar(str[pos])) {
            ++pos;
        }
        unsigned long candidateAddress = 0;
        int candidatePort = -1;
        if (parseCandidate(str, begin, pos, candidateAddress, candidatePort)) {
            outAddress = candidateAddress;
            outPort = candidatePort;
            return true; // At most one valid address per input line.
        }
    }
    return false;
}

// Separate test cases call the same required function that main uses.
#include "ipv4_tests.h"

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--test") {
        return runTests();
    }
    std::string line;
    while (true) {
        std::cout << "Enter a string (or 'END' to quit): " << std::flush;
        if (!std::getline(std::cin, line)) {
            break; // EOF also exits cleanly.
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
        // Recover octets from the returned numeric value, highest byte first.
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
}

