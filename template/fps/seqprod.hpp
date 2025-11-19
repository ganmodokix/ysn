#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"

// 多項式総積（マージテク） O(N log(N)^2) (N: 次数の総和)
// ref: https://github.com/yosupo06/library-checker-problems/issues/763
// verify: https://judge.yosupo.jp/submission/280244
template <mod_integral T>
vector<T> poly_sequence_prod(vector<vector<T>> fs) {
    if (fs.empty()) return {1};
    using U = pair<size_t, size_t>;
    auto q = priority_queue<U, vector<U>, greater<U>>{};
    REP(i, fs.size()) q.emplace(fs[i].size(), i);
    while (q.size() >= 2) {
        const auto [sz1, i] = q.top(); q.pop();
        const auto [sz2, j] = q.top(); q.pop();
        fs.emplace_back(convolve_p(move(fs[i]), move(fs[j])));
        q.emplace(fs.back().size(), fs.size() - 1);
    }
    const auto [sz, i] = q.top();
    return move(fs[i]);
}