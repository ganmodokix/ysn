#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"

// Berlekamp-Massey O(N^2)
// C-recursive の母関数を FPS 有理式で表したときの分母を返す
// original: https://nyaannyaan.github.io/library/fps/berlekamp-massey.hpp
template <mod_integral T>
vector<T> berlekamp_massey(const vector<T>& s) {
    const auto n = ssize(s);
    auto b = vector{T{1}};
    auto c = vector{T{1}};
    b.reserve(n + 1);
    c.reserve(n + 1);
    auto y = T{1};
    for (auto ed = 1LL; ed <= n; ed++) {
        auto x = T{0};
        assert(ed >= ssize(c));
        RPE(i, ssize(c)) {
            const auto offset = ed - ssize(c);
            x += c[i] * s[offset + i];
        }
        b.emplace_back(T{0});
        if (x == 0) continue;
        const auto freq = x / y;
        if (ssize(c) < ssize(b)) {
            auto cprev = c;
            c.insert(c.begin(), ssize(b) - ssize(c), T{0});
            assert(ssize(b) == ssize(c));
            REP(i, ssize(c)) c.rbegin()[i] -= freq * b.rbegin()[i];
            b = move(cprev);
            y = x;
        } else {
            REP(i, ssize(b)) c.rbegin()[i] -= freq * b.rbegin()[i];
        }
    }
    ranges::reverse(c);
    return c;
}
