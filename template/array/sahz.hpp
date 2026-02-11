#pragma once
#include "base_template.hpp"
#include "ranges_to.hpp"

// 座圧 {写像, 逆写像} が返る
// Map が std::unordered_map の場合、平均 O(N) 最悪 O(N^2)
// Map が std::map の場合、平均 O(NlogN) 最悪 O(NlogN)
// verified at <<WIP>>
template <
    typename R,
    typename Map = unordered_map<ranges::range_value_t<R>, ptrdiff_t>,
    typename Comp = less<>,
    typename Proj = identity
>
auto sahz(R&& r, Comp comp = {}, Proj proj = {})
    -> pair<Map, vector<ranges::range_value_t<R>>>
{
    using T = ranges::range_value_t<R>;
    auto unique_values = r | to_vector<T>{};
    {
        ranges::sort(unique_values, comp, proj);
        auto equiv_comp = [&](const auto& x, const auto& y) -> bool {
            return !comp(x, y) && !comp(y, x);
        };
        const auto erased_range = ranges::unique(unique_values, equiv_comp, proj);
        unique_values.erase(erased_range.begin(), erased_range.end());
    }

    auto smap = Map{};
    auto index = ptrdiff_t{0};
    for (auto&& x : unique_values) {
        smap[x] = index++;
    }

    return pair{move(smap), move(unique_values)};

}