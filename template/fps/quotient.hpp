#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// 多項式商 O(NlogN)
// f(x) = g(x)Q(x) + R(x) を満たす多項式 Q(x) を求める
// https://nyaannyaan.github.io/library/fps/formal-power-series.hpp.html
// verify: https://atcoder.jp/contests/abc300/submissions/65053375
template <mod_integral T>
constexpr vector<T> polynomial_quotient(vector<T> f, vector<T> g) {
    assert(!g.empty());
    if (f.size() < g.size()) {
        return vector<T>{0}; // 0
    }

    const auto quot_size = f.size() - g.size() + 1;

    // g のサイズが logN より小さい場合はnaiveの方が速い
    if (g.size() <= 60) {
        assert(g.back().item() != 0);

        const auto coeff = g.back().inv();
        for (auto&& x : g) x *= coeff;

        auto quo = vector(quot_size, T{0});
        DSRNG(i, quot_size - 1, 0) {
          quo[i] = f[g.size() - 1 + i];
          REP(j, g.size()) f[i + j] -= quo[i] * g[j];
        }

        for (auto&& x : quo) x *= coeff;
        quo.resize(quot_size, T{0});
        return quo;
    }

    ranges::reverse(f);
    ranges::reverse(g);
    f.resize(quot_size, T{0});
    g.resize(quot_size, T{0});
    auto q = convolve_p(move(f), fps_inv(move(g)));
    q.resize(quot_size, T{0});
    ranges::reverse(q);
    return q;
}
