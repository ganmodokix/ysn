#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"

// Bostan-Mori [x^m] f(x) / g(x) を求める O(NlogNlogM) (N: deg g)
// verified at https://judge.yosupo.jp/submission/339373
// verified at https://atcoder.jp/contests/abc436/submissions/71918989
template <mod_integral T>
constexpr T bostan_mori(vector<T> f, vector<T> g, ll m) {
    assert(m >= 0);
    assert(!g.empty());
    assert(g[0] != 0);
    assert(ssize(f) < ssize(g));
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

    // 先にNTT用の長さにしておく
    const auto fg_size = (ll)bit_ceil<ull>(max(ssize(f), ssize(g)) * 2 - 1);
    f.resize(fg_size, T{0});
    g.resize(fg_size, T{0});
    
    // 周波数領域ずらし用前計算 (ω_{2n}^BitReverse(i))_i=0^{n-1}
    auto ntt_coeff_rev = vector(fg_size / 2, T{0});
    auto ntt_coeff_inv = vector(fg_size / 2, T{0});
    {
        constexpr auto prim = T{primitive_root(T::pdiv)};
        const auto root = prim.pow((T::pdiv - 1) / fg_size);
        // const auto root_inv = prim.pow((T::pdiv - 1) / fg_size).inv();
        const auto root_inv = root.inv();
        auto coeff = T{1};
        auto coeff_inv = T{1};
        ntt_coeff_rev[0] = coeff;
        ntt_coeff_inv[0] = coeff_inv;
        auto idx = 0LL;
        REP(i, fg_size / 2 - 1) {
            // bit-reverse
            auto bit = ~i & (i + 1);
            auto rev = (fg_size / 4) / bit;
            idx ^= (fg_size / 2 - 1) & ~(rev - 1);
            // non-one coeff
            ntt_coeff_rev[i + 1] = (coeff *= root);  // ⚠️ NOT bit reversed!
            ntt_coeff_inv[idx] = (coeff_inv *= root_inv);
        }
    }

    // 周波数領域に写しておく
    f = ntt(move(f));
    g = ntt(move(g));

    auto recurse = [fg_size, &ntt_coeff_rev, &ntt_coeff_inv](
        const auto recurse, vector<T> f, vector<T> g, ll m
    ) -> T {

        if (m == 0) {
            // if (f.empty()) return 0;
            // return f[0] / g[0];
            return accumulate(ALL(f), T{0}) / accumulate(ALL(g), T{0});
        }

        // g(-x)
        auto gnx = vector(fg_size, T{0});
        REP(j, fg_size / 2) {
            gnx[j * 2] = g[j * 2 + 1];
            gnx[j * 2 + 1] = g[j * 2];
        }
        
        // 分母 g(x)g(-x) の偶数部分
        constexpr auto half = T{1} / 2;
        REP(i, fg_size / 2) {
            g[i] = (
                  g[i * 2    ] * gnx[i * 2    ]
                + g[i * 2 + 1] * gnx[i * 2 + 1]
            ) * half;
        }
        g.resize(fg_size / 2, T{0});
        
        // 2倍ゼロ埋めリサイズ　-> 「2 倍の長さの DFT の計算」
        auto gt = ntt(g, true);
        g.resize(fg_size, T{0});
        REP(i, fg_size / 2) {
            gt[i] *= ntt_coeff_rev[i];
        }
        gt = ntt(move(gt));
        DSRNG(i, fg_size / 2 - 1, 0) {
            g[i] = g[i];
            g[i + fg_size / 2] = gt[i];
        }

        // 分子 f(x)g(-x) の偶数 or 奇数部分
        if (m % 2) {
            REP(i, fg_size / 2) {
                f[i] = (
                      f[i * 2    ] * gnx[i * 2    ]
                    - f[i * 2 + 1] * gnx[i * 2 + 1]
                ) * half * ntt_coeff_inv[i];
            }
        } else {
            REP(i, fg_size / 2) {
                f[i] = (
                      f[i * 2    ] * gnx[i * 2    ]
                    + f[i * 2 + 1] * gnx[i * 2 + 1]
                ) * half;
            }
        }
        f.resize(fg_size / 2, T{0});

        // 2倍ゼロ埋めリサイズ　-> 「2 倍の長さの DFT の計算」
        auto ft = ntt(f, true);
        f.resize(fg_size, T{0});
        REP(i, fg_size / 2) {
            ft[i] *= ntt_coeff_rev[i];
        }
        ft = ntt(move(ft));
        DSRNG(i, fg_size / 2 - 1, 0) {
            f[i] = f[i];
            f[i + fg_size / 2] = ft[i];
        }

        return recurse(recurse, move(f), move(g), m / 2);

    };

    return recurse(recurse, move(f), move(g), m);

}
