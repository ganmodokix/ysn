// #REQ: base_template.hpp
template <typename T, typename Op>
struct cumsum {
    vector<ll> s;
    // a: array, op: monoid, it: identity element of (T, op)
    cumsum(const vector<T> &a, Op op, T id) {
        s.resize(a.size()+1, id);
        REP(i, a.size()) {
            s[i+1] = op(s[i], a[i]);
        }
    }
    // sum of a[i] for i in [0, n)
    T sum(size_t i) const { return s.at(i); }
};
