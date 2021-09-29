// 剰余演算ライブラリ (小): 剰余と逆元だけ欲しいときに使う軽量ライブラリ
// mod pdiv 上での a^n (pdiv は素数である必要はない)
ll modpow(const ll a, ll n, const ll pdiv) {
    if (a == 0) return 0;
    ll r = 1;
    for (ll b = (a % pdiv + pdiv) % pdiv; n; n >>= 1, (b *= b) %= pdiv) if (n & 1) (r *= b) %= pdiv;
    return r;
}
// mod pdiv 上での逆元を求める (pdiv は素数である必要が**ある**, 素数じゃなかったらACLを使う)
ll modinv(const ll a, const ll pdiv) { return modpow(a, pdiv-2, pdiv); }