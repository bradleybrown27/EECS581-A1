#ifndef IPV4_TESTS_H
#define IPV4_TESTS_H

// Included after extractIPv4 in ipv4_finder.cpp.
// Expected values are constants, independent of the parser implementation.
// Signs before an address are garbage delimiters, as required by the grammar.
int runTests() {
    struct TestCase {
        const char* input;
        bool found;
        unsigned long address;
        int port;
    };
    const TestCase tests[] = {
        {"connecting to 192.168.1.1 now", true, 3232235777UL, -1},
        {"server=10.0.0.255:8080end", true, 167772415UL, 8080},
        {"192a168.1.1.1", true, 2818638081UL, -1},
        {"Connection from 192.168.1.1 refused", true, 3232235777UL, -1},
        {"1.2.3.4", true, 16909060UL, -1},
        {"junk[1.2.3.4:80]tail", true, 16909060UL, 80},
        {"abc1.2.3.4xyz", true, 16909060UL, -1},
        {"0.0.0.0", true, 0UL, -1},
        {"0.0.0.0:0", true, 0UL, 0},
        {"255.255.255.255", true, 4294967295UL, -1},
        {"255.255.255.255:65535", true, 4294967295UL, 65535},
        {"1.2.3.4:0", true, 16909060UL, 0},
        {"1.2.3.4:1", true, 16909060UL, 1},
        {"1.2.3.4:65535", true, 16909060UL, 65535},
        {"IP: 127.0.0.1", true, 2130706433UL, -1},
        {"-1.2.3.4", true, 16909060UL, -1},
        {"+1.2.3.4", true, 16909060UL, -1},
        {"1.2.3.4:80junk", true, 16909060UL, 80},
        {"bad 999.2.3.4, good 8.8.8.8:53", true, 134744072UL, 53},
        {"1.2.3.4:65536 127.0.0.1", true, 2130706433UL, -1},
        {":1.2.3.4 8.8.8.8:53", true, 134744072UL, 53},
        {"1.2.3.4 and 8.8.8.8:53", true, 16909060UL, -1},
        {"1.2.3.4:0 8.8.8.8:53", true, 16909060UL, 0},
        {"8.8.8.8 8.8.8.8", true, 134744072UL, -1},
        {"0.0.0.0 8.8.8.8", true, 0UL, -1},
        {"", false, 0UL, -1},
        {"no number here", false, 0UL, -1},
        {"...:::...", false, 0UL, -1},
        {"256.2.3.4", false, 0UL, -1},
        {"1.256.3.4", false, 0UL, -1},
        {"1.2.256.4", false, 0UL, -1},
        {"1.2.3.256", false, 0UL, -1},
        {"9991.2.3.4", false, 0UL, -1},
        {"999999999999999999999.2.3.4", false, 0UL, -1},
        {"192.168.01.1", false, 0UL, -1},
        {"01.2.3.4", false, 0UL, -1},
        {"1.02.3.4", false, 0UL, -1},
        {"1.2.03.4", false, 0UL, -1},
        {"1.2.3.04", false, 0UL, -1},
        {"00.0.0.0", false, 0UL, -1},
        {"000.0.0.0", false, 0UL, -1},
        {"0000.0.0.0", false, 0UL, -1},
        {"1.2.3.4:080", false, 0UL, -1},
        {"1.2.3.4:00", false, 0UL, -1},
        {"1.2.3.4:00000", false, 0UL, -1},
        {"1.2.3.4:000000", false, 0UL, -1},
        {"1.2.3.4:65536", false, 0UL, -1},
        {"1.2.3.4:99999", false, 0UL, -1},
        {"1.2.3.4:100000", false, 0UL, -1},
        {"1.2.3.4:999999999999999999999", false, 0UL, -1},
        {"12.34.56", false, 0UL, -1},
        {"1", false, 0UL, -1},
        {"1.2", false, 0UL, -1},
        {"1..2.3.4", false, 0UL, -1},
        {"1.2..4", false, 0UL, -1},
        {"1.2.3.", false, 0UL, -1},
        {"1.2.3.4.5", false, 0UL, -1},
        {"192.168.1.1.", false, 0UL, -1},
        {".1.2.3.4", false, 0UL, -1},
        {":1.2.3.4", false, 0UL, -1},
        {"IP:127.0.0.1", false, 0UL, -1},
        {".:1.2.3.4", false, 0UL, -1},
        {"1:2.3.4", false, 0UL, -1},
        {"1.2:3.4", false, 0UL, -1},
        {"1.2.3:4", false, 0UL, -1},
        {"1.2.3.4:", false, 0UL, -1},
        {"1.2.3.4::80", false, 0UL, -1},
        {"1.2.3.4:80:90", false, 0UL, -1},
        {"1.2.3.4:80.", false, 0UL, -1},
        {"1.2.3.4:80.5", false, 0UL, -1},
        {"1.2.3.4:abc", false, 0UL, -1},
        {"1.2.3.4: 80", false, 0UL, -1},
        {"1.2.3.4:-1", false, 0UL, -1},
        {"1.2.3.4:+80", false, 0UL, -1},
        {"1.2. 3.4", false, 0UL, -1},
        {"1.2.3.-4", false, 0UL, -1},
        {"9991.2.3.4 1.2.3.4.5", false, 0UL, -1},
        {"END", false, 0UL, -1},
        {"end", false, 0UL, -1},
    };
    std::size_t passed = 0;
    const std::size_t count = sizeof(tests) / sizeof(tests[0]);
    for (const auto& test : tests) {
        // Sentinels verify output assignment on BOTH success and failure.
        unsigned long address = 123456789UL;
        int port = 12345;
        const bool found = extractIPv4(test.input, address, port);
        if (found == test.found && address == test.address && port == test.port) {
            ++passed;
        } else {
            std::cerr << "FAIL: \"" << test.input << "\"\n"
                      << "  expected: " << test.found << ", " << test.address
                      << ", " << test.port << '\n'
                      << "  actual:   " << found << ", " << address
                      << ", " << port << '\n';
        }
    }
    std::cout << passed << '/' << count << " parser tests passed.\n";
    return passed == count ? 0 : 1;
}
#endif

