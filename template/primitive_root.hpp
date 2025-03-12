#pragma once
#include "base_template.hpp"
#include "modint_petit.hpp"
#include "marathon/xor128_generator.hpp"

// 乱択で原始根を求める
// UB if pdiv is not a prime
consteval ll random_primitive_root(const ll pdiv) {
    assert(pdiv == 2 || (pdiv >= 2 && pdiv % 2 == 1));

    // 素因数分解
    auto pf = vector<pair<ll, ll>>{};
    auto n = pdiv - 1;
    for (auto i = 2LL; i * i <= n; ++i) {
        while (n % i == 0) {
            n /= i;
            if (pf.empty() || pf.back().first != i) {
                pf.emplace_back(i, 1);
            }
            pf.back().second *= i;
        }
    }
    if (n > 1) {
        pf.emplace_back(n, n);
    }

    // 乱択
    auto rd = xor128_generator(pdiv);
    auto g = 1LL;
    for (const auto& [q, qe] : pf) {
        auto a = -1LL;
        while (true) {
            a = rd() % pdiv;
            if (modpow(a, (pdiv - 1) / q, pdiv) != 1) {
                break;
            }
        }
        (g *= modpow(a, (pdiv - 1) / qe, pdiv)) %= pdiv;
    }
    return g;
}

// 最小の原始根を総当たりで求める pdiv が素数であればすぐ終わるはず
// コンパイル時に行ってしまうことを推奨
consteval ll primitive_root(const ll pdiv) {
    assert(pdiv == 2 || (pdiv >= 2 && pdiv % 2 == 1));

    // 素因数分解
    auto pf = vector<ll>{};
    auto n = pdiv - 1;
    for (auto i = 2LL; i * i <= n; ++i) {
        if (n % i == 0) {
            pf.emplace_back(i);
            do { n /= i; } while (n % i == 0);
        }
    }
    if (n > 1) {
        pf.emplace_back(n);
    }

    // 線形探索
    for (auto i = 2; i < pdiv; ++i) {
        auto okay = true;
        for (const auto f : pf) {
            if (modpow(i, (pdiv - 1) / f, pdiv) == 1) {
                okay = false;
                break;
            }
        }
        if (okay) {
            return i;
        }
    }
    assert(false);
}
