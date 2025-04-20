#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/remainder.hpp"

// (x_k)_{k=0}^{M-1} に対して多項式 f (deg f = N-1) の値を O(M(logM)^2+NlogN) で求める
// https://37zigen.com/multipoint-evaluation/
// verify: https://judge.yosupo.jp/submission/279884
template <mod_integral T, ranges::forward_range X>
constexpr vector<T> multipoint_evaluate(vector<T> f, X&& x_r) {
    // 重複アリで受け取っておく
    auto x = vector<T>(ranges::begin(x_r), ranges::end(x_r));
    const auto m0 = x.size();

    // 重複を除く
    {
        ranges::sort(x, less{}, &T::item);
        const auto removed = ranges::unique(x);
        x.erase(ranges::begin(removed), ranges::end(removed));
    }
    const auto m = x.size();

    if (m == 0) return {};

    // v: subproduct tree
    auto v = vector(m * 2 - 1, vector<T>{});
    REP(i, m) {
        v[m - 1 + i] = {-x[i], T{1}};
    }
    DSRNG(i, m - 2, 0) {
        auto&& cl = v[i * 2 + 1];
        auto&& cr = v[i * 2 + 2];
        v[i] = convolve_p(cl, cr);
    }

    // v: subremainder tree
    v[0] = polynomial_remainder(move(f), v[0]);
    RANGE(i, 1, m * 2 - 2) {
        v[i] = polynomial_remainder(v[(i - 1) / 2], move(v[i]));
    }

    // 重複を考えて返す
    using U = decltype(declval<T>().item());
    auto mapping = unordered_map<U, T>{};
    REP(i, m) {
        mapping[x[i].item()] = v[m - 1 + i][0];
    }
    auto ans = vector<T>{};
    ans.reserve(m0);
    for (const auto& y : x_r) {
        ans.emplace_back(mapping[y.item()]);
    }
    return ans;
}