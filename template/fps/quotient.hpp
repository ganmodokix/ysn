#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// 多項式商 O(NlogN)
// f(x) = g(x)Q(x) + R(x) を満たす多項式 Q(x) を求める
// https://nyaannyaan.github.io/library/fps/formal-power-series.hpp.html
template <mod_integral T>
vector<T> polynomial_quotient(vector<T> f, vector<T> g) {
    assert(!g.empty());
    if (f.size() < g.size()) {
        return vector<T>{0}; // 0
    }
    ranges::reverse(f);
    ranges::reverse(g);
    const auto quot_size = f.size() - g.size() + 1;
    f.resize(quot_size, T{0});
    g.resize(quot_size, T{0});
    auto q = convolve_p(move(f), fps_inv(move(g)));
    q.resize(quot_size, T{0});
    ranges::reverse(q);
    return q;
}
