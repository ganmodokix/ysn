#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// 多項式合成 f(g(x)) Kinoshita-Li algorithm O(N(logN)^2)
// verified at https://judge.yosupo.jp/submission/339265
// https://noshi91.hatenablog.com/entry/2024/03/16/224034
// https://maspypy.com/fps-%e5%90%88%e6%88%90%e3%83%bb%e9%80%86%e9%96%a2%e6%95%b0%e3%81%ae%e8%a7%a3%e8%aa%ac%ef%bc%882%ef%bc%89%e8%bb%a2%e7%bd%ae%e5%8e%9f%e7%90%86%e3%81%ab%e3%82%88%e3%82%8b%e5%90%88%e6%88%90%e3%82%a2
// https://arxiv.org/abs/2404.05177
template <mod_integral T>
constexpr vector<T> fps_compose(
    vector<T> f_, vector<T> g_
) {
    assert(f_.size() == g_.size());
    assert(f_.size() != 0);
    const auto n_original = (ll)f_.size();
    const auto n_ = (ll)bit_ceil(f_.size());
    f_.resize(n_, T{0});
    g_.resize(n_, T{0});
    const auto m_ = n_;
    assert(ssize(g_) == n_);
    assert(g_[0] == 0);
    const auto m = m_ < 0 ? n_ : m_;
    assert(m <= n_);

    // Power Projection の勾配を求めると答えが求まる
    // まずpower projection の重み付き和をとる 
    // 勾配を求めるための不定元 w(x) を入力の片方としておく
    // \sum_i f_i [x^{n-1}] w(x) g(x)^i
    // = \sum_i f_i \sum_j w_{n-1-j} [x^j] g(x)^i
    // そこでw(x)の係数について勾配をとる
    //   ∇_w \sum_i f_i \sum_j w_{n-1-j} [x^j] g(x)^i
    // = ∇_w \sum_i f_i \sum_j w_{j} [x^j] g(x)^i
    // = ∇_w \sum_j w_j \sum_i [x^{n-1-j}] f_i g(x)^i
    // = \sum_i [x^{n-1-j}] f_i g(x)^i　(j: 自由変数)
    // = [x^{n-1-j}] \sum_i f_i g(x)^i
    // = [x^j] rev(f(g(x)))  Wow!

    auto initialize_forward = [&n_](
        vector<T> f_,
        const ll n, const ll l
    ) {
        // Q(x, y) = 1 - yf(x)
        auto q = move(f_);
        REP(i, ssize(q)) q[i] = -q[i];
        q.resize(n * l, T{0});
        q[n] = 1;
        return q;
    };

    // ∇_g X
    auto initialize_backward = [&n_](const ll g_ssize, vector<T> grad_p) {
        auto grad_g_ = move(grad_p);
        grad_g_.resize(g_ssize, T{0});
        return grad_g_;
    };

    // convolve_p の backward; 便宜上第一引数の勾配を求める
    auto convolve_backward_p = [](
        vector<T> q,
        vector<T> grad_pq
    ) {
        // middle product
        const auto p_ssize = ssize(grad_pq) + 1 - ssize(q);
        assert(p_ssize > 0);
        // FFTの勾配を考えて高速化、長さ |p| + |q| - 1 に抑える
        assert(ssize(q) >= 0);
        const auto h = (ll)bit_ceil(grad_pq.size());

        auto grad_p = move(grad_pq);
        grad_p.resize(h, T{0});

        // transposed INTT
        ranges::reverse(grad_p.begin() + 1, grad_p.end());
        grad_p = ntt(move(grad_p));
        // 周波数積
        q.resize(h, T{0});
        q = ntt(move(q));
        RPE(i, ssize(grad_p)) grad_p[i] *= q[i];
        // transposed NTT
        grad_p = ntt(move(grad_p), true);
        ranges::reverse(grad_p.begin() + 1, grad_p.end());

        grad_p.resize(p_ssize, T{0});
        return grad_p;
    };
    
    // Graeffe’s method: Q(x) Q(-x) で n 半々, l 倍々 にしていく
    auto graeffe_forward =
        []
        (
            vector<T> q,
            const ll n,
            const ll l
        )
    {
        assert(ssize(q) / 2 % 2 == 0);
        assert(q[q.size() / 2] == 1);

        // 周波数領域に写す
        q = ntt(move(q));

        // Q(-x): FFT後は前半と後半のswapとなるがbit-reverseしていないので隣接swapでOK
        auto qnx = vector(q.size(), T{0});
        REP(j, ssize(q) / 2) {
            qnx[j * 2] = q[j * 2 + 1];
            qnx[j * 2 + 1] = q[j * 2];
        }
        
        // 分母 Q(x)Q(-x) の x^偶数次 をとる
        // 偶数次だけ取れればよいので高速化
        auto qqnx = move(q);
        constexpr auto half = 1_p / 2;
        RPE(i, ssize(qqnx) / 2) {
            qqnx[i] = (
                  qqnx[i * 2    ] * qnx[i * 2    ]
                + qqnx[i * 2 + 1] * qnx[i * 2 + 1]
            ) * half;
        }
        qqnx.resize(ssize(qqnx) / 2, T{0});
        qqnx = ntt(move(qqnx), true);
        qqnx[0] -= 1;  // 巡回してきてはみ出た分を調整
        auto q2 = vector(n * l, T{0});
        REP(i, n / 4) REP(j, l) {
            q2[i + (n / 2) * j] = qqnx[i + n / 2 * j];
        }
        q2[(n / 2) * l] += 1;  // 巡回してきてはみ出た分を調整
        
        return tuple{move(q2), move(qnx), n / 2, l * 2};
    };

    auto graeffe_backward = [&convolve_backward_p](
        vector<T> qnx,
        vector<T> grad_p2,
        const ll n, const ll l
    ) -> vector<T> {
        // b[i + (n / 2) * j] = pqnx[i * 2 + 1 + n * j];
        auto grad_pqnx = vector(n * l, T{0});
        // DUMP(grad_p2.size(), n * l);
        REP(i, n / 4) REP(j, l) {
            grad_pqnx[i * 2 + 1 + n * j] += grad_p2[i + (n / 2) * j];
        }
        
        // grad of NTTs
        auto grad_p = move(grad_pqnx);
        assert(grad_p.size() == qnx.size());
        
        // pqnx = ntt(move(pqnx), true);
        ranges::reverse(grad_p.begin() + 1, grad_p.end());
        grad_p = ntt(move(grad_p));
        // 周波数積
        RPE(i, ssize(grad_p)) grad_p[i] *= qnx[i];
        // auto pqnx = ntt(move(p));
        grad_p = ntt(move(grad_p), true);
        ranges::reverse(grad_p.begin() + 1, grad_p.end());
        // DUMP(grad_p);
        return grad_p;
        
    };

    auto finalize_forward = [&n_](
        const vector<T> q_,
        const ll n
    ) {
        const auto m = n_;
        auto q = vector(m, T{0});
        RPE(i, m) q[i] = q_[(i + n_ - m + 1) * n];
        ranges::reverse(q);
        auto invq = fps_inv(q);
        return move(invq);
    };
    auto finalize_backward = [&convolve_backward_p, &n_](
        vector<T> invq,
        vector<T> grad_pq,
        const ll n
    ) -> vector<T> {
        const auto m = n_;
        auto grad_p = convolve_backward_p(move(invq), move(grad_pq));
        auto grad_p_ = vector(n_ * n * 2, T{0});
        RPE(i, n_) grad_p_[(i + n_ - m) * n] = grad_p.rbegin()[i];
        return grad_p_;
    };

    // [x^i y^j] arr = arr[i + j * n]
    // 便宜上 y = x^n として実装を進める
    // Q(x) の末尾は必ず 1 なので定数倍改善の余地あり

    auto nls = vector<pair<ll, ll>>{{n_ * 2, 2LL}};
    auto qprev = vector<T>{};
    auto qnxs = vector<vector<T>>{};

    qprev = initialize_forward(g_, nls.back().first, nls.back().second);

    while (nls.back().first > 2) {
        auto&& [q2, qnx, n2, l2] = 
            graeffe_forward(
                move(qprev),
                nls.back().first, nls.back().second
            );
        qprev = move(q2);
        qnxs.emplace_back(move(qnx));
        nls.emplace_back(n2, l2);
    }

    auto invq = finalize_forward(
        move(qprev), nls.back().first
    );

    // 勾配算出

    auto grad_pq = move(f_);
    grad_pq.resize(ssize(invq) * 2 - 1, T{0});

    auto grad_p_ = finalize_backward(
        move(invq), move(grad_pq), nls.back().first
    );

    DSRNG(i, ssize(nls) - 2, 0) {
        auto&& [n, l] = nls[i];
        grad_p_ = graeffe_backward(
            move(qnxs[i]), move(grad_p_), n, l
        );
    }

    auto grad_w_ = initialize_backward(n_, move(grad_p_));

    ranges::reverse(grad_w_);

    grad_w_.resize(n_original, T{0});
    return grad_w_;
    
}
