// #REQ: base_template.hpp modint_petit.hpp
// a^x === b mod p を満たす最小の非負整数 x を返す
// {そのような x が存在するか, 存在する場合は最小の x}
// p は合成数でもok
// 参考: https://ei1333.github.io/luzhiled/snippets/math/mod-log.html
// 参考: https://qiita.com/suisen_cp/items/d597c8ec576ae32ee2d7
pair<ll, ll> modlog(ll a, ll b, ll p) {
    
    // g <- gcd(a^(ceil(log2(p))) mod p, p)
    ll g = 1;
    for (ll i = p; i; i /= 2) (g *= a) %= p;
    g = gcd(g, p);

    // d := ceil(log2(p)) 未満の x に対して a^x === b mod p が成り立たなければ a^d / g に逆元が存在
    ll t = 1, d = 0;
    for (; t % g; d++) {
        if (t == b) return {true, d};
        (t *= a) %= p;
    }
    if (b % g) return {false, -1};

    // a^x === (b/g) (x^d/g)^-1
    t /= g;
    b /= g;

    // BSGS, 最初にdだけ下駄をはいているので a^(im-j) == b/g mod p/g でok
    ll n = p / g, h = 0, gs = 1;
    for(; h * h < n; h++) (gs *= a) %= n;
    
    // baby step
    unordered_map<ll, ll> bs;
    for (ll s = 0, e = b; s < h; bs[e] = ++s) {
        (e *= a) %= n;
    }

    // giant step
    for(ll s = 0, e = t; s < n;) {
        (e *= gs) %= n;
        s += h;
        if (auto itr = bs.find(e); itr != bs.end()) {
            return {true, d + s - itr->second};
        }
    }

    return {false, -1};

}