#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// 剰余平方根 Tonelli-Shanks algorithm O((\log p)^2)
// https://nyaannyaan.github.io/library/modulo/mod-sqrt.hpp
// ⚠️ 存在しない場合 nullopt を返す
template <mod_integral T>
constexpr optional<T> mod_sqrt(const T a) {
    if (a == 0 || a == 1) return a;
    // x^2 === a (mod p) なる x が存在 <=> a^\frac{p-1}{2} === 1 (mod p)
    if (a.pow((T::pdiv - 1) / 2) != 1)
        return nullopt;
    
    auto b = T{1};
    auto one = T{1};
    while (b.pow((T::pdiv - 1) / 2) == 1) {
        // 本来乱択だが線形探索してもOKらしい?
        b += one;
    }
    auto e = countr_zero<ull>(T::pdiv - 1);
    const auto m = (T::pdiv - 1) >> e;
    const auto x0 = a.pow(m / 2);
    auto y = a * x0 * x0;
    auto x = x0 * a;
    auto z = b.pow(m);
    while (y != 1) {
        auto j = 0LL;
        auto t = y;
        while (t != one) {
            ++j;
            t *= t;
        }
        z = z.pow(1LL << (e - j - 1));
        x *= z;
        z *= z;
        y *= z;
        e = j;
    }
    return x;
}

// FPS平方根 g(x): g(x)^2 === f(x) mod x^n
// ref: https://nyaannyaan.github.io/library/fps/fps-sqrt.hpp
// verify: https://judge.yosupo.jp/submission/280241
// ⚠️ 存在しない場合、空配列を返す
template <mod_integral T>
constexpr vector<T> fps_sqrt(const vector<T>& f) {
    if (f.empty()) return {};

    auto nonzero = [](const T x) { return x != 0; };
    const auto offset = static_cast<size_t>(ranges::find_if(f, nonzero) - ranges::begin(f));
    if (f.size() == offset) return vector(f.size(), T{0});
    if (offset % 2 != 0) return {};

    const auto f0_sqrt = mod_sqrt(*(ranges::begin(f) + offset));
    if (!f0_sqrt.has_value()) return {};
    auto g = vector{*f0_sqrt};
    
    // g2 = 1/2 (g + f/g)
    while (g.size() + offset / 2 < ranges::size(f)) {
        const auto n = g.size();
        auto ff = vector<T>(
            ranges::begin(f) + offset,
            ranges::begin(f) + offset + min(n * 2, ranges::size(f) - offset)
        );
        ff.resize(n * 2, T{0});
        g.resize(n * 2, T{0});
        auto fdg = convolve_p(fps_inv(g), move(ff));
        fdg.resize(n * 2);
        REP(i, n) fdg[i] += g[i];
        REP(i, n * 2) fdg[i] /= 2;
        g = move(fdg);
    }
    g.resize(ranges::size(f) - offset / 2, T{0});
    g.insert(g.begin(), offset / 2, T{0});
    return g;
}
