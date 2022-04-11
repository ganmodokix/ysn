// #REQ: base_template
// 最小費用流 ポテンシャル付き O(FElogV)
// 費用について負閉路がない必要がある
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
        assert(cost >= 0);
        g[from].emplace_back(to  , cap,  cost, g[to].size());
        g[to  ].emplace_back(from,   0, -cost, g[from].size()-1);
    }

    // 頂点 s から t に最大 f だけ *追加で* フローを流す
    // {新たにかかったコスト, 流れなかった残りのフロー} が返る
    pair<Cost, Capacity> min_cost(size_t s, size_t t, Capacity f) {
        
        Cost CINF = numeric_limits<Cost>::max();
        Cost result = 0;
        vector<Cost> h(n, 0); // ポテンシャル

        while (f > 0) {

            // Dijkstra法+ポテンシャルで最短路を見つける
            using qitem = pair<Cost, size_t>;
            priority_queue<qitem, vector<qitem>, greater<qitem>> q;
            vector<Cost> dist(n, CINF);
            vector<size_t> prevv(n), preve(n);
            dist[s] = 0;
            q.emplace(0, s);
            while (q.size()) {
                const auto du = q.top(); q.pop();
                auto [d, u] = du;
                REP(i, g[u].size()) {
                    size_t to, rev; Capacity cap; Cost cost;
                    tie(to, cap, cost, rev) = g[u][i];
                    if (cap > 0 && chmin(dist[to], dist[u] + cost + h[u] - h[to])) {
                        prevv[to] = u;
                        preve[to] = i;
                        q.emplace(dist[to], to);
                    }
                }
            }
            if (dist[t] == CINF) return {result, f};
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

        return {result, 0};

    }

};