// #REQ: base_template.hpp graph.hpp
pair<vector<ll>, vector<ll>> dist_01bfs(ll root, const graph &g) {
    vector<ll> dist(g.size(), LINF);
    vector<ll> from(g.size(), -1LL);
    deque<ll> q = {root};
    dist[root] = 0;
    while (q.size()) {
        auto u = q.front(); q.pop_front();
        for (auto [v, d] : g.adjacent_list[u]) {
            ll d01 = !!d;
            if (chmin(dist[v], dist[u] + d01)) {
                if (d01) {
                    q.emplace_back(v);
                } else {
                    q.emplace_front(v);
                }
                from[v] = u;
            }
        }
    }
    return {dist, from};
}