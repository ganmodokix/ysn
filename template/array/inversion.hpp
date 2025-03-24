#pragma once
#include "base_template.hpp"
#include "segtree/bit.hpp"
#include "ranges_to.hpp"
// BITを用いて数列の転倒数を求める　O(NlogN)
template <ranges::forward_range R>
size_t inversion(R&& r) {

    using T = ranges::range_value_t<R>;

    // 座圧
    auto unq = r | to_vector<T>{};
    const auto n = unq.size();
    UNIQUE(unq);
    auto sahz = unordered_map<T, size_t>{};
    for (auto i = size_t{0}; i < unq.size(); ++i) sahz[unq[i]] = i;
    
    // BITで転倒数を求める
    auto ans = size_t{0};
    auto bit = BIT<size_t>(n);
    for (const auto x : r) {
        const auto i = sahz[x];
        ans += bit.sum(i + 1, n);
        bit.add(i, 1);
    }
    
    return ans;
    
}
