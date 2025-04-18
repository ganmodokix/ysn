#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/quotient.hpp"

// 多項式剰余 f(x) mod g(x)
// verify: https://atcoder.jp/contests/abc300/submissions/64971205
template <mod_integral T>
vector<T> polynomial_remainder(vector<T> f, vector<T> g) {
    if (f.size() < g.size()) {
        return f;
    }
    assert(g.size() >= 1);
    const auto remainder_size = g.size() - 1;

    auto q = polynomial_quotient(f, g);
    auto gq = convolve_p(move(g), move(q));

    
    REP(i, remainder_size) {
        gq[i] = f[i] - gq[i];
    }
    gq.resize(remainder_size, T{0});
    return gq;
}
