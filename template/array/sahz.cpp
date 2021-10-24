// 座圧 {写像, 逆写像} が返る O(NlogN)
template <typename T>
pair<map<T, int>, vector<T>> sahz(const vector<T> &x0) {
    vector<T> x = x0;
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());
    map<T, int> r;
    for (int i = 0; i < (int)x.size(); i++) {
        r[x[i]] = i;
    }
    return make_pair(r, x);
}