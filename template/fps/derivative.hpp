#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"

// FPS 微分 O(N)
template <mod_integral T>
vector<T> fps_derivative(vector<T> f) {
    if (f.empty()) return {};
    REP(i, f.size() - 1) {
        f[i] = f[i+1] * (i + 1);
    }
    f.pop_back();
    return f;
}
