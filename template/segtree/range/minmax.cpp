// #REQ: base_template opf segtree/range/monoid

// range min, range update
template <typename T>
auto segtree_min_update(const vector<T> &v) {
    T id = numeric_limits<T>::max();
    return segment_monoid_update(v, id, opf::min());
}

// range min, range min
template <typename T>
auto segtree_min_min(const vector<T> &v) {
    T id = numeric_limits<T>::max();
    return segment_monoid_monoid(v, id, opf::min());
}

// range max, range update
template <typename T>
auto segtree_max_update(const vector<T> &v) {
    T id = numeric_limits<T>::min();
    return segment_monoid_update(v, id, opf::max());
}

// range max, range max
template <typename T>
auto segtree_max_max(const vector<T> &v) {
    T id = numeric_limits<T>::min();
    return segment_monoid_monoid(v, id, opf::max());
}