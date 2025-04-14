#pragma once
#include "base_template.hpp"

namespace ganmodokix::floor_sum_detail {
    template <integral T>
    using promoted_ll_t = conditional_t<
        unsigned_integral<T>,
        #if defined(__SIZEOF_INT128__)
            __uint128_t
        #else
            uint_fast64_t
        #endif
        ,
        #if defined(__SIZEOF_INT128__)
            __int128_t
        #else
            int_fast64_t
        #endif
    >;

    template <integral T>
    constexpr T floor_div(T a, T b) {
        if constexpr (signed_integral<T>) {
            if (b < 0) {
                a *= -1, b *= -1;
            }
            if (a < 0) {
                return -((-a - T{1}) / b + T{1});
            }
        }
        return a / b;
    }
};

// Floor-sum $\sum_{k=0}^{n-1} \lfloor \frac{ak+b}{m} \rfloor$
// https://github.com/atcoder/ac-library/pull/88
// https://atcoder.jp/contests/practice2/editorial/579
template<typename T>
constexpr auto floor_sum(const T n, const T m, T a, T b)
    -> ganmodokix::floor_sum_detail::promoted_ll_t<T>
{
    using namespace ganmodokix::floor_sum_detail;
    using U = promoted_ll_t<T>;
    
    assert(n >= 0);
    assert(m != 0);

    if constexpr (signed_integral<T>) {
        if (m < 0) return floor_sum(n, -m, -a, -b);
    }

    auto ans = U{0};
    if (a < 0 || a >= m) {
        const auto quot = floor_div<U>(a, m);
        ans += U{n} * (n - 1) / 2 * quot;
        a -= quot * m;
    }
    if (b < 0 || b >= m) {
        const auto quot = floor_div<U>(b, m);
        ans += U{n} * quot;
        b -= quot * m;
    }
    if (a == 0) {
        return ans + b / m;
    }
    
    assert(0 < a && a < m);
    assert(0 <= b && b < m);

    const auto w = U{a} * n + b;
    const auto y = w / m;
    const auto z = w - y * m;

    return ans + floor_sum<U>(y, a, m, z);
}