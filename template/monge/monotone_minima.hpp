// #REQ: base_template.hpp

// Monotone-minima O(M + NlogM)
// (N, M)-shaped matrix A の各行の最小値添字 argmin_j A_{i, j} を求める
// A は陽に格納されておらず A_ij = func(i, j) として表される
// ⚠ A は Monge （行内の隣接要素との差が行を下るにつれ広義単調減少）である必要がある
// ⚠ func(i, j) が定数時間でない場合計算量に掛かる
template <typename Func, typename Comp = less<>, typename Proj = identity>
vector<size_t> monotone_minima(size_t n, size_t m, Func func, Comp comp = {}, Proj proj = {}) {
    
    using T = remove_cvref_t<
        decltype(
            invoke(proj,
                invoke(func,
                    declval<size_t>(),
                    declval<size_t>())))>;

    auto ans = vector<size_t>(n, m);

    auto subsolve = [&func, &comp, &proj, &ans](
            auto subsolve,
            size_t ib, size_t jb,
            size_t ie, size_t je
    ) -> void {
        if (ib == ie) return;
        const auto im = (ib + ie) / 2;
        {
            auto minimum = optional<T>{};
            for (const auto j : views::iota(jb, je)) {
                const auto value = invoke(proj, invoke(func, im, j));
                // ↓ ごついwarningが出る場合がある
                if (!minimum.has_value() || invoke(comp, value, *minimum)) {
                    minimum = value;
                    ans[im] = j;
                }
            }
        }
        subsolve(subsolve, ib, jb, im, ans[im] + 1);
        subsolve(subsolve, im + 1, ans[im], ie, je);
    };

    subsolve(subsolve, 0, 0, n, m);

    return ans;

}