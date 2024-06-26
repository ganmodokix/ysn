#pragma once
#include "base_template.hpp"

// ceil(log_2(n)) を求める。n以上の2冪を求めるのに使う
template <typename T, typename enable_if<is_integral_v<T>, nullptr_t>::type = nullptr>
constexpr T ceillog2(T n) {
    assert(n <= numeric_limits<T>::max() / 2);
    T h = 1, r = 0;
    while (h < n) {
        h <<= 1;
        r++;
    }
    return r;
}
