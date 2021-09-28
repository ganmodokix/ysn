// #REQ: modint
using modint = moduloint<998244353LL>;

constexpr modint operator"" _p(unsigned long long _x) noexcept { return modint(_x); }
ostream& operator<< (ostream& ost, const modint a) { return ost << a.x; }
istream& operator>> (istream& ist, modint &a) { return ist >> a.x; }

vector<modint> fac_cache = {1_p}, invfac_cache = {1_p};
void prepare_fac(long long nmax) {

    if (nmax < (long long)fac_cache.size()) return;
    while (!(nmax < (long long)fac_cache.size())) {
        fac_cache.emplace_back(fac_cache.back() * fac_cache.size());
    }

    long long m0 = fac_cache.size();
    invfac_cache.resize(nmax+1);
    invfac_cache[nmax] = fac_cache[nmax].inv();
    for (long long n = nmax; --n >= m0; ) {
        invfac_cache[n] = invfac_cache[n+1] * (n+1);
    }

}

modint    fac(long long n) { prepare_fac(n); return    fac_cache[n]; }
modint invfac(long long n) { prepare_fac(n); return invfac_cache[n]; }
constexpr bool isinpascal(long long n, long long r) noexcept { return 0 <= r && r <= n; }
modint nCr(long long n, long long r) { return !isinpascal(n, r) ? 0_p : fac(n) * invfac(r) * invfac(n-r); }
modint nPr(long long n, long long r) { return !isinpascal(n, r) ? 0_p : fac(n) * invfac(n-r); }
modint nHr(long long n, long long r) { return nCr(n+r-1, r); }
modint catalan(long long n) { return nCr(n * 2, n) / (n + 1); }