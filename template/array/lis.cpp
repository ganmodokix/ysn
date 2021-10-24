// #REQ: array/sahz
// LIS の長さを返す O(NlogN)
template <typename T>
size_t lis_length(const vector<T> &x) {
    auto [xm, mi] = sahz(x);
    vector<ll> y = x;
    FOREACH(yi, y) yi = xm[yi];
    vector<T> dp(mi.size(), numeric_limits<T>::max());
    REP(i, y.size()) {
        *lower_bound(ALL(dp), y[i]) = y[i];
    }
    return lower_bound(ALL(dp), LINF) - dp.begin();
}