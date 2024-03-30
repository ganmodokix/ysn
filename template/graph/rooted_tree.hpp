#pragma once
#include "base_template.hpp"
#include "graph.hpp"
#include "graph/01bfs.hpp"
#include "array/argsort.hpp"
// 連結の根付き木
struct rooted_tree {

    ll n;
    ll root;
    vector<pair<ll, ll>> parent; // {parent, cost to parent}
    static const ll NO_PARENT = -1;
    vector<vector<pair<ll, ll>>> children;
    vector<ll> depth;

    rooted_tree() = default;

    // 無向グラフから構成
    rooted_tree(ll root_, const graph &g):
        n(g.size()), root(root_), parent(), children(n), depth()
    {
        const auto &[dist, from] = dist_01bfs(root, g);
        parent = vector(n, pair(-1LL, -1LL));
        REP(i, n) {
            parent[i] = {from[i], from[i] == -1 ? 0 : dist[i] - dist[from[i]]};
        }
        build_children_from_parent();
        depth = dist;
    }

    // par[u] から構成
    template<typename T>
    rooted_tree(ll root_, const vector<T> &par):
        n(par.size()), root(root_), parent(n), children(n), depth()
    {
        REP(i, par.size()) {
            parent[i] = par[i];
        }
        build_children_from_parent();
        build_depth_from_parent();
    }

    // 構成用関数
    void build_children_from_parent() {
        children = vector(n, vector<pair<ll, ll>>());
        REP(i, n) {
            const auto &[par, cost] = parent[i];
            if (par != -1) {
                children[par].emplace_back(i, cost);
            }
        }
    }
    void build_depth_from_parent() {
        depth = vector<ll>(n, LINF);
        deque<ll> q = {root};
        depth[root] = 0;
        while (q.size()) {
            const auto u = q.front(); q.pop_front();
            for (const auto &[v, cost] : children[u]) {
                if (chmin(depth[v], depth[u] + 1)) {
                    q.push_back(v);
                } else {
                    DUMP("Not a tree");
                    assert(false);
                }
            }
        }
    }

    size_t size() const { return n; }

    // top-down DP 用 argsort
    vector<size_t> argsort_topdown() const {
        return argsort(depth);
    }
    // bottom-up DP 用 argsort
    vector<size_t> argsort_bottomup() const {
        auto v = argsort(depth);
        reverse(ALL(v));
        return v;
    }

};
