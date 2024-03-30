// #REQ: base_template.hpp graph.hpp graph/01bfs.hpp
// {直径, 端, 端} を返す
tuple<ll, ll, ll> tree_diameter(const graph &tree) {
    auto [dist1, from1] = dist_01bfs(0, tree);
    ll u = max_element(ALL(dist1)) - dist1.begin();
    auto [dist2, from2] = dist_01bfs(u, tree);
    ll v = max_element(ALL(dist2)) - dist2.begin();
    return {dist2[v], u, v};
}