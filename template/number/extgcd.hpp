#pragma once
#include "base_template.hpp"

// 拡張ユークリッド互除法
// ax+by=gcd(a,b) の解 {gcd(a, b), x, y} を返す
// http://noshi91.hatenablog.com/entry/2019/04/01/184957
template<signed_integral T>
constexpr tuple<T, T, T> extgcd(const T a, const T b) {

    if ((a < 0 ? -a : a) < (b < 0 ? -b : b)) {
        const auto [g, y, x] = extgcd(b, a);
        return {g, x, y};
    }

    if (b == 0) {
        return {abs(a), a < 0 ? T{-1} : T{1}, T{0}};
    }
    
    auto s = a;
    auto xs = T{1};
    auto ys = T{0};
    auto t = b;
    auto xt = T{0};
    auto yt = T{1};

    while (s % t) {
        const auto temp = s / t;
        const auto u = s - t * temp;
        const auto xu = xs - xt * temp;
        const auto yu = ys - yt * temp;

        s = t; xs = xt; ys = yt;
        t = u; xt = xu; yt = yu;
    }
    
    if (t < 0) {
        t = -t;
        xt = -xt;
        yt = -yt;
    }
    return {t, xt, yt};
}
