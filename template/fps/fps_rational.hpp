#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/berlekamp_massey.hpp"

// P(x)/Q(x) = s(x) mod x^n なる P, Q を求める O(N^2)
// Q(x) の次数を最小とする
// C-recursive の初項 s から線形漸化式を求めるのに相当する
template <mod_integral T>
pair<vector<T>, vector<T>> fps_rational(vector<T> s) {

    auto denominator = berlekamp_massey(s);

    // 初項 s(x) mod x^#s, 分母 f(x) が求まっているときに分子 A(x) を求める
    // a(x)/f(x) = s(x) mod x^#s ==> a(x) = s(x)f(x) mod x^#s
    const auto sz = s.size();
    auto numerator = convolve_p(move(s), denominator);
    numerator.resize(sz, T{0});
    while (!numerator.empty() && numerator.back() == 0) {
        numerator.pop_back();
    }
    return pair{move(numerator), move(denominator)};
}
