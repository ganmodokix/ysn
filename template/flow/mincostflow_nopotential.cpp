// #REQ: base_template
// 最小費用流 ポテンシャルなし O(FVE)
// CapacityとCostは符号付き整数型である必要がある（整数型でないと停止しない場合がある，符号付きでないと逆辺がバグる）
// Verified by AOJ GRL_6_B http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B
// Verified by ABC247-G https://atcoder.jp/contests/abc247/tasks/abc247_g
template <typename Capacity = long long, typename Cost = long long>
struct min_cost_flow {

    using edge = tuple<size_t, Capacity, Cost, size_t>; // {to, cap, cost, rev}
    
    size_t n;
    vector<vector<edge>> g;

    min_cost_flow(): n(0), g() {}
    min_cost_flow(size_t _n): n(_n), g(n) {}

    // 容量 cap, 流れ当たりのコスト cost の辺を追加
    void add_arc(size_t from, size_t to, Capacity cap, Cost cost) {
        g[from].emplace_back(to  , cap,  cost, g[to].size());
        g[to  ].emplace_back(from,   0, -cost, g[from].size()-1);
    }

    // 頂点 s から t に最大 f だけ *追加で* フローを流す
    // {新たにかかったコスト, 流れなかった残りのフロー} が返る
    pair<Cost, Capacity> min_cost(size_t s, size_t t, Capacity f) {

        Cost CINF = numeric_limits<Cost>::max();
        Cost result = 0;

        while (f > 0) {

            // Bellman-Ford法で最短路を見つける
            vector<Cost> dist(n, CINF);
            vector<size_t> prevv(n), preve(n);
            dist[s] = 0;
            for (bool yet = true; yet; ) {
                yet = false;
                REP(i, n) if (dist[i] < CINF) {
                    REP(j, g[i].size()) {
                        size_t to, rev; Capacity cap; Cost cost;
                        tie(to, cap, cost, rev) = g[i][j];
                        if (cap > 0 && chmin(dist[to], dist[i] + cost)) {
                            prevv[to] = i;
                            preve[to] = j;
                            yet = true;
                        }
                    }
                }
            }
            if (dist[t] == CINF) return {result, f};

            // 最短路に流す
            Capacity d = f;
            for (size_t i = t; i != s; i = prevv[i]) {
                auto &e = g[prevv[i]][preve[i]];
                chmin(d, get<1>(e));
            }
            f -= d;
            result += d * dist[t];
            for (size_t i = t; i != s; i = prevv[i]) {
                auto &e = g[prevv[i]][preve[i]];
                auto &r = g[i][get<3>(e)];
                get<1>(e) -= d;
                get<1>(r) += d;
            }

        }

        return {result, 0};

    }

};