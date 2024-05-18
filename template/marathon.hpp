#pragma once
#include "base_template.hpp"
#include "marathon/xor128.hpp"
namespace marathon {
    clock_t start_clock;
    constexpr double time_limit = 2; // 実行時間[s]
    constexpr double time_limit_inv = 1.0 / time_limit;
    constexpr double CLOCKS_PER_SEC_INV = 1.0 / CLOCKS_PER_SEC;
    inline double nowtime() { return CLOCKS_PER_SEC_INV * (clock() - start_clock); }
    inline bool intime() { return nowtime() < time_limit; }
    inline double progress() { return nowtime() * time_limit_inv; }
    struct preexec_dummy {
        preexec_dummy() {
            start_clock = clock();
        }
    } preexec_dummy_instance;
}
