// Cooley-Tukey型 高速フーリエ変換 O(NlogN)
template <typename T>
void fft_inplace(vector<complex<T>> &a, const bool inv = false) {
    ll n = 1, h = 0;
    while (n < a.size()) n <<= 1, h++;
    a.resize(n, 0);
    // Cooley-Tukey sort
    for (size_t i = 0; i < n; i++) {
        size_t j = 0;
        for (size_t k = 0; k < h; k++) {
            j |= (i >> k & 1) << (h-1 - k);
        }
        if (i < j) swap(a[i], a[j]);
    }
    // butterfly diagram
    constexpr T dpi = static_cast<T>(acosl(-1) * 2);
    for (size_t b = 1; b < n; b <<= 1) {
        for (size_t j = 0; j < b; j++) {
            auto w = polar<T>(1, -dpi / (b << 1) * j * (inv ? -1 : 1));
            for (size_t k = 0; k < n; k += b << 1) {
                auto s = a[j+k  ];
                auto t = a[j+k+b] * w;
                a[j+k  ] = s+t;
                a[j+k+b] = s-t;
            }
        }
    }
    if (inv) {
        T inv_n = (T)1 / n;
        for (ll i = n; i--; ) a[i] *= inv_n;
    }
}
template <typename T>
inline vector<complex<T>> fft(vector<complex<T>> a, const bool inv = false) { fft_inplace(a, inv); return a; }
// long double配列同士の畳み込み　O((N+M)log(N+M))
template <typename T>
vector<T> convolve(const vector<T> &x, const vector<T> &y) {
    size_t m = x.size() + y.size() - 1;
    size_t t = 1; while(t < m) t <<= 1;
    vector<complex<T>> xc(t, 0), yc(t, 0);
    REP(i, x.size()) xc[i] = x[i];
    REP(i, y.size()) yc[i] = y[i];
    fft_inplace(xc, false);
    fft_inplace(yc, false);
    REP(i, t) xc[i] *= yc[i];
    fft_inplace(xc, true);
    vector<T> r(m, 0);
    REP(i, m) r[i] = xc[i].real();
    return r;
}