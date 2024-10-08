#pragma once
#include "base_template.hpp"
#include "graph.hpp"
#include "radix_heap.hpp"
pair<vector<ll>, vector<ll>> dist_dijkstra(ll root, const graph &g) {
    vector<ll> dist(g.size(), LINF);
    vector<ll> from(g.size(), -1LL);
    auto q = radix_heap<ll, ll>{};
    q.emplace(dist[root] = 0, root);
    while (q.size()) {
        const auto [du, u] = q.pop();
        if (du > dist[u]) continue;
        for (auto [v, d] : g.adjacent_list[u]) {
            if (chmin(dist[v], dist[u] + d)) {
                q.emplace(dist[v], v);
                from[v] = u;
            }
        }
    }
    return {dist, from};
}
