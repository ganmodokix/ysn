// #REQ: graph
// Verified by https://atcoder.jp/contests/practice2/tasks/practice2_g?lang=ja
void scc_dfs(ll u, const graph &g, vector<ll> &vs, vector<bool> &used, ll &idx) {
    used[u] = true;
    for (auto [v, _] : g.adjacent_list[u]) {
        if (!used[v]) {
            scc_dfs(v, g, vs, used, idx);
        }
    }
    vs.push_back(u);
}
void scc_rdfs(ll u, ll k, const graph &g, vector<ll> &tpo, vector<bool> &used) {
    used[u] = true;
    tpo[u] = k;
    for (auto [v, _] : g.adjacent_list[u]) {
        if (!used[v]) {
            scc_rdfs(v, k, g, tpo, used);
        }
    }
}

// returns {list, g2}
// 連結成分はトポソされている
// list[i]: i番目の連結成分の頂点からなるリスト
// g2: 連結成分を一つの頂点とみなしたときの新しいグラフ、トポソよりfrom<to
pair<vector<vector<ll>>, graph> scc(const graph &g) {
    const graph rg = g.arc_reversed_graph();
    vector<ll> vs;
    ll idx = 0;
    {
        vector<bool> used(g.size(), false);
        REP(i, g.size()) if (!used[i]) {
            scc_dfs(i, g, vs, used, idx);
        }
    }
    vector<ll> tpo(g.size(), -1);
    ll k = 0;
    {
        vector<bool> used(g.size(), false);
        REP(i, vs.size()) {
            auto u = vs.end()[-1-i];
            if (!used[u]) {
                scc_rdfs(u, k++, rg, tpo, used);
            }
        }
    }
    vector<vector<ll>> res(k);
    REP(i, g.size()) {
        assert(tpo[i] != -1);
        res[tpo[i]].emplace_back(i);
    }
    graph gsub(k);
    for (auto [u, v, d] : g.arcs()) {
        gsub.add_arc(tpo[u], tpo[v], d);
    }
    return pair(res, gsub);
}