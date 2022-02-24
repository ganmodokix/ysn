// #REQ: base_template
// 最小費用流 ポテンシャル付き O(FElogV)
// CapacityとCostは符号付き整数型である必要がある（整数型でないと停止しない場合がある，符号付きでないと逆辺がバグる）
// Verified by AOJ GRL_6_B http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B
template <typename Capacity = long long, typename Cost = long long>
struct min_cost_flow {

    using edge = tuple<size_t, Capacity, Cost, size_t>; // {to, cap, cost, rev}
    
    size_t n;
    vector<vector<edge>> g;

    min_cost_flow(): n(0), g() {}
    min_cost_flow(size_t _n): n(_n), g(n) {}

    void add_arc(size_t from, size_t to, Capacity cap, Cost cost) {
        g[from].emplace_back(to  , cap,  cost, g[to].size());
        g[to  ].emplace_back(from,   0, -cost, g[from].size()-1);
    }

    // 流せない場合，numeric_limits<Cost>::max()が返る
    Cost min_cost(size_t s, size_t t, Capacity f) {
        Cost CINF = numeric_limits<Cost>::max();
        Cost FINF = numeric_limits<Capacity>::max();
        Cost result = 0;
        vector<Cost> h(n, 0); // ポテンシャル
        while (f > 0) {
            // Dijkstra法+ポテンシャルで最短路を見つける
            using qitem = pair<Cost, size_t>;
            priority_queue<qitem, vector<qitem>, greater<qitem>> q;
            vector<Cost> dist(n, CINF);
            vector<size_t> prevv(n), preve(n);
            dist[s] = 0;
            q.push(qitem(0, s));
            while (q.size()) {
                const auto du = q.top(); q.pop();
                Cost d; size_t u; tie(d, u) = du;
                REP(i, g[u].size()) {
                    size_t to, rev; Capacity cap; Cost cost;
                    tie(to, cap, cost, rev) = g[u][i];
                    if (cap > 0 && chmin(dist[to], dist[u] + cost + h[u] - h[to])) {
                        prevv[to] = u;
                        preve[to] = i;
                        q.push(qitem(dist[to], to));
                    }
                }
            }
            if (dist[t] == CINF) return CINF;
            REP(i, n) h[i] += dist[i];
            // 最短路に流す
            Capacity d = f;
            for (size_t i = t; i != s; i = prevv[i]) {
                auto &e = g[prevv[i]][preve[i]];
                chmin(d, get<1>(e));
            }
            f -= d;
            result += d * h[t];
            for (size_t i = t; i != s; i = prevv[i]) {
                auto &e = g[prevv[i]][preve[i]];
                auto &r = g[i][get<3>(e)];
                get<1>(e) -= d;
                get<1>(r) += d;
            }
        }
        return result;
    }

};