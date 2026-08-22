#pragma once
#include "base_template.hpp"
// 累積和 Cumulative Sum
// モノイド (T, Op, Id) に対して s[i] = a[0] op a[1] op ... op[i-1] を前処理線形時間 O(N) クエリ定数時間 O(1) で求める
template <typename T, typename Op = ::std::plus<void>>
struct cumsum {
    // s[i]
    vector<T> s_;
    // op
    Op op_;

    // 構築時に前処理
    template <ranges::range R>
    cumsum(R&& r, Op op = {}, T id = {}): s_(), op_(op) {
        s_.emplace_back(move(id));
        if constexpr (ranges::sized_range<R>) {
            s_.reserve(ranges::size(r) + 1);
        }
        for (auto&& x : r) {
            s_.emplace_back(op_(s_.back(), forward<decltype(x)>(x)));
        }
    }

    // s[i] = op_{j=0}^{i-1} a[j]
    const T& sum_prefix(ptrdiff_t i) const {
        return s_.at(i);
    }

    // 可逆（＝モノイドではなく群）なら範囲opが O(1)で取れる
    template <typename Inv>
    requires requires(Inv inv, T a) {
        { inv(a) } -> convertible_to<T>;
    }
    T sum_range(ptrdiff_t l, ptrdiff_t r, Inv inv) const {
        return op_(inv(s_.at(l)), s_.at(r));
    }
};

// deduction guide
template <ranges::range R>
cumsum(R&&) -> cumsum<ranges::range_value_t<R>>;
template <ranges::range R, typename Op>
cumsum(R&&, Op) -> cumsum<ranges::range_value_t<R>, Op>;
