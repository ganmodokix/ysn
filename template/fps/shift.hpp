#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"

// 多項式の平行移動 f(x+c)
// https://judge.yosupo.jp/problem/polynomial_taylor_shift
// https://x.com/risujiroh/status/1215710785000751104
// verify: https://judge.yosupo.jp/submission/280225
template <mod_integral T>
constexpr vector<T> polynomial_taylor_shift(vector<T> f, const T c) {
    const auto n = (ll)f.size();
    auto fac = T{1};
    auto expcx = vector(n, T{1});
    REP(i, n) {
        f[i] *= fac;
        fac *= i + 1;
        if (i + 1 == n) break;
        expcx[i + 1] = expcx[i] * c;
    }
    const auto invfacn = fac.inv();
    {
        auto invfac = invfacn;
        DSRNG(i, n-1, 0) {
            invfac *= i + 1;
            expcx[i] *= invfac;
        }
    }
    ranges::reverse(f);
    f = convolve_p(move(f), move(expcx));
    f.resize(n, T{0});
    {
        auto invfac = invfacn;
        REP(i, n) {
            invfac *= n - i;
            f[i] *= invfac;
        }
    }
    ranges::reverse(f);
    return f;
}