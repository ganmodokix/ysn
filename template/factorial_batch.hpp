#pragma once
#include "base_template.hpp"
#include "modint_petit_p.hpp"
#include "sspp.hpp"

// ref: https://suisen-kyopro.hatenablog.com/entry/2023/11/22/201600
namespace factorial_batch_998244353 {

    constexpr auto pdiv = 998244353LL;
    
    vector<ll> make_batch(ll k) {
        assert(k >= 0);
        const auto b = 1LL << k;
        auto f = vector(1, 1LL);
        auto pow2 = 1LL;
        REP(i, k) {
        const auto g = sspp_998244353::sspp(f, f.size(), f.size() * 3);
            const auto getf = [&f, &g](const auto idx) {
                const auto n = (ll)f.size();
                if (idx < n) return f[idx];
                return g[idx - n];
            };
            auto ff = vector(f.size() * 2, 0LL);
            REP(i, ff.size()) {
                ff[i] = getf(i * 2)
                        * getf(i * 2 + 1) % pdiv
                        * pow2 % pdiv
                        * (i * 2 + 1) % pdiv;
            }
            pow2 += pow2; if (pow2 >= pdiv) pow2 -= pdiv;
            f = move(ff);
        }
        const auto z = (pdiv + b - 1) / b;
        f.reserve(bit_ceil<size_t>(z));
        if ((ll)f.size() < z) {
            ranges::move(
                sspp_998244353::sspp(f, f.size(), z - (ll)f.size()),
                back_inserter(f)
            );
        }
        auto fi = f[0];
        f[0] = 1;  // 0! = 1
        REP(i, z-1) {
            // ((i+1)2^K)! = (i+1)2^K * f_K(i) * (i2^K)!
            // fi == f_K(i)
            // f[i] == (i2^K)!
            const auto fip1 = f[i+1];
            f[i+1] = f[i] * fi % pdiv * (i + 1) % pdiv * pow2 % pdiv;
            fi = fip1;
        }
        f.reserve(z);
        return f;
    }

    ll fac(ll n) {
        constexpr auto k = 9LL;
        constexpr auto b = 1LL << k;
        static const auto batch = make_batch(k);
        if (n >= pdiv) return 0;
        static auto cache = unordered_map<ll, ll>{};
        if (auto it = cache.find(n); it != cache.end()) {
            return it->second;
        }
        if (n % b * 2 >= b && n / b + 1 < (ll)batch.size()) {
            auto ans = modinv_p<pdiv>(batch[n / b + 1]);
            DSRNG(x, (n / b + 1) * b, n + 1) {
                (ans *= x) %= pdiv;
            }
            return cache[n] = modinv(ans, pdiv);
        } else {
            auto ans = batch[n / b];
            RANGE(x, n / b * b + 1, n) {
                (ans *= x) %= pdiv;
            }
            return cache[n] = ans;
        }
    }
    
    // a 以上 b **以下** の整数すべての積 mod pdiv
    // \prod_{k=a}^b k
    ll seqprod(ll a, ll b) {
        assert(a <= b);
        if (a < 0) {
            const auto t = (-a + pdiv-1) / pdiv * pdiv;
            return seqprod(a + t, b + t);
        }
        if (a / pdiv != b / pdiv) return 0;
        a %= pdiv;
        if (a == 0) return 0;
        b %= pdiv;
        return fac(b) * modinv(fac(a-1), pdiv) % pdiv;
    }
    // 初項 a, 公差 b, 長さ n の等差数列の総積 mod pdiv
    // \prod_{k=0}^{n-1} (a + kb)
    ll aritprod(ll a, ll b, ll n) {
        if (n == 0) return 1;
        if ((b %= pdiv) == 0) {
            return modpow(a, n, pdiv);
        }
        const auto offset = (a * modinv(b, pdiv)) % pdiv;
        return seqprod(offset, offset + n - 1) * modpow(b, n, pdiv) % pdiv;
    }

}