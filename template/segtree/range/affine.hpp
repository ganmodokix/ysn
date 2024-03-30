#pragma once
#include "base_template.hpp"
#include "opf.hpp"
#include "segtree/lazy.hpp"

// 区間和/区間Affine
template<typename T>
struct segtree_sum_affine {
    
    using value_type = pair<T, size_t>;
    using query_type = pair<T, T>; // ax+b の {a, b} の部分
    
    lazy_segtree<
        value_type,
        value_type(*)(value_type, value_type),
        query_type,
        value_type(*)(query_type, value_type),
        query_type(*)(query_type, query_type)
    > seg;

    segtree_sum_affine(const vector<T> &a) {
        value_type idv(0, 1);
        query_type idq(1, 0);
        vector<value_type> v(a.size(), idv);
        REP(i, a.size()) {
            v[i].first = a[i];
        }
        seg = lazy_segtree(v.begin(), v.end(), &op, idv, &apply, &composite, idq);
    }

    static value_type op(value_type a, value_type b) {
        return {a.first + b.first, a.second + b.second};
    }

    static value_type apply(query_type f, value_type x) {
        return {x.first * f.first + f.second * x.second, x.second};
    }

    static query_type composite(query_type f, query_type g) {
        return {f.first * g.first, f.first * g.second + f.second};
    }

    T fetch_range(ll l, ll r) { return seg.fetch_range(l, r).first; }
    T fetch_at(ll idx) { return seg.fetch_at(idx).first; }
    void apply_range(ll l, ll r, query_type f) { return seg.apply_range(l, r, f); }
    void apply_at(ll idx, query_type f) { return seg.apply_at(idx, f); }

};
