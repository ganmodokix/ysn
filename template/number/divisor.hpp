#pragma once
#include "base_template.hpp"
// 約数列挙 O(√N)
template <typename T>
constexpr vector<T> divisor(const T n) {
    if (n < 1) return vector<T>();
    vector<T> r;
    for (long long i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            r.push_back(i);
            if (i * i != n) r.push_back(n / i);
        }
    }
    return r;
}

template <typename T>
constexpr bool is_prime_by_trial_division(const T n) {
    return n >= 2 && divisor(n).size() == 2;
}