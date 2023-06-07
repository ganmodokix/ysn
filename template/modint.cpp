template <long long pdiv>
struct moduloint {
    long long x = 0;
    constexpr moduloint(long long _x = 0) noexcept: x(regularize(_x)) {}
    static constexpr long long regularize(long long x) noexcept { x %= pdiv; x += pdiv; return x - (x >= pdiv ? pdiv : 0); }
    static constexpr long long regularize(moduloint a) noexcept { return a.x; }
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
    static constexpr long long llinv(moduloint a) noexcept { return a.pow(pdiv-2).x; }
    constexpr moduloint pow(long long n) const noexcept { return moduloint(llpow(x, n)); }
    constexpr moduloint inv() const noexcept { return moduloint(llinv(x)); }
    constexpr moduloint& operator+= (moduloint a) noexcept { x +=        a.x; if (x >= pdiv) x -= pdiv; return *this; }
    constexpr moduloint& operator-= (moduloint a) noexcept { x += pdiv - a.x; if (x >= pdiv) x -= pdiv; return *this; }
    constexpr moduloint& operator*= (moduloint a) noexcept { (x *=       a.x ) %= pdiv; return *this; }
    constexpr moduloint& operator/= (moduloint a) noexcept { (x *= llinv(a.x)) %= pdiv; return *this; }
    template <typename T> constexpr moduloint& operator+= (T a) noexcept { return *this += moduloint(a); }
    template <typename T> constexpr moduloint& operator-= (T a) noexcept { return *this -= moduloint(a); }
    template <typename T> constexpr moduloint& operator*= (T a) noexcept { return *this *= moduloint(a); }
    template <typename T> constexpr moduloint& operator/= (T a) noexcept { return *this /= moduloint(a); }
    template <typename T> constexpr moduloint operator+ (T a) const noexcept { return moduloint(*this) += a; }
    template <typename T> constexpr moduloint operator- (T a) const noexcept { return moduloint(*this) -= a; }
    template <typename T> constexpr moduloint operator* (T a) const noexcept { return moduloint(*this) *= a; }
    template <typename T> constexpr moduloint operator/ (T a) const noexcept { return moduloint(*this) /= a; }
    constexpr moduloint operator- () const noexcept { return moduloint(pdiv - x); }
    friend constexpr bool operator==(const moduloint& x, const moduloint& y) { return regularize(x) == regularize(y); }
    friend constexpr bool operator!=(const moduloint& x, const moduloint& y) { return !(x == y); }
    static auto modulo() { return pdiv; }
};
template <long long pdiv>
constexpr moduloint<pdiv> operator+ (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) + b; }
template <long long pdiv>
constexpr moduloint<pdiv> operator- (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) - b; }
template <long long pdiv>
constexpr moduloint<pdiv> operator* (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) * b; }
template <long long pdiv>
constexpr moduloint<pdiv> operator/ (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) / b; }