// #REQ: base_template
// Union-Find モジュール
struct union_find {
    vector<size_t> par;
    vector<size_t> num;
    union_find() = default;
    union_find(size_t n) : par(n, 0), num(n, 1) { iota(ALL(par), 0); }
    size_t root(size_t i) { return par[i] == i ? i : (par[i] = root(par[i])); }
    void reroot_all() { for (size_t i = 0; i < par.size(); i++) root(i); }
    bool same(size_t i, size_t j) { return root(i) == root(j); }
    bool unite(size_t i, size_t j) {
        size_t ri = root(i);
        size_t rj = root(j);
        if (ri != rj) {
            if (num[ri] > num[rj]) swap(ri, rj);
            par[ri] = rj;
            num[rj] += num[ri];
            return true;
        } else {
            return false;
        }
    }
};