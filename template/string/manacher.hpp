#pragma once
#include "base_template.hpp"
// Manacher's Algorithm: {s[i] = iを中心にした最長の回文の半径} を返す．線形時間．
template <
    ranges::random_access_range R,
    predicate Pred = ranges::equal_to,
    regular_invocable Proj = identity
>
vector<size_t> manacher(R&& r, Pred pred = {}, Proj proj = {}) {
    const auto n = ranges::size(r);
    auto r = vector(n, size_t{0});
    for (auto i = size_t{0}, j = size_t{0}; i < n; ) {
        while (
            i >= j && i + j < n &&
            invoke(pred,
                invoke(*(ranges::begin(r) + i - j)),
                invoke(*(ranges::begin(r) + i + j))
            )
        ) j++;
        r[i] = j;
        auto k = size_t{1};
        while (i >= k && k + r[i - k] < j) r[i + k] = r[i - k], ++k;
        i += k;
        j -= k;
    }
    return r;
}
