#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"

// Bostan-Mori [x^m] f(x) / g(x) を求める O(NlogNlogM) (N: deg g)
// verified at https://judge.yosupo.jp/submission/336936
// verified at https://atcoder.jp/contests/abc436/submissions/71731332
template <mod_integral T>
constexpr T bostan_mori(vector<T> f, vector<T> g, ll m) {
    assert(m >= 0);
    assert(!g.empty());
    assert(g[0] != 0);
    assert(ssize(f) < ssize(g));
    if (m == 0) {
        if (f.empty()) return 0;
        return f[0] / g[0];
    }
    // P(x)Q(-x) = A(x^2) + x B(x^2),
    // Q(x)Q(-x) = R(x^2) ∵ Q(x)Q(-x) は偶
    // 以上のように分解すると
    // P(x)/Q(x) = P(x)Q(-x) / Q(x)Q(-x) = A(x^2)/R(x^2) + x B(x^2)/R(x^2)
    // FPS が x^2 のみからなる <=> 偶 <=> 奇数次数の係数がすべて 0
    // なので
    // [x^m] P(x)/Q(x)
    // = [x^{m/2}] A(x)/R(x) if m は偶数
    // = [x^{(m-1)/2}] B(x)/R(x)

    // ステップごとにmが半減するのでダブリングの要領で O(NlogNlogM)
    // P,Qの次数は畳み込んで半減なので±1を除いて変わらず
    
    // 分母
    auto gnx = g;  // g(-x)
    STEP(i, 1, ssize(gnx) - 1, 2) {
        gnx[i] *= -1;
    }
    auto rx2 = convolve_p(move(g), gnx);
    auto r = vector<T>{};
    r.reserve((rx2.size() + 1) / 2);
    STEP(i, 0, ssize(rx2) - 1, 2) {
        r.emplace_back(rx2[i]);
    }

    auto ab = convolve_p(move(f), move(gnx));
    auto ab2 = vector<T>{};
    ab2.reserve((ssize(ab) + (m + 1) % 2) / 2);
    STEP(i, m % 2, ssize(ab) - 1, 2) {
        ab2.emplace_back(ab[i]);
    }

    return bostan_mori(move(ab2), move(r), m / 2);
}
