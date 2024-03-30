#pragma once
#include "base_template.hpp"
// 高速ゼータ変換 x.size()は2べきである必要がある O(K2^K); K being |U|
// fzt(x)[s] == Σ_{t⊇s/t⊆s} x[t]
// inv: これをtrueにするとメビウス, subset: これをtrueにするとt⊆s
template<typename T>
vector<T> fzt(vector<T> r, const bool inv = false, const bool subset = false) {
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
    return r;
}
// AND/OR畳み込み
template <typename T, typename U>
auto convolve_fzt(T&& x, U&& y, const bool subset = false) {
    assert(x.size() == y.size());
    auto X = fzt(forward<T>(x), false, subset);
    const auto Y = fzt(forward<U>(y), false, subset);
    for (size_t i = 0; i < X.size(); i++) X[i] *= Y[i];
    return fzt(move(X), true, subset);
}
template <typename T, typename U>
auto convolve_and(T&& x, U&& y) {
    return convolve_fzt(forward<T>(x), forward<U>(y), false);
    }
template <typename T, typename U>
auto convolve_or(T&& x, U&& y) {
    return convolve_fzt(forward<T>(x), forward<U>(y), true);
}
