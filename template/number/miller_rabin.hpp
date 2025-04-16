#pragma once
#include "base_template.hpp"
#include "modint/modint_petit.hpp"

// n 素数である、すなわち約数をちょうど 2 個持つ iff trueを返す
// ! 負整数 n は約数 1, n, -n を持ち3つ以上
// ! 1 は約数が 1 の一つのみ
// ! 0 は約数を無限個持つ
constexpr bool is_prime(const ll n) {
    if (n <= 1) return false;
    
    auto miller_rabin = [](const ll n, const auto& bases) -> bool {
        assert(n >= 2);
        const auto s = (ll)countr_zero<ull>(n - 1);
        const auto d = (n - 1) >> s;
        for (const ll a : bases) {
            if (a % n == 0) continue;
            auto a2rd = modpow(a, d, n);
            if (a2rd == 1) continue;
            auto composite = true;
            REP(r, s) {
                if (a2rd == n - 1) {
                    composite = false;
                    break;
                }
                if (r + 1 == s) break;
                (a2rd *= a2rd) %= n;
            }
            if (composite) {
                return false;
            }
        }
        return true;
    };

    if (n < (1LL << 32)) {
        // https://zenn.dev/mizar/articles/791698ea860581
        // https://t5k.org/prove/prove2_3.html
        return miller_rabin(n, array{2, 7, 61});
    }

    return miller_rabin(n, array{2, 325, 9375, 28178, 450775, 9780504, 1795265022});

}

// test
static_assert(!is_prime(9));
static_assert(!is_prime(32));
static_assert(!is_prime(998244352));
static_assert(is_prime(3));
static_assert(is_prime(998244353));
static_assert(is_prime((ll)1e9 + 7));
static_assert(is_prime((ll)1e9 + 9));
