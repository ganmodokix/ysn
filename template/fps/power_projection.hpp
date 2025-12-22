#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"
#include "number/primitive_root.hpp"

// Power Projection O(N(logN)^2)
// verified at https://judge.yosupo.jp/submission/339294
// ⚠️ n_ は2冪である必要がある
// ret[i] := [x^{n-1}] g(x) f(x)^i (i = 0, 1, ..., m-1) を求める
// 上限 m_ defaults to n.
// ⚠️ 必要 m_ <= n_
// https://maspypy.com/fps-%E5%90%88%E6%88%90%E3%83%BB%E9%80%86%E9%96%A2%E6%95%B0%E3%81%AE%E8%A7%A3%E8%AA%AC-1-%E9%80%86%E9%96%A2%E6%95%B0%E3%81%A8-power-projection
template <mod_integral T>
constexpr vector<T> power_projection(
    vector<T> f_, vector<T> g_,
    const ll n_, const ll m_ = -1
) {
    assert(popcount((ull)n_) == 1);
    const auto m = m_ < 0 ? n_ : m_;
    assert(m <= n_);

    // [x^i y^j] arr = arr[i + j * n]
    // 便宜上 y = x^n として実装を進める
    // Q(x) の末尾は必ず 1 なので定数倍改善の余地あり

    // それぞれ畳み込み用に 2 倍しておく
    auto n = n_ * 2;  // (x の最高次 + 1) * 2
    auto l = 2LL; // (y の最高次 + 1) * 2

    
    // P(x, y) = g(x)
    auto p = move(g_);
    p.resize(n * l, T{0});
        
    // Q(x, y) = 1 - yf(x)
    // -(rev_y)-> Q'(x, y) = -f(x) + y
    auto q = move(f_);
    REP(i, ssize(q)) q[i] = -q[i];
    q.resize(n * l, T{0});
    q[n] = 1;

    // 周波数領域ずらし用前計算 (ω_{2n}^BitReverse(i))_i=0^{n-1}
    auto ntt_coeff = vector(n * l / 2, T{0});
    auto ntt_coeff_inv = vector(n * l / 2, T{0});
    {
        constexpr auto prim = T{primitive_root(T::pdiv)};
        const auto root_inv = prim.pow((T::pdiv - 1) / (n * l)).inv();
        auto coeff_inv = T{1};
        ntt_coeff_inv[0] = coeff_inv;
        auto idx = 0LL;
        REP(i, n * l / 2 - 1) {
            // bit-reverse
            auto bit = ~i & (i + 1);
            auto rev = (n * l / 4) / bit;
            idx ^= (n * l / 2 - 1) & ~(rev - 1);
            // non-one coeff
            ntt_coeff_inv[idx] = (coeff_inv *= root_inv);
        }
    }
    
    // Graeffe’s method: Q(x) Q(-x) で n 半々, l 倍々 にしていく
    while (n > 2) {
        assert(ssize(q) / 2 % 2 == 0);
        assert(q[q.size() / 2] == 1);
        // 周波数領域に写す
        p = ntt(move(p));
        q = ntt(move(q));
        // Q(-x): FFT後は前半と後半のswapとなるがbit-reverseしていないので隣接swapでOK
        auto qnx = vector(q.size(), T{0});
        REP(j, ssize(q) / 2) {
            qnx[j * 2] = q[j * 2 + 1];
            qnx[j * 2 + 1] = q[j * 2];
        }
        // 分子 P(x)Q(-x) の x^奇数次 をとる
        auto pqnx = move(p);
        auto b = vector<T>{};
        {
            constexpr auto half = 1_p / 2;
            REP(i, ssize(pqnx) / 2) {
                pqnx[i] = (
                      pqnx[i * 2    ] * qnx[i * 2    ]
                    - pqnx[i * 2 + 1] * qnx[i * 2 + 1]
                ) * half * ntt_coeff_inv[i];
            }
            pqnx.resize(ssize(pqnx) / 2, T{0});
            pqnx = ntt(move(pqnx), true);
            b = move(pqnx);
            b.resize(n * l, T{0});
            REP(j, l) {
                ranges::fill(
                    b.begin() + (n / 2) * j + n / 4,
                    b.begin() + (n / 2) * (j + 1),
                    T{0}
                );
            }
        }
        // 分母 Q(x)Q(-x) の x^偶数次 をとる
        auto qqnx = move(q);
        auto q2 = vector<T>{};
        {
            REP(i, ssize(qqnx) / 2) {
                constexpr auto half = 1_p / 2;
                qqnx[i] = (
                    qqnx[i * 2    ] * qnx[i * 2    ]
                    + qqnx[i * 2 + 1] * qnx[i * 2 + 1]
                ) * half;
            }
            qqnx.resize(ssize(qqnx) / 2, T{0});
            qqnx = ntt(move(qqnx), true);
            qqnx[0] -= 1;  // 巡回してきてはみ出た分を調整
            q2 = move(qqnx);
            q2.resize(n * l, T{0});
            REP(j, l) {
                ranges::fill(
                    q2.begin() + (n / 2) * j + n / 4,
                    q2.begin() + (n / 2) * (j + 1),
                    T{0}
                );
            }
            q2[q2.size() / 2] += 1;  // 巡回してきてはみ出た分を調整
        }
        // n 半々, l 倍々
        p = move(b);
        q = move(q2);
        n /= 2;
        l *= 2;
    }
    
    assert(l == n_ * 2);
    RPE(i, m) p[i] = p[(i + n_ - m) * n];
    RPE(i, m) q[i] = q[(i + n_ - m + 1) * n];
    p.resize(m, T{0});
    q.resize(m, T{0});
    ranges::reverse(p);
    ranges::reverse(q);

    q = fps_inv(q);
    p = convolve_p(move(p), move(q));
    p.resize(m);
    return p;
    
}