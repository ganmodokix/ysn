#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/log.hpp"
#include "fps/exp.hpp"

// FPS べき乗 f^n 
// 注意: 0^0 = 1 を仮定
template <mod_integral T>
vector<T> fps_pow(vector<T> f, const ll n) {
    assert(n >= 0);

    if (n == 0) {
        if (f.empty()) {
            return {1}; // 0^0 := 1
        }
        auto result = vector(f.size(), T{0});
        result[0] = 1;
        return result;
    }
    if (n == 1) {
        return f;
    }

    const auto nonzero = [](const T x) -> bool { return x.item() != 0; };
    const auto offset = ranges::find_if(f, nonzero) - f.begin();
    if (offset == (ptrdiff_t)f.size()) return f;

    const auto multiplier_total = f[offset].pow(n);
    const auto multiplier_inv = f[offset].inv();

    // x^k (k > 0) で括る
    if (offset > 0) { 

        const auto result_offset = offset * n;
        if (result_offset >= (ll)f.size()) {
            return vector(f.size(), T{0});
        }
        
        auto g = vector(f.begin() + offset, f.end());
        g.resize((ll)f.size() - result_offset, 0);
        for (auto&& x : g) x *= multiplier_inv;
        auto gn = fps_pow(move(g), n);
        for (auto&& x : gn) x *= multiplier_total;
        auto result = vector(f.size(), T{0});
        ranges::copy(gn, result.begin() + result_offset);

        return result;

    } else {
        
        for (auto&& x : f) x *= multiplier_inv;
        f = fps_log(move(f));
        for (auto&& x : f) x *= n;
        f = fps_exp(move(f));
        for (auto&& x : f) x *= multiplier_total;

        return f;

    }
}
