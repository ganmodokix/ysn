#pragma once
#include "base_template.hpp"
#include "number/divisor.hpp"
#include "modint_petit.hpp"
#include "number/miller_rabin.hpp"

// 剰余演算ライブラリ (小): 剰余と逆元だけ欲しいときに使う軽量ライブラリ
// mod pdiv 上での a^n (pdiv は素数である必要が**ある**)
// フェルマーの小定理を用いて負のべき乗も計算可能としたもの
template <ll pdiv>
requires (is_prime(pdiv)) && (pdiv - 1 <= numeric_limits<ll>::max() / (pdiv - 1))
constexpr ll modpow_p(ll a, ll n) {
    a %= pdiv; if (a < 0) a += pdiv;
    n %= pdiv - 1; if (n < 0) n += pdiv - 1;
    ll result = 1;
    for (ll base = a; n; n >>= 1) {
        if (n & 1) (result *= base) %= pdiv;
        (base *= base) %= pdiv;
    }
    return result;
}

// mod pdiv 上での逆元を求める (pdiv は素数である必要が**ある**, 素数じゃなかったらACLを使う)
constexpr ll modinv(const ll a, const ll pdiv) { return modpow(a, pdiv-2, pdiv); }

// 素数moduloがコンパイル時に分かっている場合
template <ll pdiv>
constexpr ll modinv_p(const ll a) {
    return modpow_p<pdiv>(a);
}