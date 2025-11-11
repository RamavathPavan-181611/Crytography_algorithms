#include "des.hpp"
#include <iostream>
#include <iomanip>

int main()
{
    uint32_t R = 0xF0AAF0AA;
    uint64_t subkey = 0x1B02EFFC7072ULL;

    uint32_t result = des::feistel(R, subkey);

    std::cout << "f(R,K) = 0x"
              << std::hex << std::uppercase
              << std::setw(8) << std::setfill('0') << result
              << std::endl;

    return 0;
}
