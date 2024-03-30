#pragma once
#include "base_template.hpp"
// 高速約数ゼータ変換 O(VlogV); V being x.size()
// fzt(x)[s] == Σ_{s|t/t|s} x[t] (ただし∀i,0|iとする)
// inv: これをtrueにするとメビウス, subset: これをtrueにするとt|s
template<typename T>
vector<T> fdzt(vector<T>&& a_, const bool inv = false, const bool subset = false) {
    auto a = vector<T>(move(a_));
    const size_t n = a.size();
    vector<uint_fast8_t> sieve(n, true);
    if (inv) {
        for (size_t i = 1; i < n; i++) {
            a[i] -= a[0];
        }
    }
    for (size_t i = 2; i < n; i++) {
        if (!sieve[i]) continue;
        if (!subset) {
            if (inv) {
                for (size_t j = 1; i*j < n; j++) {
                    sieve[i*j] = false;
                    a[j] -= a[i*j];
                }
            } else {
                for (size_t j = (n-1)/i; j > 0; j--) {
                    sieve[i*j] = false;
                    a[j] += a[i*j];
                }
            }
        } else {
            if (inv) {
                for (size_t j = (n-1)/i; j > 0; j--) {
                    sieve[i*j] = false;
                    a[i*j] -= a[j];
                }
            } else {
                for (size_t j = 1; i*j < n; j++) {
                    sieve[i*j] = false;
                    a[i*j] += a[j];
                }
            }
        }
    }
    if (!inv) {
        for (size_t i = 1; i < n; i++) {
            a[i] += a[0];
        }
    }
    return a;
}
template<typename T>
vector<T> fdzt(const vector<T> &a, const bool inv = false, const bool subset = false) {
    return fdzt(vector<T>(a), inv, subset);
}
// GCD/LCM畳み込み (x*y)[d] = Σ_{gcd/lcm(i,j)=d} x[i]*y[j], 但しgcd(x,0)==lcm(0,x)==x
template <typename T, typename U>
auto convolve_zeta(T&& x, U&& y, const bool subset) {
    size_t n = max(x.size(), y.size());
    auto xc = forward<T>(x); xc.resize(n, 0);
    auto yc = forward<U>(y); yc.resize(n, 0);
    auto X = fdzt(move(xc), false, subset);
    auto Y = fdzt(move(yc), false, subset);
    for (size_t i = n; i--;) {
        X[i] *= Y[i];
    }
    return fdzt(move(X), true, subset);
}
template <typename T, typename U>
auto convolve_gcd(T&& x, U&& y) {
    return convolve_zeta(forward<T>(x), forward<U>(y), false);
}
template <typename T, typename U>
auto convolve_lcm(T&& x, U&& y) {
    return convolve_zeta(forward<T>(x), forward<U>(y), true);
}
