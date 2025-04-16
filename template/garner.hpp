#pragma once
#include "base_template.hpp"
#include "modint/modint_petit_p.hpp"
// Garnerのアルゴリズム O(N^2)
// {x === x[i] mod m[i]} から x mod pdiv を復元
// m[i] は pairwise 互いに素である必要がある
template <ranges::random_access_range X, ranges::random_access_range M>
requires convertible_to<ranges::range_value_t<X>, ll>
    && convertible_to<ranges::range_value_t<M>, ll>
ll garner(X&& x, M&& m, const ll pdiv) {
    const auto n = ranges::size(x);
    assert(ranges::size(m) == n);
    auto p = vector(n + 1, 1LL);
    auto v = vector(n + 1, 0LL);
    for (const auto k : views::iota(size_t{0}, n)) {
        // (x_k - v_k) / p_k mod m_k
        const auto t = ((x[k] - v[k]) % m[k] + m[k]) % m[k]
            * modinv_extgcd(p[k], m[k]) % m[k];
        for (const auto i : views::iota(k + 1, n + 1)) {
            const auto bi = i == n ? pdiv : m[i];
            (v[i] += t * p[i]) %= bi;
            (p[i] *= m[k]) %= bi;
        }
    }
    return v.back();
};
