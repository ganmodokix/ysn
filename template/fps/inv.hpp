#pragma once
#include "base_template.hpp"
#include "conv/ntt.hpp"
#include "modint/modint.hpp"

// FPS逆元 (f(x))^{-1} mod x^N, 入力と同じ次数 O(NlogN)
// verify: https://judge.yosupo.jp/submission/280173
template <mod_integral T>
constexpr vector<T> fps_inv(const vector<T>& f) {

    assert(*ranges::begin(f) != 0);
    auto g = vector<T>{ranges::begin(f)->inv()};

    // doubling
    while (g.size() < ranges::size(f)) {
        // g2 = 2g - fg^2
        // 2N長NTT/INTT 2回より 4N長さNTT/INTT 1回の方が速い
        const auto n = g.size();
        auto ff = vector<T>(
            ranges::begin(f), ranges::begin(f) + min(ranges::size(f), n * 2)
        );
        ff.resize(n * 4, T{0});
        g.resize(n * 4, T{0});
        ff = ntt(move(ff));
        g = ntt(move(g));
        REP(i, n * 4) {
            g[i] = g[i] * (2 - ff[i] * g[i]);
        }
        g = ntt(move(g), true);
        g.resize(n * 2);
    }
    g.resize(f.size());
    return g;
}
