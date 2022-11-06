// #REQ: base_template modint_petit.cpp garner.cpp conv/butterfly.cpp
// 精度に寄りけりだがconv1回で済むFFTの方がいい場合もあることに留意
// Cooley-Tukey型 高速フーリエ変換 O(NlogN)
template <ll pdiv = 998244353, ll prim = 3>
inline vector<ll> ntt(vector<ll> a, const bool inv = false) {
    if (inv) {
        butterfly_inv_inplace<pdiv, prim>(a);
    } else {
        butterfly_inplace<pdiv, prim>(a);
    }
    return a;
}
// 適切な原始根の存在する法での畳み込み
template <ll pdiv, ll prim>
vector<ll> convolve_p(const vector<ll> &x, const vector<ll> &y) {
    size_t t = x.size() + y.size() - 1;
    auto a = x; a.resize(t, 0);
    auto b = y; b.resize(t, 0);
    butterfly_inplace<pdiv, prim>(a);
    butterfly_inplace<pdiv, prim>(b);
    for (size_t i = 0; i < a.size(); i++) (a[i] *= b[i]) %= pdiv;
    butterfly_inv_inplace<pdiv, prim>(a);
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