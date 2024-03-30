#pragma once
#include "base_template.hpp"
#include "graph.hpp/rooted_tree.hpp"
#include "bisection.hpp"
// 根付き木上でのダブリング
struct tree_doubling {
    
    ll n;
    const rooted_tree &rt;
    vector<vector<ll>> parent_func; // v
    vector<vector<ll>> cost_func; // cost
    ll maxdoubling;

    // O(NlogN)
    tree_doubling(const rooted_tree &rt_):
        n(rt_.size()), rt(rt_), parent_func(), cost_func(), maxdoubling(0)
    {
        while ((1LL << maxdoubling) < n) ++maxdoubling;
        parent_func = vector(maxdoubling + 1, vector(n, -1LL));
        cost_func = vector(maxdoubling + 1, vector(n, 0LL));
        REP(x, n) {
            parent_func[0][x] = rt.parent[x].first;
            cost_func[0][x] = rt.parent[x].second;
            if (parent_func[0][x] == -1) {
                parent_func[0][x] = x;
            }
        }
        REP(i, maxdoubling) {
            REP(x, n) {
                parent_func[i+1][x] = parent_func[i][parent_func[i][x]];
                cost_func[i+1][x] = cost_func[i][x] + cost_func[i][parent_func[i][x]];
            }
        }
    }

    // {uからn個だけ登った頂点, u-v間のコスト} O(log n)
    pair<ll, ll> nth_parent(ll u, ll n) const {
        ll v = u;
        ll cost = 0;
        for (ll i = 0, m = n; m; i++, m >>= 1) {
            if (m & 1) {
                cost += cost_func[i][v];
                v = parent_func[i][v];
            }
        }
        return {v, cost};
    }

    // {LCA, u-l間のコスト, v-l間のコスト} O(log #V)
    tuple<ll, ll, ll> lca(ll u, ll v) const {
        if (rt.depth[u] > rt.depth[v]) {
            const auto &[l, vbc, ubc] = lca(v, u);
            return {l, ubc, vbc};
        }
        ll ubc = 0, vbc = 0, vbcc = 0, l;
        tie(v, vbcc) = nth_parent(v, rt.depth[v] - rt.depth[u]);
        ll d = bisection<ll>(-1, rt.depth[u], [&](ll m) { return nth_parent(u, m) == nth_parent(v, m); });
        tie(l, ubc) = nth_parent(u, d);
        tie(std::ignore, vbc) = nth_parent(v, d);
        return {l, ubc, vbc + vbcc};
    }

    ll dist(ll u, ll v) const {
        auto [l, ubc, vbc] = lca(u, v);
        return ubc + vbc;
    }

};
