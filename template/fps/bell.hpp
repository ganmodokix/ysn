#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/exp.hpp"

// ベル数 (B_i)_{i=0}^{n-1} を列挙 O(NlogN)
// 指数型母関数が exp(exp(x) - 1)
// verify: https://judge.yosupo.jp/submission/280231
template <mod_integral T>
constexpr vector<T> bell_numbers(size_t n) {

    // exp(x) - 1
    auto expxm1 = vector(n, T{1});
    auto invfac = T{1};
    RANGE(i, 2, n) invfac *= i;
    invfac = invfac.inv();
    DSRNG(i, n-1, 0) {
        expxm1[i] = (invfac *= i + 1);
    }
    expxm1[0] = 0;

    // 指数型母関数なのでi!を掛ければ係数が得られる
    auto bell = fps_exp(move(expxm1));
    auto fac = T{1};
    REP(i, n) {
        bell[i] *= fac;
        fac *= i + 1;
    }
    return bell;
    
}
