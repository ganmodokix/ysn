// #REQ: base_template.hpp
// 座圧 {写像, 逆写像} が返る O(NlogN)
// verified by https://atcoder.jp/contests/abc036/tasks/abc036_c
template <typename T, typename U = size_t, typename M = unordered_map<T, U>>
pair<M, vector<T>> sahz(vector<T> x) {
    UNIQUE(x);
    M r;
    REP(i, x.size()) {
        r[x[i]] = i;
    }
    return {r, x};
}