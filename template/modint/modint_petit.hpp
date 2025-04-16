#pragma once
#include "base_template.hpp"

// 剰余演算ライブラリ (小): 剰余と逆元だけ欲しいときに使う軽量ライブラリ
// mod pdiv 上での a^n (pdiv は素数である必要はない)
// pdiv * pdiv >= INT64_MAX のとき壊れる
constexpr ll modpow(ll a, ll n, const ll pdiv) {
    assert(pdiv >= 0 && pdiv <= numeric_limits<ll>::max() / pdiv);
    assert(n >= 0);
    a %= pdiv; if (a < 0) a += pdiv;
    ll result = 1;
    for (ll base = a; n; n >>= 1) {
        if (n & 1) (result *= base) %= pdiv;
        (base *= base) %= pdiv;
    }
    return result;
}
