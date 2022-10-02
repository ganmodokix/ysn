
template<typename T>
struct segtree_sum_add {

    segtree_sum_affine<T> seg;

    segtree_sum_add(const vector<T> &a): seg(a) {}

    T fetch_range(ll l, ll r) { return seg.fetch_range(l, r); }
    T fetch_at(ll idx) { return seg.fetch_at(idx); }
    void apply_range(ll l, ll r, T f) { return seg.apply_range(l, r, pair(T(1), f)); }
    void apply_at(ll idx, T f) { return seg.apply_at(idx, pair(T(1), f)); }

};

template<typename T>
struct segtree_sum_update {

    segtree_sum_affine<T> seg;

    segtree_sum_update(const vector<T> &a): seg(a) {}

    T fetch_range(ll l, ll r) { return seg.fetch_range(l, r); }
    T fetch_at(ll idx) { return seg.fetch_at(idx); }
    void apply_range(ll l, ll r, T f) { return seg.apply_range(l, r, pair(T(0), f)); }
    void apply_at(ll idx, T f) { return seg.apply_at(idx, pair(T(0), f)); }

};