// #REQ: base_template.cpp graph.cpp graph/01bfs.cpp array/argsort.cpp

// ラベル付き（頂点を区別する）木からPrüferコードへの変換
std::vector<ll> tree_to_pruefer(const graph &tree) {

    assert(tree.size() >= 2);

    const auto n = tree.size();
    auto deg = vector<ll>(n, 0);
    priority_queue<ll, vector<ll>, greater<ll>> leaves;
    
    REP(i, n) {
        deg[i] = tree.adjacent_list[i].size();
        if (deg[i] == 1) {
            leaves.push(i);
        }
    }

    auto result = vector<ll>();
    result.reserve(n - 2);

    while (leaves.size()) {
        const auto u = leaves.top(); leaves.pop();

        for (const auto& [v, _] : tree.adjacent_list[u]) {
            if (deg[v] >= 2) {
                result.push_back(v);
            }
            if (--deg[v] == 1) {
                leaves.push(v);
            }
        }
    }

    return result;
    
}

// Prüferコードからラベル付き（頂点を区別する）木からへの変換
graph pruefer_to_tree(const vector<ll>& pruefer) {
    
    const auto n = (ll)pruefer.size() + 2;
    auto deg = vector<ll>(n, 1);

    for (const auto x : pruefer) {
        deg[x]++;
    }

    priority_queue<ll, vector<ll>, greater<ll>> leaves;
    REP(u, n) {
        if (deg[u] == 1) {
            leaves.push(u);
        }
    }

    auto tree = graph(n);
    for (const auto v : pruefer) {
        const auto u = leaves.top(); leaves.pop();
        tree.add_edge(u, v);
        --deg[u];
        if (--deg[v] == 1) {
            leaves.push(v);
        }
    }

    const auto it = find(deg.begin(), deg.end(), 1);
    assert(it != deg.end());
    const auto jt = find(next(it), deg.end(), 1);

    tree.add_edge(it - deg.begin(), jt - deg.begin());

    return tree;

}
