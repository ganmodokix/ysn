#pragma once
#include "base_template.hpp"

// SMAWK O(N + M)
// (N, M)-shaped matrix A の各行の最小値添字 argmin_j A_{i, j} を求める
// A は陽に格納されておらず A_ij = func(i, j) として表される
// ⚠ A は Monge （行内の隣接要素との差が行を下るにつれ広義単調減少）である必要がある
// ⚠ func(i, j) が定数時間でない場合計算量に掛かる
template <typename Func, typename Comp = less<>, typename Proj = identity>
vector<size_t> smawk(size_t n, size_t m, Func func, Comp comp = {}, Proj proj = {}) {

    using T = remove_cvref_t<
        decltype(
            invoke(proj,
                invoke(func,
                    declval<size_t>(),
                    declval<size_t>())))>;

    auto ans = vector(n, m);

    auto subsolve = [&func, &comp, &proj, &ans](
        auto subsolve, vector<size_t> js, size_t ib, size_t ie, size_t id
    ) -> void {
        if (ib >= ie) return;

        {
            auto js2 = vector<size_t>{};
            auto i = ib;
            for (const auto j : js) {
                while (
                    !js2.empty()
                    && !invoke(comp,
                        invoke(proj, invoke(func, i, js2.back())),
                        invoke(proj, invoke(func, i, j))
                    )
                ) {
                    js2.pop_back();
                    i -= id;
                }
                if (js2.size() != (ie - ib - 1) / id) {
                    js2.push_back(j);
                    i += id;
                }
            }

            subsolve(subsolve, move(js2), ib + id, ie, id * 2);
        }

        for (auto i = ib, q = size_t{0}; i < ie; i += id * 2) {
            const auto jt = i + id < ie ? ans[i + id] : js.back();
            auto fm = optional<T>{};
            for (; q < js.size(); q++) {
                auto fq = invoke(proj, invoke(func, i, js[q]));
                if (!fm.has_value() || invoke(comp, fq, *fm)) {
                    fm = fq;
                    ans[i] = js[q];
                }
                if (js[q] == jt) break;
            }
        }

    };

    auto js = vector<size_t>(m);
    iota(ALL(js), 0);
    subsolve(subsolve, js, 0, n, 1);

    return ans;
    
}
