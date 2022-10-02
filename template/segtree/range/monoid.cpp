// #REQ: base_template segtree/lazy

// 区間モノイド区間更新
template <typename T, typename Op>
struct segment_monoid_monoid {

    lazy_segtree<T, Op, T, Op, Op> seg;

    segment_monoid_monoid(const vector<T> &a, T id, Op op):
        seg(a.begin(), a.end(), op, id, op, op, id)
    {}

    T fetch_range(ll l, ll r) { return seg.fetch_range(l, r); }
    T fetch_at(ll idx) { return seg.fetch_at(idx); }
    void apply_range(ll l, ll r, T f) { return seg.apply_range(l, r, f); }
    void apply_at(ll idx, T f) { return seg.apply_at(idx, f); }

};

// 区間モノイド区間更新
template <typename T, typename Op>
struct segment_monoid_update {

    lazy_segtree<T, Op, T, T(*)(T, T), T(*)(T, T)> seg;

    segment_monoid_update(const vector<T> &a, T id, Op op):
        seg(a.begin(), a.end(), op, id, &update, &update, id)
    {}

    static T update(T x, T) {
        return x;
    }

    T fetch_range(ll l, ll r) { return seg.fetch_range(l, r); }
    T fetch_at(ll idx) { return seg.fetch_at(idx); }
    void apply_range(ll l, ll r, T f) { return seg.apply_range(l, r, f); }
    void apply_at(ll idx, T f) { return seg.apply_at(idx, f); }

};