#pragma once
#include "base_template.hpp"
// Binary Indexed Tree (Fenwick tree)
// (T, Add) は可換群である必要がある
// Negate(T) -> T: 逆元
// ZERO: 単位元
template <typename T, typename Add = std::plus<>, typename Negate = std::negate<>, T ZERO = T{}>
class BIT {

private:
    vector<T> val;
    Add addition;
    Negate negate;

public:
    size_t n;
    
    BIT(size_t _n, Add add_ = {}, Negate negate_ = {})
        : val(_n + 1, ZERO), addition(add_), negate(negate_), n(_n) {}

    template <ranges::input_range R>
    requires ranges::sized_range<R>
    BIT(R&& r, Add add_ = {}, Negate negate_ = {})
        : val(ranges::size(r) + 1, ZERO), addition(add_), negate(negate_), n(val.size() - 1)
    {
        auto i = size_t{0};
        for (auto&& x : r) {
            addition(i++, forward<decltype(x)>(x));
        }
    }

    // a[i] に v を足す O(logN)
    void add(const size_t _i, T v) {
        assert(_i < n);
        for (auto i = _i + 1; i <= n; i += i & (-i)) {
            val[i] = addition(move(val[i]), v);
        }
    }

    // [0, _i) の和を求める O(logN)
    T get(ptrdiff_t i) const {
        chmax(i, 0);
        chmin(i, static_cast<ptrdiff_t>(n));
        auto ans = ZERO;
        for (auto j = static_cast<size_t>(i); j > 0; j ^= j & (-j)) {
            ans = addition(move(ans), val[j]);
        }
        return ans;
    }
    // [l, r) の和を求める o(N)
    T sum(ptrdiff_t l, ptrdiff_t r) {
        chmax(l, 0);
        chmin(r, static_cast<ptrdiff_t>(n));
        return addition(get(static_cast<T>(r)), negate(get(static_cast<T>(l))));
    }
};
