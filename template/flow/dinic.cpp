// Dinic法による最大流
// Capacityには必ず整数型を指定すること
// BFSとDFSを組み合わせてO(EV^2)だが実用的にはもっと速いらしい
// 最大二部マッチングではO(E√V)，辺容量がすべて同じならばO(min{E^(1/2),V^(2/3)})
// <!> ちゃんとO(EV^2)になる最悪ケースが知られている
// Verified by AtC ABC010-D https://atcoder.jp/contests/abc010/tasks/abc010_4
// Verified by AOJ GRL_6_A http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A
template <typename Capacity = long long>
struct dinic {
    using edge = tuple<size_t, Capacity, size_t>; // {to, cap, rev idx at g[to]}
    size_t n;
    vector<vector<edge>> g;
    vector<size_t> level, iter;
    dinic() : n(0), g(), level(), iter() {}
    dinic(size_t _n) : n(_n), g(n), level(n), iter(n) {}
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
    void bfs(size_t s) {
        constexpr ll ZINF = numeric_limits<size_t>::max();
        fill(ALL(level), ZINF);
        deque<size_t> q = {s};
        level[s] = 0;
        while (q.size()) {
            size_t u = q.front(); q.pop_front();
            for (const auto &[v, cap, _] : g[u]) {
                if (cap > 0 && level[v] == ZINF) {
                    level[v] = level[u] + 1;
                    q.push_back(v);
                }
            }
        }
    }
    Capacity dfs(size_t v, size_t t, Capacity f) {
        if (v == t) return f;
        for (size_t &i = iter[v]; i < g[v].size(); i++) {
            auto& [to, cap, rev] = g[v][i];
            if (cap <= 0 || level[v] >= level[to]) continue;
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
        constexpr ll ZINF = numeric_limits<size_t>::max();
        if (s == t) return 0;
        Capacity f = 0;
        while (true) {
            bfs(s);
            if (level[t] == ZINF) return f;
            fill(ALL(iter), 0);
            Capacity d;
            while ((d = dfs(s, t, numeric_limits<Capacity>::max())) > 0) {
                f += d;
            }
        }
    }
};