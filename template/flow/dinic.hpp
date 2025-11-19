#pragma once
#include "base_template.hpp"
#include "array/lazy_fill_vector.hpp"
// Dinic法による最大流
// Capacityには必ず整数型を指定すること
// BFSとDFSを組み合わせてO(EV^2)だが実用的にはもっと速いらしい
// 最大二部マッチングではO(E√V)，辺容量がすべて同じならばO(min{E^(1/2),V^(2/3)})
// <!> ちゃんとO(EV^2)になる最悪ケースが知られている
// Verified by AtC ABC010-D https://atcoder.jp/contests/abc010/tasks/abc010_4
// Verified by AOJ GRL_6_A http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A
template <typename Capacity = long long>
struct dinic {
    struct edge {
        size_t to = {};
        Capacity cap = {};
        size_t rev = {};
    };
    
    size_t n = {};
    vector<vector<edge>> g = {};
    lazy_fill_vector<size_t> level = {};
    lazy_fill_vector<size_t> iter = {};
    
    dinic() = default;
    dinic(const size_t _n) : n(_n), g(n), level(n), iter(n) {}
    
    void add_arc(const size_t from, const size_t to, const Capacity cap) {
        assert(from < n && to < n);
        assert(cap >= 0);
        g[from].emplace_back(edge{.to = to, .cap = cap, .rev = g[to].size()});
        g[to].emplace_back(edge{.to = from, .cap = 0, .rev = g[from].size() - 1});
    }
    
    void add_edge(const size_t u, const size_t v, const Capacity cap) {
        add_arc(u, v, cap);
        add_arc(v, u, cap);
    }

    void bfs(const size_t s) {
        constexpr auto ZINF = numeric_limits<size_t>::max();
        level.fill(ZINF);
        auto q = deque<size_t>{s};
        level[s] = 0;
        while (q.size()) {
            const auto u = q.front(); q.pop_front();
            for (const auto& [v, cap, rev] : g[u]) {
                if (cap > 0 && level[v] == ZINF) {
                    level[v] = level[u] + 1;
                    q.push_back(v);
                }
            }
        }
    }

    Capacity dfs(const size_t v, const size_t t, const Capacity f) {
        if (v == t) return f;
        for (auto& i = iter[v]; i < g[v].size(); i++) {
            auto& [to, cap, rev] = g[v][i];
            if (cap <= 0 || level[v] >= level[to]) continue;
            const auto d = dfs(to, t, min(f, cap));
            if (d > 0) {
                cap -= d;
                g[to][rev].cap += d;
                return d;
            }
        }
        return 0;
    }

    Capacity max_flow(size_t s, size_t t) {
        assert(s < n && t < n);
        constexpr size_t ZINF = numeric_limits<size_t>::max();
        if (s == t) return 0;
        auto f = Capacity{0};
        while (true) {
            bfs(s);
            if (level[t] == ZINF) return f;
            iter.fill(0);
            for (
                auto d = Capacity{};
                (d = dfs(s, t, numeric_limits<Capacity>::max())) > 0;
            ) {
                f += d;
            }
        }
    }
};
