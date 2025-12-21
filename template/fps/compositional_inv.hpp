#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/log.hpp"
#include "fps/exp.hpp"
#include "fps/power_projection.hpp"

// FPS 合成に対する逆関数 O(N(logN)^2)
// f に対して g(f(x)) = f(g(x)) なる g を返す
// https://maspypy.com/fps-%E5%90%88%E6%88%90%E3%83%BB%E9%80%86%E9%96%A2%E6%95%B0%E3%81%AE%E8%A7%A3%E8%AA%AC-1-%E9%80%86%E9%96%A2%E6%95%B0%E3%81%A8-power-projection
template <mod_integral T>
constexpr vector<T> compositional_inverse(vector<T> f)
{
    assert(ssize(f) >= 2);
    assert(f[0] == 0);
    assert(f[1] != 0);
    if (f[1] != 1) {
        const auto cinv = f[1].inv();
        REP(i, ssize(f)) f[i] *= cinv;
        auto result = compositional_inverse(f);

        auto base = 1_p;
        REP(i, ssize(result)) {
            result[i] *= base;
            if (i + 1 == i_len) break;
            base *= cinv;
        }
        return result;
    }

    const auto n_ = ssize(f);
    f.resize(bit_ceil(f.size()), T{0});
    const auto n = ssize(f);
    
    // [x^{n-1-i}] i * (g(x) / x)^{-n} = n * [x^{n-1}] f(x)^i
    // [x^{n-1-i}] (g(x) / x)^{-n} = n/i * [x^{n-1}] f(x)^i
    // ... Lagrange Inversion Theorem
    const auto one = vector<T>{1};
    auto goxpmn = power_projection(f, one, n);
    RANGE(i, 1 + n - n_, n - 1) goxpmn[i] *= modint(n - 1) / i;
    ranges::reverse(goxpmn);
    
    // g(x) / x
    goxpmn.resize(n_ - 1);
    auto gox = fps_log(move(goxpmn));
    const auto m1on = -1_p / (n - 1);
    REP(i, ssize(gox)) gox[i] *= m1on;
    gox = fps_exp(gox);
    
    // g(x)
    gox.emplace(gox.begin(), 0_p);
    return gox;
}