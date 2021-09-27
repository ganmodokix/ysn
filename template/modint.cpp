template<long long pdiv>
struct modint {
    long long x = 0;
    constexpr modint(long long _x = 0) noexcept: x(regularize(_x)) {}
    static constexpr long long regularize(long long x) noexcept { x %= pdiv; x += pdiv; return x - (x >= pdiv ? pdiv : 0); }
    static constexpr long long regularize(modint a) noexcept { return a.x; }
    static constexpr long long llpow(long long a, long long n) noexcept {
        a %= pdiv; if (a < 0) a += pdiv;
        n %= pdiv-1; if (n < 0) n += pdiv-1;
        long long result = 1;
        for (long long base = a; n; n >>= 1) {
            if (n & 1) (result *= base) %= pdiv;
            (base *= base) %= pdiv;
        }
        return result;
    }
    static constexpr long long llinv(long long a) noexcept { return llpow(a, pdiv-2); }
    static constexpr long long llinv(modint a) noexcept { return a.pow(pdiv-2).x; }
    constexpr modint pow(long long n) const noexcept { return modint(llpow(x, n)); }
    constexpr modint inv() const noexcept { return modint(llinv(x)); }
    constexpr modint& operator+= (modint a) noexcept { x +=        a.x; if (x >= pdiv) x -= pdiv; return *this; }
    constexpr modint& operator-= (modint a) noexcept { x += pdiv - a.x; if (x >= pdiv) x -= pdiv; return *this; }
    constexpr modint& operator*= (modint a) noexcept { (x *=       a.x ) %= pdiv; return *this; }
    constexpr modint& operator/= (modint a) noexcept { (x *= llinv(a.x)) %= pdiv; return *this; }
    template <typename T> constexpr modint& operator+= (T a) noexcept { return *this += modint(a); }
    template <typename T> constexpr modint& operator-= (T a) noexcept { return *this -= modint(a); }
    template <typename T> constexpr modint& operator*= (T a) noexcept { return *this *= modint(a); }
    template <typename T> constexpr modint& operator/= (T a) noexcept { return *this /= modint(a); }
    template <typename T> constexpr modint operator+ (T a) const noexcept { return modint(*this) += a; }
    template <typename T> constexpr modint operator- (T a) const noexcept { return modint(*this) -= a; }
    template <typename T> constexpr modint operator* (T a) const noexcept { return modint(*this) *= a; }
    template <typename T> constexpr modint operator/ (T a) const noexcept { return modint(*this) /= a; }
    constexpr modint operator- () const noexcept { return modint(pdiv - x); }
};
constexpr modint operator+ (const long long a, const modint b) noexcept { return modint(a) + b; }
constexpr modint operator- (const long long a, const modint b) noexcept { return modint(a) - b; }
constexpr modint operator* (const long long a, const modint b) noexcept { return modint(a) * b; }
constexpr modint operator/ (const long long a, const modint b) noexcept { return modint(a) / b; }
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