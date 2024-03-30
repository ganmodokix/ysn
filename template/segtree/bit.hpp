// #REQ: base_template
// Binary Indexed Tree モジュール (0-indexed)
template <typename T>
class BIT {
private:
    vector<T> val;
public:
    ull n;
    BIT(ull _n) : val(_n+1, 0), n(_n) {}
    BIT(vector<T> arr) : val(arr.size() + 1, 0), n(arr.size()) {
        REP(i, n) add(i, arr[i]);
    }
    // a[i] に v を足す O(logN)
    void add(const ull _i, const T _v) {
        for (ull i = _i + 1; i <= n; i += i & (-i)) {
            val[i] += _v;
        }
    }
    // [0, _i) の和を求める O(logN)
    T get(ull i) const {
        T ans = 0;
        for (; i > 0; i ^= i & (-i)) {
            ans += val[i];
        }
        return ans;
    }
    // [l, r) の和を求める o(N)
    T sum(const ull l, const ull r) {
        return get(r) - get(l);
    }
};