#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "modint/modint_petit_p.hpp"
#include "modint/modint_factorial_cache.hpp"
#include "fps/multipoint.hpp"


// ⚠️遅いので非推奨、前計算ですら2sに間に合わない
// 代わりに factorial_batch.hpp (SSPP版) を強く推奨


// 不定元 x の上昇階乗冪 x^\bar{k} = x(x+1) ... (x+k-1) の多項式を求める O(K(logK)^2)
// \prod_{i=0}^{k-1} (x + i * \mathrm{step})
// result[i] = 1st-kind Starling (k, i)
template <mod_integral T>
constexpr vector<T> raising_factorial_poly(const ll k, const T step = T{1}) {
    assert(k >= 0);
    if (k == 0) return {1};
    auto f = vector(k * 2 - 1, vector<T>{});
    REP(i, k) {
        f[k - 1 + i] = {step * i, 1};
    }
    DSRNG(i, k - 2, 0) {
        f[i] = convolve_p(move(f[i * 2 + 1]), move(f[i * 2 + 2]));
    }
    return f[0];
}

template <mod_integral T, size_t b>
requires (b > 0)
constexpr vector<T> make_batch() {
    auto x = vector(T::pdiv / b - 1, T{0});
    RPE(i, x.size()) {
        x[i] = 1 + b * i;
    }
    auto y = multipoint_evaluate(raising_factorial_poly<T>(b), x);
    auto batch = vector(y.size() + 1, T{1});
    REP(i, y.size()) {
        batch[i + 1] = batch[i] * y[i];
    }
    return batch;
}

template <mod_integral T>
T fac(ll n) {
    assert(n >= 0);
    if (n >= T::pdiv) return T{0};

    constexpr auto b = 1LL << 13;
    static const auto batch = make_batch<T, b>();
    
    using U = decltype(declval<T>().item());
    static auto cache = unordered_map<U, T>{};
    if (const auto it = cache.find(n); it != cache.end()) return it->second;

    if (n % b * 2 < b || n / b + 1 >= (ll)batch.size()) {
        auto y = batch[n / b];
        RANGE(i, n / b * b + 1, n) {
            y *= i;
        }
        return cache[n] = y;
    } else {
        auto y = batch[n / b + 1].inv();
        RANGE(i, n + 1, (n / b + 1) * b) {
            y *= i;
        }
        return cache[n] = y.inv();
    }
}

// a 以上 b **以下** の整数すべての積 mod pdiv
// \prod_{k=a}^b k
template <mod_integral T>
T seqprod(ll a, ll b) {
    assert(a <= b);
    if (a < 0) {
        const auto t = (-a + T::pdiv - 1) / T::pdiv * T::pdiv;
        return seqprod<T>(a + t, b + t);
    }
    if (a / T::pdiv != b / T::pdiv) return 0;
    a %= T::pdiv;
    if (a == 0) return 0;
    b %= T::pdiv;
    return fac<T>(b) / fac<T>(a-1);
}
// 初項 a, 公差 b, 長さ n の等差数列の総積 mod pdiv
// \prod_{k=0}^{n-1} (a + kb)
template <mod_integral T>
T aritprod(T a, T b, ll n) {
    assert(n >= 0);
    if (n == 0) return T{1};
    if (b.item() == 0) {
        return a.pow(n);
    }
    const auto offset = (a / b).item();
    return seqprod<T>(offset, offset + n - 1) * b.pow(n);
}