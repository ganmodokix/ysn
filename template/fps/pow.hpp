#pragma once
#include "base_template.hpp"
#include "modint_petit.hpp"
#include "fps/log.hpp"
#include "fps/exp.hpp"

// FPS べき乗 f^n 
// 注意: 0^0 = 1 を仮定
template <ll pdiv, ll prim>
vector<ll> fps_pow(vector<ll> f, ll n) {
    assert(n >= 0);

    if (n == 0) {
        if (f.empty()) {
            return {1}; // 0^0 := 1
        }
        auto result = vector<ll>(f.size(), 0);
        result[0] = 1;
        return result;
    } else if (n == 1) {
        return f;
    }

    const auto nr = n % pdiv;

    ll offset = find_if(ALL(f), [](auto x) { return x != 0; }) - f.begin();
    if (offset == (ll)f.size()) return f;

    const auto multiplier_total = modpow(f[offset], n, pdiv);
    const auto multiplier_inv = modinv(f[offset], pdiv);

    if (offset > 0) { 

        const auto result_offset = offset * n;
        if (result_offset >= (ll)f.size()) {
            return vector<ll>(f.size(), 0);
        }
        
        auto g = vector<ll>(f.begin() + offset, f.end());
        g.resize((ll)f.size() - result_offset, 0);
        for (auto&& x : g) (x *= multiplier_inv) %= pdiv;
        auto gn = fps_pow<pdiv, prim>(move(g), n);
        for (auto&& x : gn) (x *= multiplier_total) %= pdiv;
        auto result = vector<ll>(f.size(), 0);
        copy(gn.begin(), gn.end(), result.begin() + result_offset);

        return result;

    } else {
        
        for (auto&& x : f) (x *= multiplier_inv) %= pdiv;
        f = fps_log<pdiv, prim>(move(f));
        for (auto&& x : f) x = x * nr % pdiv;
        f = fps_exp<pdiv, prim>(move(f));
        for (auto&& x : f) (x *= multiplier_total) %= pdiv;

        return f;

    }
}
