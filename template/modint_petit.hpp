// #REQ: base_template.hpp
// 剰余演算ライブラリ (小): 剰余と逆元だけ欲しいときに使う軽量ライブラリ
// mod pdiv 上での a^n (pdiv は素数である必要はない)
constexpr ll modpow(ll a, ll n, const ll pdiv) {
    a %= pdiv; if (a < 0) a += pdiv;
    n %= pdiv-1; if (n < 0) n += pdiv-1;
    ll result = 1;
    for (ll base = a; n; n >>= 1) {
        if (n & 1) (result *= base) %= pdiv;
        (base *= base) %= pdiv;
    }
    return result;
}
// mod pdiv 上での逆元を求める (pdiv は素数である必要が**ある**, 素数じゃなかったらACLを使う)
constexpr ll modinv(const ll a, const ll pdiv) { return modpow(a, pdiv-2, pdiv); }