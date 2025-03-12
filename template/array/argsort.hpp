#pragma once
#include "base_template.hpp"
template <
    ranges::random_access_range R,
    typename Comp = ::std::less<void>,
    typename Proj = ::std::identity
>
vector<size_t> argsort(R&& r, Comp comp = {}, Proj proj = {}) {
    auto rank = vector(ranges::size(r), size_t{});
    iota(ALL(rank), 0);
    ranges::sort(rank, comp, [&proj, &r](const auto i) {
        return invoke(proj, r[i]);
    });
    return rank;
}
