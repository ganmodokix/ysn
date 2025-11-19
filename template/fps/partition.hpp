#pragma once
#include "base_template.hpp"
#include "conv/ntt.hpp"
#include "modint/modint.hpp"
#include "fps/exp.hpp"

// 分割数 \prod_{k=1}^\infty (1 - x^k) mod x^n
// https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE#toc15
template <mod_integral T>
vector<T> fps_partition(ll n) {
    assert(n >= 0);
    if (n == 0) return {};

    auto ans = vector(n, T{0});
    ans[0] = 1;
    RANGE(i, 1, n) {
        const auto k1 = i * (i * 3 - 1) / 2;
        if (k1 >= n) break;
        ans[k1] += i % 2 ? -1 : 1;
        const auto k2 = i * (i * 3 + 1) / 2;
        if (k2 >= n) continue;
        ans[k2] += i % 2 ? -1 : 1;
    }
    return ans;
}

// 部分和DP \prod_i (1 + x^{r_i}) mod x^n
// https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE#toc15
// verify: https://judge.yosupo.jp/submission/280214
template <mod_integral T, ranges::input_range R>
requires integral<ranges::range_value_t<R>>
constexpr vector<T> fps_subset_sum(R&& r, const ll n) {
    assert(n >= 0);
    if (n == 0) return {};
    auto a = vector<ll>(ALL(r));
    // \log(1+x) = \sum_{k=0}^\infty \frac{(-1)^k}{k+1} x^{k+1}
    if (a.empty()) return {1};
    ranges::sort(a);
    assert(a.front() >= 0);
    auto sumlog1px = vector(n, T{0});
    for (auto it = ranges::begin(a); it != ranges::end(a); ) {
        auto jt = it; while (jt != ranges::end(a) && *jt == *it) ++jt;
        const auto ai = static_cast<size_t>(*it);
        if (ai == 0) {
            it = jt;
            continue;
        }
        const auto num = T{distance(it, jt)};
        const auto nnum = -num;
        // \log[(1 + x^{a_i})^k] = k \log(1 + x^{a_i})
        for (auto k = size_t{1}; k * ai < static_cast<size_t>(n); ++k) {
            sumlog1px[k * ai] += (k % 2 ? num : nnum) / k;
        }
        it = jt;
    }
    return fps_exp(move(sumlog1px));
}