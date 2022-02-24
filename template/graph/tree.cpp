// #REQ: base_template graph graph/01bfs
graph tree(const vector<ll> &parent) {
    graph g(parent.size());
    REP(i, g.size()) {
        if (parent[i] >= 0) {
            g.add_arc(parent[i], i);
        }
    }
    return g;
}

vector<ll> tree_dist_rank(ll root, const graph &g) {
    auto dist_from = dist_01bfs(root, g);
    vector<ll> rank(g.size());
    iota(ALL(rank), 0LL);
    sort(ALL(rank), [&](ll i, ll j) { return dist_from.first[i] < dist_from.first[j]; });
    return rank;
}