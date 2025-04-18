#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"

// FPS 積分 O(N) 定数項は 0 とする
template <mod_integral T>
vector<T> fps_integral(vector<T> f) {
    f.emplace_back(T{0});
    DSRNG(i, f.size() - 1, 1) {
        f[i] = f[i-1] / i;
    }
    f[0] = 0;
    return f;
}
