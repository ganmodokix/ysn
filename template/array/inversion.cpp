// #REQ: segtree/bit
// BITを用いて数列の転倒数を求める　O(NlogN)
template <typename T>
size_t inversion(vector<T> &x0) {
    
    vector<T> x = x0;
    const int n = x0.size();
    
    // 座圧
    vector<T> unq = x;
    sort(unq.begin(), unq.end());
    unq.erase(unique(unq.begin(), unq.end()), unq.end());
    map<T, int> sahz;
    for (size_t i = 0; i < unq.size(); i++) sahz[unq[i]] = i;
    for (size_t i = 0; i < x.size(); i++) x[i] = sahz[x[i]];
    
    // BITで転倒数を求める
    size_t ans = 0;
    BIT<int> bit(n);
    for (int i = 0; i < n; i++) {
        ans += bit.sum(x[i]+1, n);
        bit.add(x[i], 1);
    }
    
    return ans;
    
}
