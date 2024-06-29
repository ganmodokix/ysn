#pragma once
#include "base_template.hpp"
template <ranges::random_access_range R>
vector<size_t> argsort(R&& arr) {
    const auto size = ranges::size(arr);
    auto rank = vector<size_t>(size);
    ::std::iota(ALL(rank), size_t{0});
    const auto accessor = [&arr](const auto i) { return *(ranges::begin(arr) + i); };
    ranges::sort(rank, less{}, accessor);
    return rank;
}
