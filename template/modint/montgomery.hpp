#pragma once
#include "base_template.hpp"
#include "modint/modint_petit_p.hpp"

// t * n^{-1} mod 2^32
template <uint64_t n>
constexpr uint64_t montgomery_reduction(const uint64_t t) {
    constexpr auto r = uint64_t{1} << 32;
    constexpr auto ninv = static_cast<uint64_t>((r - modinv_extgcd(n, r)) % (r - 1));
    const auto m = ((t & (r - 1)) * ninv) & (r - 1);
    const auto t2 = (t + m * n) >> 32;
    return t2 - (t2 >= n ? n : 0);
}

// a * b * (2^32) mod N を % なしで求める
template <uint64_t n>
constexpr uint64_t montgomery_prod(const uint64_t a, const uint64_t b) {
    constexpr auto r = uint64_t{1} << 32;
    constexpr auto rmn = r % n;
    constexpr auto r2mn = rmn * rmn % n;
    return montgomery_reduction<n>(
        montgomery_reduction<n>(a * r2mn) * montgomery_reduction<n>(b * r2mn)
    );
}