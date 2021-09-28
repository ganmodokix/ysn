// #REQ: modint
using modint = moduloint<998244353LL>;
constexpr modint operator"" _p(unsigned long long _x) noexcept { return modint(_x); }
ostream& operator<< (ostream& ost, const modint a) { return ost << a.x; }
istream& operator>> (istream& ist, modint &a) { return ist >> a.x; }
constexpr modint pow(modint a, long long n) noexcept { return a.pow(n); }
vector<modint> fac_cache = {1}, invfac_cache = {1};
void prepare_fac(long long nmax) {
    long long m0 = fac_cache.size() - 1;
    if (m0 < nmax) {
           fac_cache.resize(nmax+1);
        invfac_cache.resize(nmax+1);        
        for (long long n = m0+1; n <= nmax; n++) {
            fac_cache[n] = fac_cache[n-1] * n;
        }
        invfac_cache[nmax] = fac_cache[nmax].inv();
        for (long long n = nmax-1; n > m0; n--) {
            invfac_cache[n] = invfac_cache[n+1] * (n+1);
        }
    }
}
modint    fac(long long n) { prepare_fac(n); return    fac_cache[n]; }
modint invfac(long long n) { prepare_fac(n); return invfac_cache[n]; }
constexpr bool isinpascal(long long n, long long r) noexcept { return 0 <= r && r <= n; }
modint nCr(long long n, long long r) { return !isinpascal(n, r) ? 0_p : fac(n) * invfac(r) * invfac(n-r); }
modint nPr(long long n, long long r) { return !isinpascal(n, r) ? 0_p : fac(n) * invfac(n-r); }
modint nHr(long long n, long long r) { return nCr(n+r-1, r); }