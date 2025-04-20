#pragma once
#include "base_template.hpp"

// Fourier meets Möbius (subset convloution, 非交和OR畳み込み)
// result[S] = \sum_{T \subseteq S} x[S \setminus T]y[T] = \sum_{T \sqcup U} x[T][U]
// https://arxiv.org/abs/cs/0611101
// verify: https://judge.yosupo.jp/submission/280169
template <ranges::random_access_range X, ranges::random_access_range Y>
auto subset_convolve(X&& x, Y&& y)
    -> vector<decltype(declval<ranges::range_value_t<X>>() * declval<ranges::range_value_t<Y>>())>
{
    assert(ranges::size(x) == ranges::size(y));
    const auto n = ranges::size(x);
    assert(has_single_bit(n));
    const auto w = countr_zero(n);

    using VX = ranges::range_value_t<X>;
    using VY = ranges::range_value_t<Y>;
    auto dpx = vector(w + 1, vector(1ULL << w, VX{}));
    auto dpy = vector(w + 1, vector(1ULL << w, VY{}));
    REP(mask, n) {
        dpx[popcount<ull>(mask)][mask] += x[mask];
        dpy[popcount<ull>(mask)][mask] += y[mask];
    }
    
    REP(l, w + 1) {
        REP(i, w) {
            REP(mask_, 1ULL << (w - 1)) {
                const auto lo = mask_ & ((1LL << i) - 1);
                const auto hi = mask_ & ~((1LL << i) - 1);
                const auto mask = hi << 1 | lo;
                dpx[l][mask | 1LL << i] += dpx[l][mask];
                dpy[l][mask | 1LL << i] += dpy[l][mask];
            }
        }
    }

    using Result = decltype(declval<VX>() * declval<VY>());
    REP(mask, 1ULL << w) {
        auto v = vector(w + 1, Result{});
        REP(i, w + 1) REP(j, w + 1 - i) {
            v[i + j] += dpx[i][mask] * dpy[j][mask];
        }
        REP(i, w + 1) {
            dpx[i][mask] = v[i];
        }
    }
    
    REP(l, w + 1) {
        REP(i, w) {
            REP(mask_, 1LL << (w - 1)) {
                const auto lo = mask_ & ((1LL << i) - 1);
                const auto hi = mask_ & ~((1LL << i) - 1);
                const auto mask = hi << 1 | lo;
                dpx[l][mask | 1LL << i] -= dpx[l][mask];
            }
        }
    }
    
    auto ans = vector(n, Result{});
    REP(mask, n) {
        ans[mask] = dpx[popcount<ull>(mask)][mask];
    }
    return ans;
    
}