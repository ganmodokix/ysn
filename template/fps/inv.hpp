#pragma once
#include "base_template.hpp"
#include "conv/ntt.hpp"
#include "modint/modint.hpp"

// FPS逆元 (f(x))^{-1} mod x^N, 入力と同じ次数 O(NlogN)
template <mod_integral T>
vector<T> fps_inv(const vector<T>& f) {

    assert(f[0] != 0);
    auto g = vector<T>{f[0].inv()};

    // doubling
    while (g.size() < f.size()) {
        // g2 = 2g - fg^2
        // 2N長NTT/INTT 2回より 4N長さNTT/INTT 1回の方が速い
        const auto n = g.size();
        assert(popcount(n) == 1);
        auto ff = vector(f.begin(), f.begin() + min(f.size(), n * 2));
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
