#pragma once
#include "base_template.hpp"
// 関係式付き Union-Find モジュール
// (型 Potential, 二項演算 op, 単位元 id, 逆元 Inv) は群をなす必要がある
// "x_i = x_j op d" の集合を管理する
template<typename Potential, typename Op, Potential id, typename Inv>
struct relational_union_find {
    
    vector<size_t> par; // par[i] := 頂点 i の親
    vector<size_t> num; // num[i] := 頂点 i を根とする連結成分の頂点数
    vector<Potential> ptd; // ポテンシャル差 ptd[i] := x_i op inv(x_par[i])
    // ptd を葉から累積 op するとポテンシャル値 x_i op inv(x_root(i)) が得られる

    Op op; // 演算
    Inv inv; // 逆元を返す関数
    
    relational_union_find(size_t n, Op op_ = {}, Inv inv_ = {}):
        par(n, 0), num(n, 1), ptd(n, id), op(op_), inv(inv_)
    {
        iota(ALL(par), 0);
    }

    // UF木について、頂点 i の根を求める
    size_t root(size_t i) {
        if (par[i] != i) {
            const auto r = root(par[i]);
            ptd[i] = op(ptd[par[i]], ptd[i]);
            par[i] = r;
        }
        return par[i];
    }

    // すべての頂点/変数をrerootする
    void reroot_all() {
        for (size_t i = 0; i < par.size(); i++) root(i);
    }
    
    // i, j が同じ連結成分のとき iif true（関係式でいうと従属変数の関係にあるか）
    bool same(size_t i, size_t j) {
        return root(i) == root(j);
    }

    // 関係式 x_i = x_j op d を追加
    // UF木で i -> j に重み d の辺を張ることに相当
    bool unite(size_t i, size_t j, Potential d) {
        size_t ri = root(i);
        size_t rj = root(j);
        if (ri != rj) {
            d = op(op(potential(j), d), inv(potential(i)));
            if (num[ri] > num[rj]) {
                swap(ri, rj);
                d = inv(d);
            }
            ptd[ri] = d;
            par[ri] = rj;
            num[rj] += num[ri];
            return true;
        } else {
            return false;
        }
    }

    // potential(u) := x_u op inv(x_root(u))
    Potential potential(size_t i) {
        root(i);
        return ptd[i];
    }

    // same(i, j) ならば、diff(i, j) = x_i op inv(x_j)
    // ※ !same(i, j) ならば x_i と x_j は独立変数
    Potential diff(size_t i, size_t j) {
        return op(potential(i), inv(potential(j)));
    }

};

// 関係式の演算を通常の加算としたポテンシャル付きUF
// verified by ABC328-F https://atcoder.jp/contests/abc328/tasks/abc328_f
template <typename T>
requires ::std::is_integral_v<T> || ::std::is_floating_point_v<T>
struct potentialized_union_find : relational_union_find<T, ::std::plus<T>, T(0), ::std::negate<T>> {};
