#pragma once
#include "base_template.hpp"
#include "fps/log.hpp"
#include "conv/ntt.hpp"

// FPS exp
template <ll pdiv, ll prim>
vector<ll> fps_exp(const vector<ll>& f) {

    assert(f[0] % pdiv == 0);
    auto g = vector<ll>{1};

    while (g.size() < f.size()) {
        const auto ngg = g.size() * 2;

        // 1 - log g + f
        auto gg = g;
        gg.resize(ngg, 0);
        gg = fps_log<pdiv, prim>(move(gg));
        REP(i, ngg) {
            gg[i] = (!i + pdiv - gg[i] + (i < (ll)f.size() ? f[i] : 0)) % pdiv;
        }

        // g (1 - log g + f)
        g = convolve_p<pdiv, prim>(move(g), gg);
        g.resize(ngg, 0);
    }
    
    g.resize(f.size(), 0);
    return g;
}
