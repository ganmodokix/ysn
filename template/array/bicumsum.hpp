#pragma once
#include "base_template.hpp"
#include "array/cumsum.hpp"
// 双方向累積和 Bidirectional Cumulative Sum
// モノイド (T, Op, Id) に対して前方・後方の累積演算を前処理線形時間 O(N) で求める
template <typename T, typename Op = ::std::plus<void>>
struct bicumsum {
    struct reverse_op {
        Op op_;

        T operator()(const T& lhs, const T& rhs) {
            return op_(rhs, lhs);
        }
    };

    Op op_;
    cumsum<T, Op> forward_cs_;
    cumsum<T, reverse_op> backward_cs_;

    template <ranges::bidirectional_range R>
    requires ranges::common_range<R>
    bicumsum(R&& r, Op op = {}, T id = {}):
        op_(move(op)),
        forward_cs_(r, op_, id),
        backward_cs_(views::reverse(r), reverse_op{op_}, move(id))
    {}

    // prefix [0, i) の総 op: op(a[0], ..., a[i - 1])
    const T& sum_prefix(ptrdiff_t i) const {
        return forward_cs_.sum_prefix(i);
    }

    // suffix [i, N) の総 op: op(a[i], ..., a[n - 1])
    const T& sum_suffix(ptrdiff_t i) const {
        return backward_cs_.sum_prefix(
            ssize(backward_cs_.s_) - 1 - i
        );
    }

    // a[i] を除いた総 op: op(a[0], ..., a[i - 1], a[i + 1], ..., a[n - 1])
    T sum_except(ptrdiff_t i) const {
        return op_(sum_prefix(i), sum_suffix(i + 1));
    }
};

// deduction guide
template <ranges::bidirectional_range R>
requires ranges::common_range<R>
bicumsum(R&&) -> bicumsum<ranges::range_value_t<R>>;
template <ranges::bidirectional_range R, typename Op>
requires ranges::common_range<R>
bicumsum(R&&, Op) -> bicumsum<ranges::range_value_t<R>, Op>;
