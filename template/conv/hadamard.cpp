// #REQ: base_template number/ceillog2.cpp

// 高速 Walsh-Hadamard 変換 O(N log N)
template <typename T>
vector<T> fht(vector<T> a, const bool inv = false) {
    assert(popcountll(a.size()) == 1);
    const auto n = static_cast<ll>(a.size());
    for (auto i = 1; i < n; i <<= 1) {
        for (auto j = n; (--j &= ~i) >= 0; ) {
            const auto u = a[j];
            const auto v = a[j + i];
            a[j  ] = u + v;
            a[j + i] = u - v;
        }
    }
    if (inv) {
        for (auto&& x : a) {
            x /= n;
        }
    }
    return a;
}
template <typename T, typename U>
auto convolve_xor(T&& x, U&& y) {
    assert(x.size() == y.size());
    auto a = fht(forward<T>(x));
    const auto b = fht(forward<U>(y));
    for (auto i = size_t{0}; i < a.size(); ++i) {
        a[i] *= b[i];
    }
    return fht(move(a), true);
}