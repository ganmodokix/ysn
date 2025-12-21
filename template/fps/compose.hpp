#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// 多項式合成 f(g(x)) Kinoshita-Li algorithm O(N(logN)^2)
// verified at 
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
        vector<T> f_, vector<T> g_,
        const ll n, const ll l
    ) {

        auto p = vector(n * l, T{0});
        auto q = vector(n * l, T{0});

        // P(x, y) = g(x)
        p = move(g_);
        p.resize(n * l, T{0});

        // Q(x, y) = 1 - yf(x)
        q[0] = 1;
        REP(i, min<ll>(ssize(f_), n_)) {
            q[i + n] = -f_[i];
        }

        return pair{move(p), move(q)};

    };

    // ∇_g X
    auto initialize_backward = [&n_](const vector<T>& g_, vector<T> grad_p) {
        auto grad_g_ = move(grad_p);
        grad_g_.resize(ssize(g_), T{0});
        return grad_g_;
    };

    // convolve_p の backward; 便宜上第一引数の勾配を求める
    auto convolve_backward_p = [](
        vector<T> q,
        const vector<T>& pq,
        vector<T> grad_pq
    ) {
        // middle product
        const auto length = ssize(pq) + 1 - ssize(q);
        constexpr auto USES_NAIVE = false;
        if (!USES_NAIVE) {
            // FFTの勾配を考えて高速化、長さ |p| + |q| - 1 に抑える
            assert(ssize(q) >= 0);
            const auto h = (ll)bit_ceil(pq.size());
            const auto hinv = T{h}.inv();

            auto grad_p = move(grad_pq);
            grad_p.resize(h, T{0});

            // transposed INTT
            ranges::reverse(ranges::subrange(grad_p.begin() + 1, grad_p.end()));
            grad_p = ntt(move(grad_p));
            REP(i, ssize(grad_p)) grad_p[i] *= hinv;
            // 周波数積
            q.resize(h, T{0});
            q = ntt(move(q));
            RPE(i, ssize(grad_p)) grad_p[i] *= q[i];
            // transposed NTT
            RPE(i, ssize(grad_p)) grad_p[i] *= h;
            grad_p = ntt(move(grad_p), true);
            ranges::reverse(ranges::subrange(grad_p.begin() + 1, grad_p.end()));

            grad_p.resize(length, T{0});
            return grad_p;
        } else {
            // 長さ |p| + 2|q| - 2 で FFT
            assert(ssize(grad_pq) == ssize(pq));
            const auto offset = ssize(q) - 1;
            auto rq = move(q);
            ranges::reverse(rq);
            auto grad_p = convolve_p(move(grad_pq), move(rq));
            grad_p = vector(
                grad_p.begin() + offset,
                grad_p.begin() + offset + length
            );
            return grad_p;
        }
    };
    
    // Graeffe’s method: Q(x) Q(-x) で n 半々, l 倍々 にしていく
    auto graeffe_forward =
        []
        (
            vector<T> p,
            vector<T> q,
            const ll n,
            const ll l
        )
    {
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
        auto q2 = vector(n * l, T{0});
        REP(i, n / 4) REP(j, l) {
            q2[i + (n / 2) * j] = qqnx[i * 2 + n * j];
        }
        // n 半々, l 倍々
        const auto p2 = move(b);
        return tuple{move(p2), move(q2), move(qnx), n / 2, l * 2};
    };

    auto graeffe_backward = [&convolve_backward_p](
        vector<T> p2, vector<T> qnx,
        vector<T> grad_p2,
        const ll n, const ll l
    ) -> vector<T> {
        // b[i + (n / 2) * j] = pqnx[i * 2 + 1 + n * j];
        auto grad_pqnx = vector(n * l, T{0});
        // DUMP(grad_p2.size(), n * l);
        REP(i, n / 4) REP(j, l) {
            grad_pqnx[i * 2 + 1 + n * j] += grad_p2[i + (n / 2) * j];
        }
        
        constexpr auto USES_NAIVE = false;
        if constexpr (!USES_NAIVE) {
            // grad of NTTs
            auto grad_p = move(grad_pqnx);
            assert(grad_p.size() == qnx.size());
            const auto h = T{ssize(grad_p)};
            const auto hinv = h.inv();
            
            // pqnx = ntt(move(pqnx), true);
            ranges::reverse(ranges::subrange(grad_p.begin() + 1, grad_p.end()));
            grad_p = ntt(move(grad_p));
            REP(i, ssize(grad_p)) grad_p[i] *= hinv;
            // 周波数積
            RPE(i, ssize(grad_p)) grad_p[i] *= qnx[i];
            // auto pqnx = ntt(move(p));
            RPE(i, ssize(grad_p)) grad_p[i] *= h;
            grad_p = ntt(move(grad_p), true);
            ranges::reverse(ranges::subrange(grad_p.begin() + 1, grad_p.end()));
            // DUMP(grad_p);
            return grad_p;
        } else {
            // naive convs
            // DUMP(grad_pqnx);
            qnx = ntt(move(qnx), true);
            auto pqnx = vector(qnx.size(), T{0});
            REP(i, n / 4) REP(j, l) {
                pqnx[i * 2 + 1 + n * j] += p2[i + (n / 2) * j];
            }

            auto grad_p = convolve_backward_p(qnx, pqnx, grad_pqnx);
            // DUMP(grad_p);
            
            return grad_p;
        }
    };

    auto finalize_forward = [&n_](
        const vector<T>& p_, const vector<T> q_,
        const ll n
    ) {
        const auto m = n_;
        auto p = vector(m, T{0});
        auto q = vector(m, T{0});
        RPE(i, m) p[i] = p_[i * n];
        RPE(i, m) q[i] = q_[i * n];
        auto invq = fps_inv(q);
        auto pinvq = convolve_p(p, invq);
        // p.resize(m, T{0});
        return tuple{move(pinvq), move(invq)};
    };
    auto finalize_backward = [&convolve_backward_p, &n_](
        vector<T> invq,
        vector<T> pinvq,
        vector<T> grad_pq,
        const ll n
    ) -> vector<T> {
        auto grad_p = convolve_backward_p(move(invq), move(pinvq), move(grad_pq));
        auto grad_p_ = vector(n_ * n * 2, T{0});
        // DUMP(ssize(grad_p_), n);
        RPE(i, n_) grad_p_[i * n] = grad_p[i];
        return grad_p_;
    };

    // [x^i y^j] arr = arr[i + j * n]
    // 便宜上 y = x^n として実装を進める
    // Q(x) の末尾は必ず 1 なので定数倍改善の余地あり

    auto nls = vector<pair<ll, ll>>{{n_ * 2, 2LL * 2}};
    auto pqs = vector<pair<vector<T>, vector<T>>>{};
    auto qnxs = vector<vector<T>>{};

    auto w = vector(n_, T{1});
    {
        auto&& [p, q] = initialize_forward(move(g_), w, nls.back().first, nls.back().second);
        pqs.emplace_back(move(p), move(q));
    }

    while (nls.back().first > 2) {
        auto&& [p2, q2, qnx, n2, l2] = 
            graeffe_forward(
                pqs.back().first, move(pqs.back().second),
                nls.back().first, nls.back().second
            );
        pqs.emplace_back(move(p2), move(q2));
        qnxs.emplace_back(move(qnx));
        nls.emplace_back(n2, l2);
    }

    auto&& [pinvq, invq] = finalize_forward(
        pqs.back().first, move(pqs.back().second), nls.back().first
    );

    // 勾配算出

    auto grad_pq = move(f_);
    grad_pq.resize(pinvq.size(), T{0});

    auto grad_p_ = finalize_backward(
        move(invq), move(pinvq), move(grad_pq), nls.back().first
    );

    DSRNG(i, ssize(pqs) - 2, 0) {
        auto&& [n, l] = nls[i];
        grad_p_ = graeffe_backward(
            move(pqs[i + 1].first), move(qnxs[i]), move(grad_p_), n, l
        );
    }

    auto grad_w_ = initialize_backward(w, move(grad_p_));

    ranges::reverse(grad_w_);

    grad_w_.resize(n_original, T{0});
    return grad_w_;
    
}
