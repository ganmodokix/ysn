#pragma once
#include "base_template.hpp"
#include "graph.hpp"

// 虚樹 LCA-based auxiliary tree
// vertices: 含む頂点の列
// arrival(u): オイラーツアーで u に入る時刻
// departure(u): オイラーツアーで u を出る時刻
// lca(u, v): u と v の LCA
// returns {頂点番号座圧したグラフ, 座圧写像, 座圧逆写像}
template <typename R, typename Arrival, typename Departure, typename LCA>
auto lca_auxiliary_tree(
    const graph& tree, R&& vertices,
    Arrival arrival, Departure departure, LCA lca
) -> tuple<graph, vector<ll>, unordered_map<ll, ll>>
{
    auto vs = vector<ll>{};
    if constexpr (ranges::sized_range<R>) {
        vs.reserve(ranges::size(vertices));
    }
    for (const auto v : vertices) {
        vs.emplace_back(v);
    }

    // 行きがけ順でソートしてオイラーツアーで通るLCAだけ追加する
    ranges::sort(vs, less{}, arrival);
    REP(i, vs.size() - 1) {
        const auto u = vs[i];
        const auto v = vs[i + 1];
        vs.emplace_back(lca(u, v));
    }
    ranges::sort(vs, less{}, arrival);
    vs.erase(unique(vs.begin(), vs.end()), vs.end());
    const auto m = vs.size();  // auxiliary tree の頂点数
    auto vinv = unordered_map<ll, ll>{};
    REP(i, m) {
        vinv[vs[i]] = i;
    }

    auto q = vector<ll>{};
    auto gaux = graph(m);
    for (const auto u : vs) {
        // q が虚樹根から u までの traceback になるようにする
        while (q.size() && !(arrival(q.back()) < arrival(u)
            && departure(u) < arrival(q.back()))) q.pop_back();
        // 親との辺を張る
        if (q.size()) {
            const auto parent_u = q.back();
            gaux.add_edge(parent_u, u);
        }
        q.push_back(u);
    }

    return {gaux, move(vs), move(vinv)};
}