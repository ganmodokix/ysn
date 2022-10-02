// #REQ: base_template

// 非再帰遅延セグ木
// (Value, Op): モノイド
// Query: 作用素
// Apply: 作用素をモノイド元に適用 (Query, Value) -> Value
// Composite: 作用素の合成 (Query, Query) -> Query, (f, g) |-> f○g
// [References]
// - https://hcpc-hokudai.github.io/archive/structure_segtree_001.pdf
// - https://qiita.com/hotman78/items/b52e0b84ed7082761bad
template<typename Value, typename Op, typename Query, typename Apply, typename Composite>
class lazy_segtree {

    Value identity_element;
    Query identity_query;
    Op op;
    Apply apply;
    Composite composite;

    size_t n;

    // 1-index にするとノード i に対して親ノード floor(i/2), 子ノード {2i, 2i+1}
    vector<Value> node;
    vector<Query> lazy;
    vector<bool> ready;

    public:

    lazy_segtree() = default;

    lazy_segtree(
        size_t n_, Value fill_value,
        Op op_, Value idvalue_, Apply apply_, Composite composite_, Query idquery_
    ) noexcept:
        identity_element(idvalue_), identity_query(idquery_),
        op(op_), apply(apply_), composite(composite_),
        n(n_),
        node(n * 2 + 1, fill_value),
        lazy(n * 2 + 1, identity_query),
        ready(n * 2 + 1, true)
    {
        build_values();
    }

    template <typename Iterator>
    lazy_segtree(
        Iterator begin, Iterator end,
        Op op_, Value idvalue_, Apply apply_, Composite composite_, Query idquery_
    ) noexcept:
        identity_element(idvalue_), identity_query(idquery_),
        op(op_), apply(apply_), composite(composite_),
        n(distance(begin, end)),
        node(n * 2 + 1),
        lazy(n * 2 + 1, identity_query),
        ready(n * 2 + 1, true)
    {
        Iterator itr = begin;
        size_t idx = 0;
        for (; itr != end; itr = next(itr), ++idx) {
            node[idx + n + 1] = *itr;
        }
        build_values();
    }

    private:

    // bottom-up に初期化
    void build_values() {
        for (size_t i = n; i >= 1; i--) {
            node[i] = i*2+1 <= n*2 ? op(node[i*2], node[i*2+1]) : node[i*2];
        }
    }

    pair<size_t, size_t> regularize_segment(ll l_, ll r_) const {
        const size_t l = max(0LL, min<ll>(n, l_));
        const size_t r = max(0LL, min<ll>(n, r_));
        return {l, r};
    }

    bool empty_segment(size_t l, size_t r) const {
        return l >= r;
    }

    Value eval(size_t v) {
        if (ready[v]) return node[v];
        node[v] = apply(lazy[v], node[v]);
        if (v < n) {
            lazy[v*2] = composite(lazy[v], lazy[v*2]);
            lazy[v*2+1] = composite(lazy[v], lazy[v*2+1]);
            ready[v*2] = ready[v*2+1] = false;
        }
        lazy[v] = identity_query;
        ready[v] = true;
        return node[v];
    }
    
    // 遅延評価 O(logN)
    Value eval_topdown(size_t k) {
        while (k > n * 2) k /= 2;
        if (!k) return identity_element;
        auto depth = 63 - __builtin_clzll((uint64_t)k);
        for (int d = depth; d >= 0; d--) {
            eval(k >> d);
        }
        return node[k];
    }

    void eval_bottomup(size_t k) {
        k >>= __builtin_ctzll(k) + 1;
        while (k >= 1) {
            node[k] = k*2+1 > n*2 ? eval(k*2) : op(eval(k*2), eval(k*2+1));
            k /= 2;
        }
    }

    public:

    size_t size() const {
        return n;
    }

    // [l, r) の総opを返す O(logN)
    // [l,r) = ∅ の場合は単位元を返す
    Value fetch_range(ll l_, ll r_) {

        auto [l, r] = regularize_segment(l_, r_);
        if (empty_segment(l, r)) {
            return identity_element;
        }

        l += n + 1;
        r += n + 1;
        auto sum_left = identity_element;
        auto sum_right = identity_element;
        eval_topdown(l);
        eval_topdown(r);
        while (l < r) {
            if (l & 1) sum_left = op(sum_left, eval(l++));
            if (r & 1) sum_right = op(eval(--r), sum_right);
            l >>= 1;
            r >>= 1;
        }
        return op(sum_left, sum_right);

    }

    // a[i] を返す
    // 範囲外の場合は単位元を返す
    Value fetch_at(size_t idx) {
        if (idx >= n) return identity_element;
        return fetch_range(idx, idx + 1);
    }

    // 作用素 f を [l, r) に適用 O(logN)
    void apply_range(ll l_, ll r_, Query f) {

        auto [l, r] = regularize_segment(l_, r_);
        if (empty_segment(l, r)) {
            return;
        }

        l += n + 1;
        r += n + 1;
        
        auto l0 = l, r0 = r;
        eval_topdown(l0);
        eval_topdown(r0);
        while (l < r) {
            if (l & 1) {
                eval(l);
                assert(ready[l]);
                lazy[l] = f;
                ready[l] = false;
                l++;
            }
            if (r & 1) {
                --r;
                eval(r);
                assert(ready[r]);
                lazy[r] = f;
                ready[r] = false;
            }
            l >>= 1;
            r >>= 1;
        }
        eval_bottomup(l0);
        eval_bottomup(r0);

    }

    // 作用素 f を a[i] に適用する
    // 範囲外の場合はなにもしない
    void apply_at(size_t idx, Query f) {
        if (idx >= n) return;
        return apply_range(idx, idx + 1, f);
    }

};