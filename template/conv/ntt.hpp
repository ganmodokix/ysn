#pragma once
#include "base_template.hpp"
#include "modint_petit.hpp"
#include "garner.hpp"
#include "conv/butterfly.hpp"

// 精度に寄りけりだがconv1回で済むFFTの方がいい場合もあることに留意
// Cooley-Tukey型 高速フーリエ変換 O(NlogN)
template <ll pdiv = 998244353, ll prim = 3, typename T>
auto ntt(T&& a, const bool inv = false) {
    if (inv) {
        return butterfly_inv<pdiv, prim>(forward<T>(a));
    } else {
        return butterfly<pdiv, prim>(forward<T>(a));
    }
}
template <ll pdiv = 998244353, ll prim = 3, typename T>
auto intt(T&& a) {
    return ntt(forward<T>(a), true);
}

// \mathbb{F}_{m} での畳み込み
template <ll pdiv, ll prim, typename T, typename U>
vector<ll> convolve_p(T&& x, U&& y) {
    assert(!x.empty() && !y.empty());
    const auto t = x.size() + y.size() - 1;
    if (min(x.size(), y.size()) <= size_t{60}) {
        auto r = vector(t, 0LL);
        REP(i, x.size()) REP(j, y.size()) {
            r[i + j] += x[i] * y[j] % pdiv;
            if (r[i + j] >= pdiv) r[i + j] -= pdiv;
        }
        return r;
    }
    const auto h = bit_ceil(t);
    auto a = forward<T>(x); a.resize(h, 0);
    auto b = forward<U>(y); b.resize(h, 0);
    a = butterfly<pdiv, prim>(move(a));
    b = butterfly<pdiv, prim>(move(b));
    for (auto i = size_t{0}; i < a.size(); i++) {
        (a[i] *= b[i]) %= pdiv;
    }
    a = butterfly_inv<pdiv, prim>(move(a));
    a.resize(t);
    return a;
}

// 任意MODでの畳み込み演算
vector<ll> convolve(const vector<ll> &x, const vector<ll> &y, const ll pdiv) {
    if (pdiv == 998244353) return convolve_p<998244353, 3>(x, y);
    const auto conv1 = convolve_p<1224736769, 3>(x, y);
    // const auto conv2 = convolve_p<998244353, 3>(x, y);
    const auto conv3 = convolve_p<469762049, 3>(x, y);
    const auto conv4 = convolve_p<167772161, 3>(x, y);
    ll t = x.size() + y.size() - 1;
    vector<ll> result(t, 0);
    for (ll i = 0; i < t; i++) {
        vector<pair<ll, ll>> g = {
            {conv1[i], 1224736769},
            // {conv2[i], 998244353},
            {conv3[i], 469762049},
            {conv4[i], 167772161}
        };
        result[i] = garner(g, pdiv);
    }
    return result;
}
