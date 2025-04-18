#pragma once
#include "base_template.hpp"
#include "modint/modint_petit_p.hpp"
#include "ranges_to.hpp"

using moduloint_item_t = long signed long;

// 剰余類環 \mathbb{Z}/n\mathbb{Z}
template <moduloint_item_t pdiv_>
requires (pdiv_ >= 1)
struct moduloint {
    // テンプレートパラメータにできるリテラルクラス型にできるよう public にしているが基本 item() を使用すること
    moduloint_item_t x = 0;
    
    static constexpr auto pdiv = pdiv_;

    // 2乗がオーバーフローしないことを確認
    static_assert((pdiv - 1) <= numeric_limits<moduloint_item_t>::max() / (pdiv - 1));

    constexpr moduloint(moduloint_item_t _x = 0) noexcept: x(regularize(_x)) {}
    
    private:
    // unsafe ctor
    struct __regularized {};
    constexpr moduloint(moduloint_item_t _x, __regularized) noexcept: x(_x) {}
    public:
    
    static constexpr moduloint_item_t regularize(moduloint_item_t x) noexcept {
        const auto y = x % pdiv;
        return y < 0 ? y + pdiv : y;
    }
    static constexpr moduloint_item_t llpow(moduloint_item_t a, moduloint_item_t n) noexcept {
        return modpow_p<pdiv>(a, n);
    }
    static constexpr moduloint_item_t llinv(moduloint_item_t a) noexcept {
        // return modinv_p<pdiv>(a);
        return modinv_extgcd(a, pdiv);
    }
    constexpr moduloint pow(moduloint_item_t n) const noexcept {
        return moduloint(llpow(x, n), __regularized{});
    }
    constexpr moduloint inv() const noexcept {
        return moduloint(llinv(x), __regularized{});
    }
    constexpr moduloint& operator+= (const moduloint a) noexcept {
        x += a.x; if (x >= pdiv) x -= pdiv;
        return *this;
    }
    constexpr moduloint& operator-= (const moduloint a) noexcept {
        x -= a.x; if (x < 0) x += pdiv;
        return *this;
    }
    constexpr moduloint& operator*= (const moduloint a) noexcept {
        (x *= a.x) %= pdiv;
        return *this;
    }
    constexpr moduloint& operator/= (const moduloint a) noexcept {
        (x *= llinv(a.x)) %= pdiv;
        return *this;
    }
    template <integral T> constexpr moduloint& operator+= (const T a) noexcept {
        return *this += moduloint(a);
    }
    template <integral T> constexpr moduloint& operator-= (const T a) noexcept {
        return *this -= moduloint(a);
    }
    template <integral T> constexpr moduloint& operator*= (const T a) noexcept {
        return *this *= moduloint(a);
    }
    template <integral T> constexpr moduloint& operator/= (const T a) noexcept {
        return *this /= moduloint(a);
    }
    template <integral T> constexpr moduloint operator+ (const T a) const noexcept {
        return moduloint(*this) += a;
    }
    template <integral T> constexpr moduloint operator- (const T a) const noexcept {
        return moduloint(*this) -= a;
    }
    template <integral T> constexpr moduloint operator* (const T a) const noexcept {
        return moduloint(*this) *= a;
    }
    template <integral T> constexpr moduloint operator/ (const T a) const noexcept {
        return moduloint(*this) /= a;
    }
    constexpr moduloint operator+ (const moduloint a) const noexcept {
        const auto y = x + a.x;
        return moduloint(y >= pdiv ? y - pdiv : y, __regularized{});
    }
    constexpr moduloint operator- (const moduloint a) const noexcept {
        const auto y = x - a.x;
        return moduloint(y < 0 ? y + pdiv : y, __regularized{});
    }
    constexpr moduloint operator* (const moduloint a) const noexcept {
        return moduloint(x * a.x % pdiv, __regularized{});
    }
    constexpr moduloint operator/ (const moduloint a) const noexcept {
        return moduloint(x * llinv(a.x) % pdiv, __regularized{});
    }
    
    constexpr moduloint operator- () const noexcept {
        return moduloint(x ? pdiv - x : 0, __regularized{});
    }
    friend constexpr bool operator==(const moduloint a, const moduloint b) noexcept {
        return a.x == b.x;
    }
    friend constexpr bool operator!=(const moduloint a, const moduloint b) noexcept {
        return !(a == b);
    }
    constexpr auto item() const noexcept { return x; }
};
template <moduloint_item_t pdiv>
constexpr moduloint<pdiv> operator+ (const moduloint_item_t a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) + b; }
template <moduloint_item_t pdiv>
constexpr moduloint<pdiv> operator- (const moduloint_item_t a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) - b; }
template <moduloint_item_t pdiv>
constexpr moduloint<pdiv> operator* (const moduloint_item_t a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) * b; }
template <moduloint_item_t pdiv>
constexpr moduloint<pdiv> operator/ (const moduloint_item_t a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) / b; }

// I/O 対応
template <ll pdiv>
ostream& operator<< (ostream& ost, const moduloint<pdiv> a) { return ost << a.item(); }
template <ll pdiv>
istream& operator>> (istream& ist, moduloint<pdiv>& a) { auto x = moduloint_item_t{}; ist >> x; a = x; return ist; }

// DUMP()対応
template <ll pdiv>
ostream& __dump_single(const moduloint<pdiv> value) {
    return cerr << "\e[35m" << value.item() << "\e[2m_" << pdiv << "\e[m";
}

// ranges_to 対応
namespace ganmodokix {
    template <ll pdiv, integral To>
    struct _Converter<moduloint<pdiv>, To> {
        constexpr To operator() (const moduloint<pdiv>& x) const {
            return static_cast<To>(x.item());
        }
    };
}

// 剰余類環 modint であることを表すコンセプト
template <typename T>
concept mod_integral =
requires(const T a, const T b, ll n) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { -a } -> std::convertible_to<T>;
    { a.pow(n) } -> std::convertible_to<T>;
    { a.inv() } -> std::convertible_to<T>;
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
