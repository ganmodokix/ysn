#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/quotient.hpp"
#include "fps/remainder.hpp"

// x^n mod c(x) を求める
template <mod_integral T>
vector<T> kitamasa_coefficients(const vector<T>& c, ll n) {
    assert(n >= 0);
    assert(!c.empty());

    // x^{n+1} = x * x^n
    auto plus_one = [&](vector<T> f) -> vector<T> {
        assert(f.size() + 1 == c.size());
        const auto fl = f.back();
        DSRNG(i, (ll)f.size() - 1, 0) {
            f[i] = c[i] * fl + (i >= 1 ? f[i-1] : T{0});
        }
        return f;
    };

    // x^{2n} = (x^n)^2
    auto double_up = [&](vector<T> f) -> vector<T> {
        auto ff = f;
        return polynomial_remainder(convolve_p(move(f), move(ff)), c);
    };

    auto f = vector(c.size() - 1, T{0});
    {
        auto k = bit_width<ull>(n);
        while (k > 0 && (n >> (k - 1)) < (ll)c.size() - 1) --k;
        f[n >> k] = 1;
        while (k > 0) {
            f = double_up(move(f));
            if (n >> --k & 1) {
                f = plus_one(move(f));
            }
        }
    }

    return f;
}

// 高速kitamasa法 O(N(logN)^2)
// 線形漸化式 a_{i+N} = \sum_{k=0}^{N-1} a_{i + k} c_{k}
//  + 初項 a_k (k = 0, 1, ..., N-1)
// に対して、a_n を modint で求める
template <mod_integral T>
T fast_kitamasa(vector<T> c, const vector<T>& a, ll n) {
    assert(n >= 0);
    assert(a.size() == c.size());
    if (n <= (ll)a.size()) return a[n];

    // \sum_i c_i x^i === x^N    mod c(x)
    c.emplace_back(-1);
    
    const auto f = kitamasa_coefficients(c, n);
    auto ans = T{0};
    REP(i, a.size()) {
        ans += a[i] * f[i];
    }
    return ans;
    
}