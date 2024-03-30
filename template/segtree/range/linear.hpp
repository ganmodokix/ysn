#pragma once
#include "base_template.hpp"
#include "opf.hpp"
#include "segtree/lazy.hpp"

// 区間ベクトル和/区間線形変換
template<typename T, size_t n>
struct segtree_linear {
    
    using value_type = array<T, n>; // n-vector
    using query_type = array<T, n * n>; // n*n-matrix
    
    lazy_segtree<
        value_type,
        value_type(*)(const value_type &, const value_type &),
        query_type,
        value_type(*)(const query_type &, const value_type &),
        query_type(*)(const query_type &, const query_type &)
    > seg;

    segtree_linear(const vector<array<T, n>> &a) {
        value_type idv; REP(i, n) idv[i] = 0;
        query_type idq; REP(i, n) REP(j, n) idv[i * n + j] = i == j;
        vector<value_type> v(a.size(), idv);
        REP(i, a.size()) {
            v[i].first = a[i];
        }
        seg = lazy_segtree(v.begin(), v.end(), &op, idv, &apply, &composite, idq);
    }

    static value_type op(const value_type &a, const value_type &b) {
        value_type c;
        REP(i, n) c[i] = a[i] + b[i];
        return c;
    }

    static value_type apply(const query_type &f, const value_type &x) {
        value_type y;
        REP(i, n) {
            y[i] = 0;
            REP(j, n) {
                y[i] += f[i * n + j] * x[j];
            }
        }
        return y;
    }

    static query_type composite(const query_type &f, const query_type &g) {
        query_type h;
        REP(i, n * n) h[i] = 0;
        REP(i, n) REP(k, n) RPE(j, n) {
            h[i * n + j] = f[i * n + k] * g[k * n + j];
        }
        return h;
    }

    value_type fetch_range(ll l, ll r) { return seg.fetch_range(l, r); }
    value_type fetch_at(ll idx) { return seg.fetch_at(idx); }
    void apply_range(ll l, ll r, query_type f) { return seg.apply_range(l, r, f); }
    void apply_at(ll idx, query_type f) { return seg.apply_at(idx, f); }

};
