#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/log.hpp"
#include "conv/ntt.hpp"

// FPS exp
template <mod_integral T>
constexpr vector<T> fps_exp(const vector<T>& f) {

    assert(f[0] == 0);
    auto g = vector<T>{1};

    while (g.size() < f.size()) {
        const auto n = g.size();

        // 1 - log g + f
        auto gg = g;
        gg.resize(n * 2, T{0});
        gg = fps_log(move(gg));
        REP(i, n * 2) {
            gg[i] = -gg[i] + (i < (ll)f.size() ? f[i] : 0);
        }
        gg[0] += 1;

        // g (1 - log g + f)
        g = convolve_p(move(g), move(gg));
        g.resize(n * 2, 0);
    }
    
    g.resize(f.size(), T{0});
    return g;
}
