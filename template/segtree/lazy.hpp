#pragma once
#include "base_template.hpp"

// 非再帰遅延セグ木
// (Value, Op): モノイド
// Query: 作用素
// Apply: 作用素をモノイド元に適用 (Query, Value) -> Value
// Composite: 作用素の合成 (Query, Query) -> Query, (f, g) |-> f○g
// ⚠️ iterative segment tree 型実装をとっていて、葉が [N+1, 2N+1) となっている
// [References]
// - https://hcpc-hokudai.github.io/archive/structure_segtree_001.pdf
// - https://qiita.com/hotman78/items/b52e0b84ed7082761bad
// - https://codeforces.com/blog/entry/18051
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

    // iterative segment tree で [l_, r_) を被覆する O(logN) 個の頂点を返す
    // 格納順は左から右になる; eval(roots[0]) op ... op eval(roots.end()[-1]) で求まる
    // time も O(logN) のはず
    vector<size_t> find_roots(ll l_, ll r_) const {
        auto [l, r] = regularize_segment(l_, r_);

        auto roots = vector<size_t>{};
        {
            auto rleft = vector<size_t>{};
            auto rright = vector<size_t>{};
            const auto offset = n + 1;
            l += offset;
            r += offset;
            while (l < r) {
                if (l & 1) rleft.emplace_back(l++);
                if (r & 1) rright.emplace_back(--r);
                l >>= 1;
                r >>= 1;
            }
            // roots = rleft + rright[::-1]
            roots = move(rleft);
            ranges::copy(rright | views::reverse, back_inserter(roots));
        }
        return roots;
    }

    // begin/l を固定して end/r を二分探索
    // predは Value を単一の引数として () 演算子が真偽値を返す必要がある
    // そのとき、pred(op(v[l:r])) が false となる最大の境界 r を返す
    // そのような境界が見つからなければ、すなわち op(v[l:]) のとき、無効値を返す
    // ⚠️ ACL と pred の真偽が逆な点に注意（ACLは単調減少で pred(id) == true）
    // verified at https://atcoder.jp/contests/abc426/submissions/73129083
    template <typename Pred>
    requires predicate<Pred, Value>
    optional<size_t> bisection_end(ll l_, Pred pred) {
        const auto l = l_ >= 0 ? (size_t)l_ : size_t{0};
        assert(l <= n);
        if (pred(identity_element)) return nullopt;
        const auto roots = find_roots(l, n);
        auto sum = identity_element;
        auto advance_and_add_if = [this, &sum, &pred](
            auto u, bool negates_prod = false
        ) -> bool {
            const auto term = op(sum, eval(u));
            if (pred(term) == negates_prod) {
                sum = term;
                return true;
            }
            return false;
        };
        eval_topdown(l + n + 1);
        eval_topdown(n * 2 + 1);
        for (const auto root : roots) {
            if (advance_and_add_if(root)) continue;
            auto u = root;
            const auto num_non_leaves = n + 1;
            while (u < num_non_leaves) {
                eval(u);
                eval(u <<= 1);
                if (advance_and_add_if(u)) ++u;
            }
            // trueの境界が端に来る場合は右端の要素(!=境界)で止まる
            if (u < n * 2 + 1) {
                if (advance_and_add_if(u)) ++u;
            }
            return u - num_non_leaves;
        }
        return n;
    }

    // verified at https://atcoder.jp/contests/abc426/submissions/73129132
    template <typename Pred>
    requires predicate<Pred, Value>
    optional<size_t> bisection_begin(ll r_, Pred pred) {
        assert(r_ >= 0);
        const auto r = r_ >= 0 ? (size_t)r_ : size_t{0};
        if (pred(identity_element)) return nullopt;
        const auto roots = find_roots(0, r);
        auto sum = identity_element;
        auto chadd_if = [this, &sum, &pred](
            const size_t u, bool negates_prod = false
        ) -> bool {
            const auto term = op(eval(u), sum);
            if (pred(term) == negates_prod) {
                sum = term;
                return true;
            }
            return false;
        };
        eval_topdown(n + 1);
        eval_topdown(r + n + 1);
        for (const auto root : roots | views::reverse) {
            if (chadd_if(root)) continue;
            auto u = root;
            const auto num_non_leaves = n + 1;
            while (u < num_non_leaves) {
                eval(u);
                eval(u <<= 1);
                if (++u >= n * 2 + 1) --u;
                if (chadd_if(u)) --u;
            }
            // trueの境界が端に来る場合は右端の要素(!=境界)で止まる
            if (u >= 0) {
                if (chadd_if(u)) --u;
            }
            return u + 1 - num_non_leaves;
        }
        return 0;
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

    // モノイド (値, op) の単位元を返す
    Value get_identity_value() const noexcept {
        return identity_element;
    }

    // モノイド (作用素, composite) の単位元を返す
    Query get_identity_query() const noexcept {
        return identity_query;
    }

};
