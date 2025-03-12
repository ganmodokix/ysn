#pragma once
#include "base_template.hpp"

struct xor128_generator {

    uint32_t x = 123456789u;
    uint32_t y = 362436069u;
    uint32_t z = 521288629u;
    uint32_t w = 88675123u;

    consteval xor128_generator(const uint32_t seed = 0) { x ^= seed; }

    consteval uint32_t operator()() {
        auto t = uint32_t{};
        t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));  
    }

};