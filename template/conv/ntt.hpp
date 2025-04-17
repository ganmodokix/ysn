#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "garner.hpp"
#include "conv/butterfly.hpp"
#include "number/primitive_root.hpp"
#include "number/miller_rabin.hpp"
#include "ranges_to.hpp"

// 精度に寄りけりだがconv1回で済むFFTの方がいい場合もあることに留意
// Cooley-Tukey型 高速フーリエ変換 O(NlogN)
// 原始根 prim_ が指定されない場合は最小の原始根が指定される
template <typename T, ll prim_ = -1>
requires mod_integral<typename T::value_type>
auto ntt(T&& a, const bool inv = false) -> vector<typename T::value_type> {
    using V = typename T::value_type;
    constexpr auto prim = V{prim_ != -1 ? prim_ : primitive_root(V::pdiv)};
    if (inv) {
        return butterfly_inv<V, prim>(forward<T>(a));
    } else {
        return butterfly<V, prim>(forward<T>(a));
    }
}
template <mod_integral T, ll prim_ = -1>
auto intt(T&& a) -> vector<T> {
    return ntt<T, prim_>(forward<T>(a), true);
}

// \mathbb{F}_{m} での畳み込み
template <ranges::random_access_range X, ranges::random_access_range Y>
requires convertible_to<ranges::range_value_t<X>, ranges::range_value_t<Y>>
auto convolve_p(X&& x, Y&& y)
    -> vector<remove_cvref_t<ranges::range_value_t<X>>>
{
    using T = remove_cvref_t<ranges::range_value_t<X>>;

    assert(!x.empty() && !y.empty());
    const auto t = x.size() + y.size() - 1;
    
    // 長くない方の長さが60以下の場合は (ACL参照) naiveの方が logN の分速い
    if (min(x.size(), y.size()) <= size_t{60}) {
        auto r = vector(t, T{0});
        REP(i, x.size()) REP(j, y.size()) {
            r[i + j] += x[i] * y[j];
        }
        return r;
    }
    
    const auto h = bit_ceil(t);
    auto a = forward<X>(x); a.resize(h, T{0});
    auto b = forward<Y>(y); b.resize(h, T{0});
    constexpr auto prim = primitive_root(T::pdiv);
    a = butterfly<T, prim>(move(a));
    b = butterfly<T, prim>(move(b));
    for (const auto i : views::iota(size_t{0}, a.size())) {
        a[i] *= b[i];
    }
    a = butterfly_inv<T, prim>(move(a));
    a.resize(t);
    return a;
}

// 任意MODでの畳み込み演算
// pdiv は必ずしも素数である必要はない
template <ranges::forward_range X, ranges::forward_range Y>
requires
    convertible_to<ranges::range_value_t<X>, ll> &&
    convertible_to<ranges::range_value_t<Y>, ll>
vector<ll> convolve(X&& x, Y&& y, const ll pdiv) {
    // logN 分より短ければ naive
    if constexpr (ranges::sized_range<X> && ranges::sized_range<Y>) {
        constexpr auto naive_size = size_t{60};
        if (ranges::size(x) <= naive_size || ranges::size(y) <= naive_size) {
            const auto regularize = [pdiv](const ll x) {
                const auto y = x % pdiv;
                return y + (y >= 0 ? 0 : pdiv);
            };
            const auto xval = x | views::transform(regularize) | to_vector<ll>{};
            const auto yval = y | views::transform(regularize) | to_vector<ll>{};
            assert(!xval.empty() && !yval.empty());
            const auto t = xval.size() + yval.size() - 1;
            auto result = vector(t, 0LL);
            REP(i, xval.size()) RPE(j, yval.size()) {
                result[i + j] += xval[i] * yval[j] % pdiv;
                if (result[i + j] >= pdiv) {
                    result[i + j] -= pdiv;
                }
            }
            return result;
        }
    }

    using T1 = moduloint<1224736769LL>;
    using T2 = moduloint<469762049LL>;
    using T3 = moduloint<167772161LL>;

    const auto conv1 = convolve_p(x | to_vector<T1>{}, y | to_vector<T1>{});
    const auto conv2 = convolve_p(x | to_vector<T2>{}, y | to_vector<T2>{});
    const auto conv3 = convolve_p(x | to_vector<T3>{}, y | to_vector<T3>{});

    const auto t = conv1.size();
    auto result = vector(t, 0LL);
    for (auto i = size_t{0}; i < t; i++) {
        result[i] = garner(
            array{conv1[i].item(), conv2[i].item(), conv3[i].item()},
            array{T1::pdiv, T2::pdiv, T3::pdiv},
            pdiv
        );
    }
    return result;
}
