#pragma once
#include "base_template.hpp"

// a * b mod 2^61-1
// https://qiita.com/keymoon/items/11fac5627672a6d6a9f6
constexpr uint64_t multiply_m61(uint64_t a, uint64_t b) {
    constexpr auto modulo = (uint64_t{1} << 61) - 1;
#ifdef __SIZEOF_INT128__
    return __uint128_t{a} * __uint128_t{b} % modulo;
#else
    constexpr auto mask30 = (uint64_t{1} << 30) - 1;
    constexpr auto mask31 = (uint64_t{1} << 31) - 1;
    constexpr auto mask61 = modulo;

    const auto au = a >> 31;
    const auto ad = a & mask31;
    const auto bu = b >> 31;
    const auto bd = b & mask31;
    const auto mid = ad * bu + au * bd;
    const auto midu = mid >> 30;
    const auto midd = mid & mask30;
    const auto x = au * bu * 2 + midu + (midd << 31) + ad * bd;

    const auto xu = x >> 61;
    const auto xd = x & mask61;

    auto res = xu + xd;
    if (res >= modulo) res -= modulo;
    return res;
#endif
}

constexpr uint64_t modpow_m61(const uint64_t x, uint64_t n) {
    auto result = uint64_t{1};
    for (uint64_t base = x; n; n >>= 1) {
        if (n & 1) {
            result = multiply_m61(result, base);
        }
        base = multiply_m61(base, base);
    }
    return result;
}

constexpr uint64_t modinv_m61(const uint64_t x) {
    assert(x != 0);
    constexpr auto modulo = (uint64_t{1} << 61) - 1;
    return modpow_m61(x, modulo - 2);
};

struct moduloint_m61 {
    
    // members
    using T = uint64_t;
    static constexpr T modulo = T{(1ULL << 61) - 1};
    T value = 0;

    // ctors
    constexpr moduloint_m61() = default;
    constexpr moduloint_m61(T x): value(x) {
        (value %= modulo) += modulo;
        if (value >= modulo) value -= modulo;
    }

    // methods
    constexpr moduloint_m61 inv() const {
        return {modinv_m61(value)};
    }

    // operators
    friend constexpr moduloint_m61 operator+(const moduloint_m61& a, const moduloint_m61 &b) {
        auto v = a.value + b.value;
        if (v >= modulo) v -= modulo;
        return {v};
    }
    friend constexpr moduloint_m61 operator-(const moduloint_m61& a, const moduloint_m61 &b) {
        auto v = a.value + modulo - b.value;
        if (v >= modulo) v -= modulo;
        return {v};
    }
    friend constexpr moduloint_m61 operator*(const moduloint_m61& a, const moduloint_m61 &b) {
        return {multiply_m61(a.value, b.value)};
    }
    friend constexpr moduloint_m61 operator/(const moduloint_m61& a, const moduloint_m61 &b) {
        return {multiply_m61(a.value, modinv_m61(b.value))};
    }
    friend ostream& operator<<(ostream& ost, const moduloint_m61& x) {
        return ost << x.value;
    }
    friend istream& operator>>(istream& ist, moduloint_m61& x) {
        auto value = uint64_t{0};
        ist >> value;
        x = moduloint_m61{value};
        return ist;
    }
};
