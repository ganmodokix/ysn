#pragma once
#include "base_template.hpp"
#include "modint/modint_petit_p.hpp"
#include "ranges_to.hpp"

// 剰余類環 \mathbb{Z}/n\mathbb{Z}
template <int64_t pdiv_, signed_integral Item = int64_t, signed_integral Extended = int64_t>
struct moduloint {
    
    using item_type = Item;  // 保持する型
    using extended_type = Extended;  // 乗算の計算途中で膨らみうる値のための型
    
    static constexpr auto pdiv = Extended{pdiv_};

    // テンプレートパラメータにできるリテラルクラス型にできるよう public にしているが基本 item() を使用すること
    Item x = 0;

    // 加減のオーバーフローチェック
    static_assert(pdiv - 1 <= numeric_limits<Item>::max() / 2);
    static_assert(numeric_limits<Item>::lowest() + pdiv - 1 <= 0);
    // 乗算がオーバーフローしないことを確認
    // モンゴメリ乗算があれば不要
    static_assert((pdiv - 1) <= numeric_limits<Extended>::max() / (pdiv - 1));
    
    constexpr moduloint(Extended _x = 0) noexcept: x(regularize(_x)) {}
    
    private:
    // unsafe ctor
    struct __regularized {};
    constexpr moduloint(Item _x, __regularized) noexcept: x(_x) {}
    public:
    
    static constexpr Item regularize(Extended x) noexcept {
        const auto y = x % pdiv;
        return static_cast<Item>(y < 0 ? y + pdiv : y);
    }
    static constexpr Item llpow(Item a, int64_t n) noexcept {
        return static_cast<Item>(modpow_p<pdiv>(a, n));
    }
    static constexpr Item llinv(Item a) noexcept {
        // return modinv_p<pdiv>(a);
        return modinv_extgcd(a, pdiv);
    }
    constexpr moduloint pow(int64_t n) const noexcept {
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
        x = Extended{x} * Extended{a.x} % pdiv;
        return *this;
    }
    constexpr moduloint& operator/= (const moduloint a) noexcept {
        x = Extended{x} * Extended{llinv(a.x)} % pdiv;
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
        return moduloint(Extended{x} * Extended{a.x} % pdiv, __regularized{});
    }
    constexpr moduloint operator/ (const moduloint a) const noexcept {
        return moduloint(Extended{x} * Extended{llinv(a.x)} % pdiv, __regularized{});
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
template <int64_t pdiv, integral T>
constexpr moduloint<pdiv> operator+ (const T a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) + b; }
template <int64_t pdiv, integral T>
constexpr moduloint<pdiv> operator- (const T a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) - b; }
template <int64_t pdiv, integral T>
constexpr moduloint<pdiv> operator* (const T a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) * b; }
template <int64_t pdiv, integral T>
constexpr moduloint<pdiv> operator/ (const T a, const moduloint<pdiv> b) noexcept { return moduloint<pdiv>(a) / b; }

// I/O 対応
template <int64_t pdiv>
ostream& operator<< (ostream& ost, const moduloint<pdiv> a) { return ost << a.item(); }
template <int64_t pdiv>
istream& operator>> (istream& ist, moduloint<pdiv>& a) { auto x = (typename moduloint<pdiv>::item_type){}; ist >> x; a = x; return ist; }

// DUMP()対応
template <int64_t pdiv>
ostream& __dump_single(const moduloint<pdiv> value) {
    return cerr << "\e[35m" << value.item() << "\e[2m_" << pdiv << "\e[m";
}

// ranges_to 対応
namespace ganmodokix {
    template <int64_t pdiv, integral To>
    struct _Converter<moduloint<pdiv>, To> {
        constexpr To operator() (const moduloint<pdiv>& x) const {
            return static_cast<To>(x.item());
        }
    };
}

// 剰余類環 modint であることを表すコンセプト
template <typename T>
concept mod_integral =
requires(const T a, const T b, const int64_t n) {
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
