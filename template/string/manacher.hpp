#pragma once
#include "base_template.hpp"
// Manacher's Algorithm: {s[i] = iを中心にした最長の回文の半径} を返す．線形時間．
template <
    ranges::random_access_range R,
    typename Pred = ranges::equal_to,
    typename Proj = identity
>
vector<size_t> manacher(R&& r, Pred pred = {}, Proj proj = {}) {
    const auto n = ranges::size(r);
    auto res = vector(n, size_t{0});
    for (auto i = size_t{0}, j = size_t{0}; i < n; ) {
        while (
            i >= j && i + j < n &&
            invoke(pred,
                invoke(proj, *(ranges::begin(r) + i - j)),
                invoke(proj, *(ranges::begin(r) + i + j))
            )
        ) j++;
        res[i] = j;
        auto k = size_t{1};
        while (i >= k && k + r[i - k] < j) res[i + k] = res[i - k], ++k;
        i += k;
        j -= k;
    }
    return res;
}
