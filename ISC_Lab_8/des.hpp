#pragma once
#include <cstdint>

namespace des
{
    uint32_t feistel(uint32_t R, uint64_t subkey);
    void key_schedule(uint64_t key64, uint64_t subkeys[16]);
}
