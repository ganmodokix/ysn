#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "modint/modint_factorial_cache.hpp"

// moduloint<PDIV> を modint 型として定義する
// 2回以上呼び出すとコンパイルエラーになるはず
#define USING_MODINT(PDIV) \
    using modint = moduloint<(PDIV)>; \
    \
    /* Literal */ \
    constexpr modint operator""_p(unsigned long long _x) noexcept { return modint(_x); } \
    \
    /* Factorial n! */ \
    factorial_cache<modint> fac_cache; \
    modint    fac(long long n) { fac_cache.prepare(n); return fac_cache.fac[n]; } \
    modint invfac(long long n) { fac_cache.prepare(n); return fac_cache.invfac[n]; } \
    constexpr bool isinpascal(long long n, long long r) noexcept { return 0 <= r && r <= n; } \
    modint nCr(long long n, long long r) { return !isinpascal(n, r) ? 0_p : fac(n) * invfac(r) * invfac(n-r); } \
    modint nPr(long long n, long long r) { return !isinpascal(n, r) ? 0_p : fac(n) * invfac(n-r); } \
    modint nHr(long long n, long long r) { return nCr(n+r-1, r); } \
    modint catalan(long long n) { return nCr(n * 2, n) / (n + 1); }
