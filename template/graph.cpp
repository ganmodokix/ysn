struct graph {
    vector<vector<pair<ll, ll>>> adjacent_list;
    graph() = default;
    graph(size_t n): adjacent_list(n) {}
    void add_arc(ll u, ll v, ll cost = 1) { adjacent_list[u].emplace_back(v, cost); }
    void add_edge(ll u, ll v, ll cost = 1) { add_arc(u, v, cost); add_arc(v, u, cost); }
    auto size() const { return adjacent_list.size(); }
    auto edge_list() const {
        vector<tuple<ll, ll, ll>> e;
        REP(u, size()) {
            for (auto [v, d] : adjacent_list[u]) {
                e.emplace_back(u, v, d);
            }
        }
        return e;
    }
    graph reverse_edges() const {
        graph g(size());
        REP(u, size()) {
            for (auto [v, d] : adjacent_list[u]) {
                g.add_arc(v, u, d);
            }
        }
        return g;
    }
};