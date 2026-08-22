#pragma once
#include "base_template.hpp"
// (Z/2Z)^hoge上のXOR基底, raw_valueが真の場合aから選んで線形独立なベクトルを返す
// https://twitter.com/noshi91/status/1200702280128856064
template <typename T, ranges::input_range R>
vector<T> xor_basis(R&& a, bool raw_value = false)
{
    // noshi基底 O(N)
    auto result = vector<T>{};
    auto bases = vector<T>{};
    if constexpr (ranges::sized_range<R>) {
        result.reserve(ranges::size(a));
        bases.reserve(ranges::size(a));
    }
    for (const T d : a) {
        auto e = T{d};
        for (const T b : bases) e = min(e, static_cast<T>(e ^ b));
        if (e) {
            bases.push_back(e);
            result.push_back(raw_value ? d : e);
        }
    }
    return result;
}

template <ranges::input_range R>
auto xor_basis(R&& a, bool raw_value = false)
    -> vector<ranges::range_value_t<R>>
{
    return xor_basis<ranges::range_value_t<R>>(
        forward<R>(a), raw_value
    );
}
