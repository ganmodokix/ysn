// #REQ: modint_petit.cpp garner.cpp
// 精度に寄りけりだがconv1回で済むFFTの方がいい場合もあることに留意
// Cooley-Tukey型 高速フーリエ変換 O(NlogN)
template <ll pdiv = 998244353, ll prim = 3>
void ntt_inplace(vector<ll> &a, const bool inv = false) {
    // Shita-goshirae
    ll n = 1, h = 0;
    while (n < a.size()) n <<= 1, h++;
    a.resize(n, 0);
    for (size_t i = 0; i < n; i++) {
        ((a[i] %= pdiv) += pdiv) %= pdiv;
    }
    // Cooley-Tukey sort
    for (size_t i = 0; i < n; i++) {
        size_t j = 0;
        for (size_t k = 0; k < h; k++) {
            j |= (i >> k & 1) << (h-1 - k);
        }
        if (i < j) swap(a[i], a[j]);
    }
    // butterfly enzan
    for (size_t b = 1; b < n; b <<= 1) {
        const ll phase = (pdiv-1) / (b << 1);
        const ll root = modpow(prim, (inv ? phase * (pdiv-2) % (pdiv-1) : phase), pdiv);
        ll w = 1;
        for (size_t j = 0; j < b; j++) {
            for (size_t k = 0; k < n; k += b << 1) {
                ll s = a[j+k  ];
                ll t = a[j+k+b] * w % pdiv;
                a[j+k  ] = s + t; if (a[j+k  ] >= pdiv) a[j+k  ] -= pdiv;
                a[j+k+b] = s - t; if (a[j+k+b] < 0) a[j+k+b] += pdiv;
            }
            (w *= root) %= pdiv;
        }
    }
    if (inv) {
        const ll invn = modinv(n, pdiv);
        for (ll i = n; i--; ) (a[i] *= invn) %= pdiv;
    }
}
template <ll pdiv = 998244353, ll prim = 3>
inline vector<ll> ntt(vector<ll> a, const bool inv = false) { ntt_inplace<pdiv, prim>(a, inv); return a; }
// 適切な原始根の存在する法での畳み込み
template <ll pdiv, ll prim>
vector<ll> convolve_p(const vector<ll> &x, const vector<ll> &y) {
    size_t t = x.size() + y.size() - 1;
    auto a = x; a.resize(t, 0);
    auto b = y; b.resize(t, 0);
    ntt_inplace<pdiv, prim>(a);
    ntt_inplace<pdiv, prim>(b);
    for (size_t i = 0; i < a.size(); i++) (a[i] *= b[i]) %= pdiv;
    ntt_inplace<pdiv, prim>(a, true);
    a.resize(t);
    return a;
}
// 任意MODでの畳み込み演算
vector<ll> convolve(const vector<ll> &x, const vector<ll> &y, const ll pdiv) {
    if (pdiv == 998244353) return convolve_p<998244353, 3>(x, y);
    const auto conv1 = convolve_p<1224736769, 3>(x, y);
    // const auto conv2 = convolve_p<998244353, 3>(x, y);
    const auto conv3 = convolve_p<469762049, 3>(x, y);
    const auto conv4 = convolve_p<167772161, 3>(x, y);
    ll n = conv1.size();
    ll t = x.size() + y.size() - 1;
    vector<ll> result(t, 0);
    for (ll i = 0; i < t; i++) {
        vector<pair<ll, ll>> g = {
            {conv1[i], 1224736769},
            // {conv2[i], 998244353},
            {conv3[i], 469762049},
            {conv4[i], 167772161}
        };
        result[i] = garner(g, pdiv);
    }
    return result;
}