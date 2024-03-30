// #REQ: base_template
// 拡張ユークリッド互除法
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (abs(a) < abs(b)) return extgcd(b, a, y, x);
    if (b) {
        ll g = extgcd(b, a%b, y, x);
        y -= a / b * x;
        return g;
    } else {
        x = 1, y = 0;
        return a;
    }
}
// mod p での a のモジュラ逆元、存在しない場合-1が返る
ll modinv(ll a, ll p) {
    ll x, y; ll g = extgcd(a, p, x, y);
    return g == 1 ? (x%p+p)%p : -1;
}