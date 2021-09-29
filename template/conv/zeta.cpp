// 高速ゼータ変換 x.size()は2べきである必要がある O(K2^K); K being |U|
// fzt(x)[s] == Σ_{t⊇s/t⊆s} x[t]
// inv: これをtrueにするとメビウス, subset: これをtrueにするとt⊆s
template<typename T>
void fzt_inplace(vector<T> &r, const bool inv = false, const bool subset = false) {
    const size_t m = r.size();
    size_t n = 0, nmask = 1; while (nmask < m) nmask <<= 1, n++;
    assert(nmask == m);
    for (size_t i = 0; i < n; i++) {
        const size_t imask = 1ULL << i;
        for (size_t j = 0; j < m; j++) {
            if (!subset) {
                if (!(j & imask)) {
                    if (inv) r[j] -= r[j | imask]; else r[j] += r[j | imask];
                }
            } else {
                if (j & imask) {
                    if (inv) r[j] -= r[j ^ imask]; else r[j] += r[j ^ imask];
                }
            }
        }
    }
}
template<typename T>
vector<T> fzt(const vector<T> &x, const bool inv = false, const bool subset = false) {
    vector<T> r = x;
    fzt_inplace(r, inv, subset);
    return r;
}
// AND/OR畳み込み
template <typename T>
vector<T> convolve_fzt(const vector<T> &x, const vector<T> &y, const bool subset = false) {
    assert(x.size() == y.size());
    vector<T> X = fzt(x, false, subset);
    vector<T> Y = fzt(y, false, subset);
    for (size_t i = 0; i < X.size(); i++) X[i] *= Y[i];
    return fzt(X, true, subset);
}
template <typename T> vector<T> convolve_and(const vector<T> &x, const vector<T> &y) { return convolve_fzt(x, y, false); }
template <typename T> vector<T> convolve_or (const vector<T> &x, const vector<T> &y) { return convolve_fzt(x, y, true ); }