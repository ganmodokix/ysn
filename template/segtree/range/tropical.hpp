#pragma once
#include "base_template.hpp"
#include "opf.hpp"
#include "segtree/lazy.hpp"

// range min, range add
template <typename T>
auto segtree_min_add(const vector<T> &v) {
    T idm = numeric_limits<T>::max();
    T idp = 0;
    return lazy_segtree(v.begin(), v.end(), opf::min(), idm, opf::plus(), opf::plus(), idp);
}

// range max, range add
template <typename T>
auto segtree_max_add(const vector<T> &v) {
    T idm = numeric_limits<T>::min();
    T idp = 0;
    return lazy_segtree(v.begin(), v.end(), opf::max(), idm, opf::plus(), opf::plus(), idp);
}
