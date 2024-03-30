// #REQ: base_template.hpp
ll binom_naive(ll n, ll m) {
    static auto cache = map<pair<ll, ll>, ll>{};
    if (n < 0 || m < 0 || m > n) return 0;
    if (auto itr = cache.find({n, m}); itr != cache.end()) {
        return itr->second;
    }
    auto r = 1LL;
    const auto mm = min(m, n - m);
    REP(i, mm) r *= n - i;
    REP(i, mm) r /= i + 1;
    return cache[{n, m}] = r;
}

ll binom_lucas(ll n, ll m, ll p) {
    assert(p > 0);
    if (n < 0 || m < 0 || m > n || p == 1) return 0;
    auto r = 1LL;
    for (; n && m; n /= p, m /= p) {
        const auto ni = n % p;
        const auto mi = m % p;
        (r *= binom_naive(ni, mi)) %= p;
    }
    return r;
}