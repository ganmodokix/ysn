#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// Power Projection O(N(logN)^2)
// ⚠️ n_ は2冪である必要がある
// ret[i] := [x^{n-1}] g(x) f(x)^i (i = 0, 1, ..., m-1) を求める
// 上限 m_ defaults to n.
// ⚠️ 必要 m_ <= n_
// https://maspypy.com/fps-%E5%90%88%E6%88%90%E3%83%BB%E9%80%86%E9%96%A2%E6%95%B0%E3%81%AE%E8%A7%A3%E8%AA%AC-1-%E9%80%86%E9%96%A2%E6%95%B0%E3%81%A8-power-projection
template <mod_integral T>
constexpr vector<T> power_projection(
    const vector<T>& f_, const vector<T>& g_,
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

    auto p = vector(n * l, T{0});
    auto q = vector(n * l, T{0});

    // P(x, y) = g(x)
    REP(i, min<ll>(ssize(g_), n_)) {
        p[i] = g_[i];
    }

    // Q(x, y) = 1 - yf(x)
    // -(rev_y)-> Q'(x, y) = -f(x) + y
    q[n] = 1;
    REP(i, min<ll>(ssize(f_), n_)) {
        q[i] = -f_[i];
    }
    
    // Graeffe’s method: Q(x) Q(-x) で n 半々, l 倍々 にしていく
    while (n > 2) {
        assert(ssize(q) / 2 % 2 == 0);
        assert(q[q.size() / 2] == 1);
        // Q(-x)
        auto qnx = q; STEP(i, 1, ssize(qnx), 2) qnx[i] = -qnx[i];
        qnx = ntt(move(qnx));
        // 分子 P(x)Q(-x) の x^奇数次 をとる
        auto pqnx = ntt(move(p));
        RPE(i, ssize(pqnx)) pqnx[i] *= qnx[i];
        pqnx = ntt(move(pqnx), true);
        auto b = vector(n * l, T{0});
        REP(i, n / 4) REP(j, l) {
            b[i + (n / 2) * j] = pqnx[i * 2 + 1 + n * j];
        }
        // 分母 Q(x)Q(-x) の x^偶数次 をとる
        auto qqnx = ntt(move(q));
        RPE(i, ssize(qqnx)) qqnx[i] *= qnx[i];
        qqnx = ntt(move(qqnx), true);
        qqnx[0] -= 1;  // 巡回してきてはみ出た分を調整
        auto q2 = vector(n * l, T{0});
        REP(i, n / 4) REP(j, l) {
            q2[i + (n / 2) * j] = qqnx[i * 2 + n * j];
        }
        q2[(n / 2) * l] += 1;
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