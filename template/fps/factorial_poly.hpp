#pragma once
#include "modint/modint.hpp"
#include "fps/linear_prod.hpp"

// 不定元 x の階乗冪 x(x+step) ... (x+step*(k-1)) の多項式を求める O(K(logK)^2)
// \prod_{i=0}^{k-1} (x + i * \mathrm{step})
template <mod_integral T>
constexpr vector<T> factorial_poly(const ll k, const T step) {
    return linear_prod<T>(
        views::iota(0LL, k)
        | views::transform([&step](const T x) { return x * step; })
    );
}

// 不定元 x の上昇階乗冪 x^\bar{k} = x(x+1) ... (x+k-1) の多項式を求める O(K(logK)^2)
// result[i] = 1st-kind Starling (k, i)
template <mod_integral T>
constexpr vector<T> raising_factorial_poly(const ll k) {
    return factorial_poly<T>(k, T{1});
}

// 不定元 x の下降階乗冪 x^\bar{k} = x(x-1) ... (x-k+1) の多項式を求める O(K(logK)^2)
// \prod_{i=0}^{k-1} (x - i * \mathrm{step})
// result[i] = (-1)^{k-1-i} 1st-kind Starling (k, i)
template <mod_integral T>
constexpr vector<T> falling_factorial_poly(const ll k) {
    return factorial_poly<T>(k, T{-1});
}