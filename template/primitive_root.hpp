#pragma once
#include "base_template.hpp"
#include "modint_petit.hpp"
#include "number/euler_phi.hpp"
#include "marathon/xor128_generator.hpp"

namespace primitive_root_detail {
    template<integral T>
    consteval pair<T, vector<pair<T, T>>> phi_prime_factors(const T n) {
        assert(n >= 2);
        auto pf = vector<pair<T, T>>{};
        const auto phi = euler_phi(n);
        {
            auto n = phi;
            for (auto i = T{2}; i <= n / i; ++i) {
                if (n % i == 0) {
                    pf.emplace_back(i, 1);
                    do { n /= i; pf.back().second *= i; } while (n % i == 0);
                }
            }
            if (n > 1) {
                pf.emplace_back(n, n);
            }
        }
        return {phi, move(pf)};
    }
}

// 乱択で原始根を求める
consteval ll random_primitive_root(const ll pdiv) {
    assert(pdiv >= 2);

    // 素因数分解
    const auto [phi, pf] = primitive_root_detail::phi_prime_factors(pdiv);

    // 乱択
    auto rd = xor128_generator(pdiv);
    auto g = 1LL;
    for (const auto& [q, qe] : pf) {
        auto a = -1LL;
        while (true) {
            a = rd() % pdiv;
            if (modpow(a, phi / q, pdiv) != 1) {
                break;
            }
        }
        (g *= modpow(a, phi / qe, pdiv)) %= pdiv;
    }
    return g;
}

// 最小の原始根を総当たりで求める pdiv が素数であればすぐ終わるはず
// コンパイル時に行ってしまうことを推奨
consteval ll primitive_root(const ll pdiv) {
    assert(pdiv >= 2);

    // 素因数分解
    const auto [phi, pf] = primitive_root_detail::phi_prime_factors(pdiv);

    // 線形探索
    for (auto i = 2LL; i < pdiv; ++i) {
        auto okay = true;
        for (const auto& [f, _] : pf) {
            if (modpow(i, phi / f, pdiv) == 1) {
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
