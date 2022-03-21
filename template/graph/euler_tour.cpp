// #REQ: base_template graph
// 無向グラフ g に対して root の属する連結成分の全域木のオイラーツアーを返す
// returns {direction, vertex, edge cost}
// direction: 'A' なら arrival, 'D' なら departure
// vertex: 見ている頂点で，directionが'A'のときは遷移*先*，'D'のときは遷移*元* (それぞれ元/先でない方の頂点はそのparent)
// edge cost: 来た/行く辺の重み、vertex == root のときは 0
auto euler_tour(ll root, const graph &g) {

    vector<tuple<char, ll, ll>> tour;

    vector<ll> depth(g.size(), LINF);
    vector<ll> parent(g.size(), -1);
    deque<pair<ll, ll>> q = {{-root, 0LL}, {root, 0LL}};
    depth[root] = 0;
    
    while (q.size()) {
        auto [u, costu] = q.back(); q.pop_back();
        if (u >= 0) {
            for (auto [v, costv] : g.adjacent_list[u]) {
                if (depth[v] == LINF) {
                    depth[v] = depth[u] + 1;
                    parent[v] = u;
                    tour.emplace_back('A', v, costv);
                    q.emplace_back(v, costv);
                    q.emplace_back(~v, costv);
                }
            }
        } else {
            tour.emplace_back('D', ~u, costu);
        }
    }

    return tuple(tour, depth, parent);

}