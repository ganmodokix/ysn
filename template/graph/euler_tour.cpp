// #REQ: graph
void euler_tour_dfs(
    ll u, const graph &g,
    vector<ll> depth,
    vector<tuple<char, ll, ll>> &tour
) {
    for (auto [v, cost] : g.adjacent_list[u]) {
        if (chmin(depth[v], depth[u] + 1)) {
            tour.emplace_back('A', v, cost);
            euler_tour_dfs(v, g, depth, tour);
            tour.emplace_back('A', v, cost);
        }
    }
}
// 無向グラフ g に対して root の属する連結成分の全域木のオイラーツアーを返す
// returns {direction, vertex, edge cost}
// direction: 'A' なら arrival, 'D' なら departure
// vertex: vertex under consideration
// edge cost: 来た/行く辺の重み、vertex == root のときは 0
auto euler_tour(ll root, const graph &g) {
    vector<tuple<char, ll, ll>> tour;
    vector<ll> depth(g.size());
    depth[root] = 0;
    tour.emplace_back('A', root, 0);
    euler_tour_dfs(root, g, depth, tour);
    tour.emplace_back('D', root, 0);
    return tour;
}