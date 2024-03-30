#pragma once
#include "base_template.hpp"
// Binary Indexed Tree モジュール (0-indexed)
template <typename T>
class BIT {
private:
    vector<vector<T>> val;
public:
    ull h, w;
    BIT(ull _h, ull _w) : val(_h+1, vector<T>(_w+1, 0)), h(_h), w(_w) {}
    BIT(vector<vector<T>> tbl) : val(tbl.size()+1, vector<T>(tbl[0].size()+1, 0)), h(tbl.size()), w(tbl[0].size()) {
        REP(i, h) REP(j, w) {
            add(i, j, tbl[i][j]);
        }
    }
    // a[i][j] += v
    void add(const ull _i, const ull _j, const T _v) {
        for (ull i = _i + 1; i <= h; i += i & (-i)) {
            for (ull j = _j + 1; j <= w; j += j & (-j)) {
                val[i][j] += _v;
            }
        }
    }
    // Σ_{0<=i<_i,0<=j<_j}a[i][j] を求める
    T get(const ull _i, const ull _j) const {
        T ans = 0;
        for (ull i = _i; i > 0; i ^= i & (-i)) {
            for (ull j = _j; j > 0; j ^= j & (-j)) {
                ans += val[i][j];
            }
        }
        return ans;
    }
};
