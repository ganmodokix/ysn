#pragma once
#include "base_template.hpp"
// 素因数分解 O(√N)
template <typename T>
vector<T> factorize(T x) {
    if (x <= 1) return vector<T>();
    vector<T> r; T x0 = x;
    while (!(x & 1))
        x >>= 1, r.push_back(2);
    for (long long f = 3; f * f <= x0; f += 2)
        while (x % f == 0)
            x /= f, r.push_back(f);
    if (x > 1) r.push_back(x);
    return r;
}
