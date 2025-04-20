#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"

// \prod_i (x - r_i) を分割統治で求める
template <mod_integral T, ranges::input_range R>
requires convertible_to<ranges::range_value_t<R>, T>
constexpr vector<T> linear_prod(R&& r) {
    auto v = vector<vector<T>>{};
    for (auto&& a : r) {
        v.emplace_back(vector<T>{forward<decltype(a)>(a), 1});
    }
    while (v.size() > 1) {
        REP(i, v.size() / 2) {
            v[i] = convolve_p(move(v[i * 2]), move(v[i * 2 + 1]));
        }
        if (v.size() % 2 == 1) {
            v[v.size() / 2] = move(v.back());
        }
        v.resize((v.size() + 1) / 2);
    }
    return move(v[0]);
}