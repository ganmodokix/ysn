#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/derivative.hpp"
#include "fps/multipoint.hpp"
#include "fps/quotient.hpp"

// \prod_i (x - r_i) を分割統治で求める
template <mod_integral T, ranges::input_range R>
requires convertible_to<ranges::range_value_t<R>, T>
vector<T> linear_prod(R&& r) {
    auto v = vector<vector<T>>{};
    for (auto&& a : r) {
        v.emplace_back(vector<T>{forward<decltype(a)>(a), 1});
    }
    while (v.size() > 1) {
        REP(i, v.size() / 2) {
            v[i] = convolve_p(move(v[i * 2]), move(v[i * 2 + 1]));
        }
        if (v.size() % 2 == 1) {
            v[v.size() / 2] = move(v.back());
        }
        v.resize((v.size() + 1) / 2);
    }
    return move(v[0]);
}

// 多項式 Lagrange 補間 O(N(logN)^2)
// ref: https://37zigen.com/lagrange-interpolation/
// verify: https://judge.yosupo.jp/submission/279911
template <mod_integral T, ranges::forward_range X, ranges::forward_range Y>
requires convertible_to<ranges::range_value_t<X>, T>
    && convertible_to<ranges::range_value_t<Y>, T>
vector<T> lagrange_interpolate(X&& x_, Y&& fx_) {
    using U = decltype(declval<T>().item());
    auto evidence = unordered_map<U, T>{};
    {
        auto it = ranges::begin(x_);
        auto jt = ranges::begin(fx_);
        while (it != ranges::end(x_) && jt != ranges::end(fx_)) {
            const auto xitem = T{forward<decltype(*it)>(*it)}.item();
            if (
                auto kt = evidence.find(xitem);
                kt != evidence.end()
            ) {
                assert(*jt == kt->second);
            } else {
                evidence[xitem] = forward<decltype(*jt)>(*jt);
            }
            ++it, ++jt;
        }
    }

    assert(!evidence.empty());

    // ユニークな評価点を配列にする
    auto x = vector<T>{};
    auto fx = vector<T>{};
    x.reserve(evidence.size());
    fx.reserve(evidence.size());
    for (const auto& [xi, fxi] : evidence) {
        x.emplace_back(xi);
        fx.emplace_back(fxi);
    }
    
    // 各 \frac{l(x)}{x-x_i} の 係数を求める
    auto l = linear_prod<T>(x | views::transform(negate{}));
    auto ldash = fps_derivative(l);
    auto a = multipoint_evaluate(move(ldash), x);
    REP(i, x.size()) {
        a[i] = fx[i] / a[i];
    }

    // \sum_i \frac{l(x)}{x - x_i} の分母分子を求める
    auto v = vector<pair<vector<T>, vector<T>>>{};
    RPE(i, x.size()) {
        v.emplace_back(vector{a[i]}, vector{-x[i], T{1}});
    }
    while (v.size() > 1) {
        REP(i, v.size() / 2) {
            auto&& [pt, pb] = v[i * 2];
            auto&& [qt, qb] = v[i * 2 + 1];

            // pt/pb + qt/qb = (pt * qb + qt * pb) / (pb * qb)
            auto ptqb = convolve_p(move(pt), qb);
            auto qtpb = convolve_p(move(qt), pb);
            auto pqb = convolve_p(move(pb), move(qb));
            
            auto pqt = move(ptqb);
            REP(i, qtpb.size()) pqt[i] += qtpb[i];
            v[i] = {move(pqt), move(pqb)};
        }
        if (v.size() % 2 == 1) {
            v[v.size() / 2] = move(v.back());
        }
        v.resize((v.size() + 1) / 2);
    }
    
    // l(x) \sum_i \frac{l(x)}{x - x_i} = f(x)
    auto&& [vt, vb] = v[0];
    auto lvt = convolve_p(move(l), move(vt));
    return polynomial_quotient(move(lvt), move(vb));
    
}