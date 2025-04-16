#pragma once
#include "base_template.hpp"
#include "modint/modint_petit_p.hpp"

// 剰余類環 \mathbb{Z}/n\mathbb{Z}
template <long long pdiv_>
requires (pdiv_ >= 1)
struct moduloint {
private:
    long long x = 0;
public:
    static constexpr auto pdiv = pdiv_;
    constexpr moduloint(long long _x = 0) noexcept: x(regularize(_x)) {}
    static constexpr long long regularize(long long x) noexcept { x %= pdiv; x += pdiv; return x - (x >= pdiv ? pdiv : 0); }
    static constexpr long long regularize(moduloint a) noexcept { return a.x; }
    static constexpr long long llpow(long long a, long long n) noexcept { return modpow_p<pdiv>(a, n); }
    static constexpr long long llinv(long long a) noexcept { return modinv_p<pdiv>(a); }
    static constexpr long long llinv(moduloint a) noexcept { return modinv_p<pdiv>(a.x); }
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
    constexpr auto item() const { return x; }
};
template <long long pdiv>
constexpr moduloint<pdiv> operator+ (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) + b; }
template <long long pdiv>
constexpr moduloint<pdiv> operator- (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) - b; }
template <long long pdiv>
constexpr moduloint<pdiv> operator* (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) * b; }
template <long long pdiv>
constexpr moduloint<pdiv> operator/ (const long long a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) / b; }

// 剰余類環 modint であることを表すコンセプト
template <typename T>
concept mod_integral =
requires(const T a, const T b) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { a.item() } -> std::integral;
} &&
requires(T& a, const T b) {
    { a += b } -> std::convertible_to<T&>;
    { a -= b } -> std::convertible_to<T&>;
    { a *= b } -> std::convertible_to<T&>;
    { a /= b } -> std::convertible_to<T&>;
} &&
std::integral<decltype(T::pdiv)>;

static_assert(integral<ll>);
static_assert(mod_integral<moduloint<998244353>>);
static_assert(mod_integral<moduloint<(ll)1e9 + 7>>);
