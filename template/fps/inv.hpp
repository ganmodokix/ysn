#pragma once
#include "base_template.hpp"
#include "conv/ntt.hpp"
#include "modint_petit.hpp"

// FPS逆元, 入力と同じ次数 O(NlogN)
template <ll pdiv, ll prim>
vector<ll> fps_inv(const vector<ll>& f) {

    assert(f[0] % pdiv != 0);
    auto g = vector<ll>{modinv(f[0], pdiv)};

    // doubling
    while (g.size() < f.size()) {

        // g^2 mod x^2K
        const auto ngg = g.size() * 2;
        auto gg = g;
        gg.resize(ngg, 0);
        gg = ntt<pdiv, prim>(move(gg));
        for (auto&& x : gg) (x *= x) %= pdiv;
        gg = intt<pdiv, prim>(move(gg));

        // fg^2 mod x^2K
        const auto nfgg = g.size() * 4;
        auto ff = vector<ll>(f.begin(), f.begin() + min(f.size(), ngg));
        ff.resize(nfgg, 0);
        ff = ntt<pdiv, prim>(move(ff));
        gg.resize(nfgg, 0);
        gg = ntt<pdiv, prim>(move(gg));
        REP(i, nfgg) (gg[i] *= ff[i]) %= pdiv;
        gg = intt<pdiv, prim>(move(gg));
        gg.resize(ngg);

        // 2g - fg^2
        g.resize(ngg, 0);
        REP(i, ngg) {
            g[i] = (g[i] * 2 + pdiv - gg[i]) % pdiv;
        }
    }
    g.resize(f.size());
    return g;
}
