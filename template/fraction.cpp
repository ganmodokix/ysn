struct fraction {
    ll p = 0, q = 1;
    fraction() = default;
    constexpr fraction(ll p_, ll q_ = 1) noexcept: p(p_), q(q_) { regularize(); }
    constexpr void regularize() noexcept { if (ll g = gcd(p, q); g) { p /= g; q /= g; } if (q < 0) { p *= -1; q *= -1; } }
    constexpr fraction& operator*= (fraction a) noexcept { p *= a.p; q *= a.q; regularize(); return *this; }
    constexpr fraction& operator/= (fraction a) noexcept { p *= a.q; q *= a.p; regularize(); return *this; }
    constexpr fraction& operator+= (fraction a) noexcept { ll x = q * a.q; p = p * a.q + q * a.p; q = x; regularize(); return *this; }
    constexpr fraction& operator-= (fraction a) noexcept { ll x = q * a.q; p = p * a.q - q * a.p; q = x; regularize(); return *this; }
    constexpr fraction operator+ (fraction b) { auto x = *this; return x += b; }
    constexpr fraction operator- (fraction b) { auto x = *this; return x -= b; }
    constexpr fraction operator* (fraction b) { auto x = *this; return x *= b; }
    constexpr fraction operator/ (fraction b) { auto x = *this; return x /= b; }
    constexpr fraction& operator*= (ll a) noexcept { return *this *= fraction(a); }
    constexpr fraction& operator/= (ll a) noexcept { return *this /= fraction(a); }
    constexpr fraction& operator+= (ll a) noexcept { return *this += fraction(a); }
    constexpr fraction& operator-= (ll a) noexcept { return *this -= fraction(a); }
    constexpr fraction operator* (ll a) noexcept { auto x = *this; return x *= fraction(a); }
    constexpr fraction operator/ (ll a) noexcept { auto x = *this; return x /= fraction(a); }
    constexpr fraction operator+ (ll a) noexcept { auto x = *this; return x += fraction(a); }
    constexpr fraction operator- (ll a) noexcept { auto x = *this; return x -= fraction(a); }
    constexpr fraction operator- () const noexcept { return fraction(-p, q); }
    constexpr bool operator< (const fraction &a) const noexcept { return p * other.q < a.p * q; }
    constexpr bool operator<= (const fraction &a) const noexcept { return p * other.q <= a.p * q; }
};
constexpr fraction operator"" _f(unsigned long long _x) noexcept { return fraction(_x); }
ostream& operator<< (ostream& ost, const fraction a) { return ost << a.p << "/" << a.q; }
istream& operator>> (istream& ist, fraction &a) { a.q = 1; return ist >> a.p; }
