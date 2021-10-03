// Ford-Fulkerson法による最大流
// Capacityには必ず整数型を指定すること
// Capacityが整数ならば，停止しないときDFS1回O(E)で必ず1以上フローが新たに流れるためO(FE)
// Verified by AtC ABC010-D https://atcoder.jp/contests/abc010/tasks/abc010_4
// Verified by AOJ GRL_6_A http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A
template <typename Capacity = long long>
struct ford_fulkerson {
    using edge = tuple<size_t, Capacity, size_t>; // {to, cap, rev idx at g[to]}
    size_t n;
    vector<vector<edge>> g;
    vector<uint_fast8_t> used;
    ford_fulkerson() : n(0), g(), used() {}
    ford_fulkerson(size_t _n) : n(_n), g(n), used(n) {}
    void add_arc(size_t from, size_t to, Capacity cap) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        assert(cap >= 0);
        g[from].emplace_back(to  , cap, g[to  ].size());
        g[to  ].emplace_back(from,   0, g[from].size() - 1);
    }
    void add_edge(size_t u, size_t v, Capacity cap) {
        add_arc(u, v, cap);
        add_arc(v, u, cap);
    }
    Capacity dfs(size_t v, size_t t, Capacity f) {
        if (v == t) return f;
        used[v] = true;
        for (auto &[to, cap, rev] : g[v]) {
            if (used[to] || cap <= 0) continue;
            Capacity d = dfs(to, t, min(f, cap));
            if (d > 0) {
                cap -= d;
                get<1>(g[to][rev]) += d;
                return d;
            }
        }
        return 0;
    }
    Capacity max_flow(size_t s, size_t t) {
        assert(0 <= s && s < n && 0 <= t && t < n);
        if (s == t) return 0;
        Capacity f = 0;
        while (true) {
            fill(ALLOF(used), false);
            Capacity d = dfs(s, t, numeric_limits<Capacity>::max());
            if (d == 0) return f;
            f += d;
        }
    }
};