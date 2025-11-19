#pragma once
#include "base_template.hpp"
#include "array/lazy_fill_vector.hpp"
// Ford-Fulkerson法による最大流
// Capacityには必ず整数型を指定すること
// Capacityが整数ならば，停止しないときDFS1回O(E)で必ず1以上フローが新たに流れるためO(FE)
// Verified by AtC ABC010-D https://atcoder.jp/contests/abc010/tasks/abc010_4
// Verified by AOJ GRL_6_A http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A
template <typename Capacity = long long>
struct ford_fulkerson {
    struct edge {
        size_t to = {};
        Capacity cap = {};
        size_t rev = {};
    };

    size_t n = {};
    vector<vector<edge>> g = {};
    lazy_fill_vector<uint_fast8_t> used = {};
    
    ford_fulkerson() : n(0), g(), used() {}
    ford_fulkerson(const size_t _n) : n(_n), g(n), used(n) {}

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

    Capacity dfs(const size_t v, const size_t t, const Capacity f) {
        if (v == t) return f;
        used[v] = true;
        for (auto& [to, cap, rev] : g[v]) {
            if (used[to] || cap <= 0) continue;
            const auto d = dfs(to, t, min(f, cap));
            if (d > 0) {
                cap -= d;
                g[to][rev].cap += d;
                return d;
            }
        }
        return 0;
    }

    Capacity max_flow(const size_t s, const size_t t) {
        assert(s < n && t < n);
        if (s == t) return 0;
        auto f = Capacity{0};
        while (true) {
            used.fill(false);
            const auto d = dfs(s, t, numeric_limits<Capacity>::max());
            if (d == 0) return f;
            f += d;
        }
    }
};
