#pragma once
#include "base_template.hpp"
#include "array/sahz.hpp"
// LIS の長さを返す O(NlogN)
template <ranges::forward_range R>
size_t lis_length(R&& x) {
    auto [xm, mi] = sahz(x);
    auto dp = vector<ptrdiff_t>{};
    dp.reserve(mi.size());
    for (auto&& xi : x) {
        const auto yi = xm[xi];
        const auto it = ranges::lower_bound(dp, yi);
        if (it == dp.end()) {
            dp.emplace_back(yi);
        } else {
            *it = yi;
        }
    }
    return dp.size();
}
