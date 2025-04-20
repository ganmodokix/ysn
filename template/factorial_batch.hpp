#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "modint/modint_petit_p.hpp"
#include "modint/modint_factorial_cache.hpp"
#include "sspp.hpp"

// (任意)! mod pdiv を O(\sqrt{P} logP) で行う (SSPP版)
// ref: https://suisen-kyopro.hatenablog.com/entry/2023/11/22/201600
// verify: https://atcoder.jp/contests/abc385/submissions/64926029
namespace factorial_batch {

    constexpr auto pdiv = 998244353LL;
    
    template <mod_integral T>
    vector<T> make_batch(size_t k) {
        const auto b = size_t{1} << k;
        auto f = vector(1, T{1});
        auto pow2 = T{1};
        REP(i, k) {
            const auto g = sspp::sspp(f, f.size(), f.size() * 3);
            const auto getf = [&f, &g](const auto idx) {
                const auto n = (ll)f.size();
                if (idx < n) return f[idx];
                return g[idx - n];
            };
            auto ff = vector(f.size() * 2, T{0});
            REP(i, ff.size()) {
                ff[i] = getf(i * 2) * getf(i * 2 + 1) * pow2 * (i * 2 + 1);
            }
            pow2 += pow2;
            f = move(ff);
        }
        const auto z = (size_t{T::pdiv} + b - 1) / b;
        f.reserve(bit_ceil(z));
        if (f.size() < z) {
            ranges::move(
                sspp::sspp(f, f.size(), z - f.size()),
                back_inserter(f)
            );
        }
        auto fi = f[0];
        f[0] = 1;  // 0! = 1
        REP(i, z - 1) {
            // ((i+1)2^K)! = (i+1)2^K * f_K(i) * (i2^K)!
            // fi == f_K(i)
            // f[i] == (i2^K)!
            const auto fip1 = f[i + 1];
            f[i + 1] = f[i] * fi * (i + 1) * pow2;
            fi = fip1;
        }
        if (f.size() > z) f.resize(z);
        return f;
    }

    template <mod_integral T>
    T fac(ll n) {
        assert(n >= 0);
        if (n >= T::pdiv) return T{0};

        constexpr auto k = size_t{9};
        constexpr auto b = size_t{1} << k;
        static const auto batch = make_batch<T>(k);

        // memoize
        static auto cache = unordered_map<ll, T>{};
        if (auto it = cache.find(n); it != cache.end()) {
            return it->second;
        }

        // fetch from batch
        if (n % b * 2 >= b && n / b + 1 < batch.size()) {
            auto ans = batch[n / b + 1].inv();
            DSRNG(x, (n / b + 1) * b, n + 1) {
                ans *= x;
            }
            return cache[n] = ans.inv();
        } else {
            auto ans = batch[n / b];
            RANGE(x, n / b * b + 1, n) {
                ans *= x;
            }
            return cache[n] = ans;
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

}